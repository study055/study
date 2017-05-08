#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <linux/fb.h> // /usr/include/linux/fb.h

#include "bmp.h"

typedef unsigned long u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef signed long s32;
typedef signed short s16;
typedef signed char s8;

#define DEVNAME "/dev/fb0"

#define RGB888(r, g, b) ( (r) << 16 | (g) << 8 | (b))

struct ldm_info {
	int fd;
	//理论上由应用层修改配置的参数（实际不可能接受你的修改）
	struct fb_var_screeninfo var;
	//固定参数，逻辑上就不可能让应用层修改
	struct fb_fix_screeninfo fix;
	size_t pixel_size; //每个像素占多少字节
	size_t x, y; //显存的实际分辨率
	u8 *addr; //映射到当前进程空间的显存首地址
	/* 屏蔽底层屏幕参数信息的画点函数，只要传入统一的参数就能画点，无需考虑屏幕参数细节
	 * @x, y: 目标像素点的坐标
	 * @color：像素点颜色，标准RGB888色彩格式
	 */
	void (*draw_pixel)(struct ldm_info *, size_t x, size_t y, u32 color);
};
static struct ldm_info ldm;

static void draw_pixel_32bit(struct ldm_info *info, size_t x, size_t y, u32 color)
{
	//调整RGB888格式的颜色到当前屏幕的色深格式
	u8 red = ((color >> 16) & 0xff) >> (8 - ldm.var.red.length);
	u8 green = ((color >> 8) & 0xff) >> (8 - ldm.var.green.length);
	u8 blue = ((color) & 0xff) >> (8 - ldm.var.blue.length);

	u32 disp_color = (red << ldm.var.red.offset) | (green << ldm.var.green.offset) | (blue << ldm.var.blue.offset);

	*(u32*)(info->addr + y * info->fix.line_length + x * info->pixel_size) = disp_color;
}

static void draw_pixel_16bit(struct ldm_info *info, size_t x, size_t y, u32 color)
{
	//调整RGB888格式的颜色到当前屏幕的色深格式
	u8 red = ((color >> 16) & 0xff) >> (8 - ldm.var.red.length);
	u8 green = ((color >> 8) & 0xff) >> (8 - ldm.var.green.length);
	u8 blue = ((color) & 0xff) >> (8 - ldm.var.blue.length);

	u16 disp_color = (red << ldm.var.red.offset) | (green << ldm.var.green.offset) | (blue << ldm.var.blue.offset);

	*(u16*)(info->addr + y * info->fix.line_length + x * info->pixel_size) = disp_color;
}

/* 画一个矩形，利用draw_pixel方法屏蔽底层屏幕参数的细节
 * @info: 屏幕主对象
 * @x, y: 矩形左上角坐标
 * @xlen, ylen: 矩形边长
 * @color: 矩形颜色，RGB888格式
 */
static void draw_rect(struct ldm_info *info, size_t x, size_t y, size_t xlen, size_t ylen, u32 color)
{
	size_t ix, iy;
	for (iy = y; iy < y + ylen; ++iy) {
		for (ix = x; ix < x + xlen; ++ix) {
			info->draw_pixel(info, ix, iy, color);
		}
	}
}

static inline void clear_screen(struct ldm_info *info, u32 color)
{
	draw_rect(info, 0, 0, info->x - 1, info->y - 1, color);
}

static int disp_bmp(struct ldm_info *info, const char *bmp_name)
{
	FILE *fp = fopen(bmp_name, "r");
	if (!fp) {
		perror("fopen bmp");
		return -1;
	}

	//读取bmp的文件头
	struct bmp_head head = {0};
	fread(&head, sizeof(head), 1, fp);

	printf("bmp: width=%u, height=%u, bpp=%u\n", head.info.width, head.info.height, head.info.count);

	//将文件指针定位到像素数据的入口位置
	//fseek(fp, head.file.offset, SEEK_SET);

	//以图片和屏幕分辨率之间，较小的值为显示分辨率
	size_t dispx = info->x < head.info.width ? info->x : head.info.width;
	size_t dispy = info->y < head.info.height ? info->y : head.info.height;
	printf("dispx=%u, dispy=%u\n", dispx, dispy);

	u32 color = 0; //单个像素缓冲区
	size_t ix, iy; //显存的当前写入坐标
	size_t bmp_cur_line_offset = 0; //bmp文件当前读取的行，距离文件头部的偏移量
	for (iy = 0; iy < dispy; ++iy) {
		//bmp图片一行的字节数将会自动补齐到4的倍数，所以统计bmp一行字节数时，必须也补齐到4的倍数，所以最后有+3再& ~3的操作
		bmp_cur_line_offset = head.file.offset + (head.info.height - 1 - iy) * (((head.info.width) * (head.info.count / 8) + 3) & ~3);
		fseek(fp, bmp_cur_line_offset, SEEK_SET);
		for (ix = 0; ix < dispx; ++ix) {
			//读取一个像素
			fread(&color, head.info.count / 8, 1, fp);
			switch (head.info.count) {
			case 16: //RGB565
				//将图片中读出的一个RGB565格式的像素调整成RGB888写入显存
				color = RGB888(color >> 11 << 3, (color >> 5 << 2) & 0xff, (color << 3) & 0xff);
				info->draw_pixel(info, ix, iy, color);
				break;
			case 24:
				//bmp的24位格式，每个像素只有3个字节
				info->draw_pixel(info, ix, iy, color);
				color = 0;
				break;
			case 32:
				//bmp的32位像素中的24位有效数据是高位对齐的
				info->draw_pixel(info, ix, iy, color >> 8);
				break;
			}
		}
	}

	fclose(fp);

	return 0;
}

/* 在屏幕上画出一个沿着屏幕边缘顺时针移动的矩形，函数无限循环，利用双缓冲来消除矩形的闪烁
 * @info: 所操作的window对象，包含该window的一切属性，其中start_addr成员就是draw_pixel操作的显存的基地址
 * @xlen, ylen: 矩形的x和y的边长
 * @xstep, ystep: 矩形每次移动的像素数
 * @front_color: 前景色，也就是矩形的颜色，RGB888格式
 * @back_color: 背景色，RGB888格式
 */
static int doublebuffer_test(struct ldm_info *info, size_t xlen, size_t ylen, size_t xstep, size_t ystep, u32 front_color, u32 back_color)
{
	printf("%s\n", __FUNCTION__);

	//检查驱动是否支持双缓冲
	if (!info->fix.ypanstep) {
		printf("lcd driver dont support doublebuffer\n");
		return -1;
	}

	//保存原始显存首地址，作为pandiplay计算yoffset的基地址
	u32 fb_base_addr = (u32)info->addr;

	//前景，正显示在window屏幕上的显存首地址
	u32 front_buff = (u32)info->addr;
	//背景，隐藏的不显示的显存，用于存放下一帧画面的像素数据，渲染完下一帧画面之后，将切换到前景去显示。与front_buff交替着指向不同的显存地址
	u32 back_buff = (u32)info->addr + info->fix.smem_len / 2;
	u32 tmp_buff = 0; //用于交换前景和背景显存地址的临时变量

	//清屏
	clear_screen(info, back_color); //清除当前显存
	//清除背景
	info->addr = (u8*)back_buff;
	clear_screen(info, back_color);

	//循环变量，记录矩形当前的左上角坐标
	ssize_t x = 0;
	ssize_t y = 0;
	//上一帧的矩形左上角坐标
	ssize_t xprev = 0;
	ssize_t yprev = 0;
	//矩形每次循环在两个方向上移动的像素数
	ssize_t ixstep = 0;
	ssize_t iystep = 0;

	for (; ; x += ixstep, y += iystep) {
		//在后台显存上画出下一帧矩形
		draw_rect(info, x, y, xlen, ylen, front_color);

		//交换前景和背景显存
		tmp_buff = front_buff;
		front_buff = back_buff;
		back_buff = tmp_buff;

		//change_buff(info, front_buff);
		info->var.yoffset = (front_buff - fb_base_addr) / info->fix.line_length;
		int ret = ioctl(info->fd, FBIOPAN_DISPLAY, &info->var);
		//if (ioctl(info->fd, FBIOPAN_DISPLAY, &info->var) == -1) {
		if (ret == -1) {
			printf("ioctl return %d\n", ret);
			perror("pandisplay");
			return -1;
		}
		info->addr = (u8*)back_buff;

		//擦除上一帧的矩形，上一帧矩形在后台
		draw_rect(info, xprev, yprev, xlen, ylen, back_color);

		xprev = x;
		yprev = y;

		//调整矩形两个方向上移动循环变量x和y的下一帧的坐标值
		//左上角
		if ((x == 0) && (y <= 0)) {
			ixstep = xstep;
			iystep = 0;
			y = 0;
		} else if ((x + xlen >= info->x - 1) && (y == 0)) { //右上角
			ixstep = 0;
			iystep = ystep;
			x = info->x - 1 - xlen;
		} else if ((x + xlen == info->x - 1) && (y + ylen >= info->y - 1) ) { //右下角
			ixstep = -xstep;
			iystep = 0;
			y = info->y - 1 - ylen;
		} else if ((x <= 0) && (y + ylen == info->y - 1)) { //左下角
			ixstep = 0;
			iystep = -ystep;
			x = 0;
		}
	}

	return 0;
}

int main(int argc, char **argv)
{
	//1 打开设备
	ldm.fd = open(DEVNAME, O_RDWR);
	if (ldm.fd < 0) {
		perror("open device");
		goto err_open;
	}

	//2 获取var和fix两个对象
	if (ioctl(ldm.fd, FBIOGET_VSCREENINFO, &ldm.var) < 0) {
		perror("ioctl var");
		goto err_ioctl;
	}
	//var.
	//	xres, yres, xres_virtual, yres_virtual分辨率，virtual表示显存大小(可能比可见分辨率更大)，不带virtual的表示可见分辨率
	//	bits_per_pixel一个像素占多少bits
	//	red, green, blue三原色在像素数据中各占据哪些位
	printf("xres=%u, yres=%u, xres_v=%u, yres_v=%u\n", ldm.var.xres, ldm.var.yres, ldm.var.xres_virtual, ldm.var.yres_virtual);
	printf("bpp=%u\n", ldm.var.bits_per_pixel);
	printf("red: offset=%u, length=%u\n", ldm.var.red.offset, ldm.var.red.length);
	printf("green: offset=%u, length=%u\n", ldm.var.green.offset, ldm.var.green.length);
	printf("blue: offset=%u, length=%u\n", ldm.var.blue.offset, ldm.var.blue.length);

	ldm.pixel_size = ldm.var.bits_per_pixel / 8;

	if (ioctl(ldm.fd, FBIOGET_FSCREENINFO, &ldm.fix) < 0) {
		perror("ioctl fix");
		goto err_ioctl;
	}

	ldm.x = ldm.fix.line_length / ldm.pixel_size;
	ldm.y = ldm.var.yres;

	//fix.
	//	line_length 一行占据的实际字节数，包含可能存在的隐藏像素的大小
	//	smem_len屏幕实际占据的字节数，同样包含可能存在的隐藏像素大小
	printf("x=%u, y=%u\n", ldm.x, ldm.y);

	//3 计算出显存的真实分辨率，x方向的分辨率应该通过fix.line_length / (var.bits_per_pixel / 8)

	//4 根据获得的屏幕参数来挂接对应的画点函数
	switch (ldm.pixel_size) {
	case 2:
		ldm.draw_pixel = draw_pixel_16bit;
		break;
	case 4:
		ldm.draw_pixel = draw_pixel_32bit;
		break;
	default:
		fprintf(stderr, "unknown pixel_size %d\n", ldm.pixel_size);
		goto err_pixel_size;
	}

	//5 将内核中的显存内存映射到当前进程空间
	ldm.addr = mmap(NULL, ldm.fix.smem_len, PROT_WRITE, MAP_SHARED, ldm.fd, 0);
	if (ldm.addr == MAP_FAILED) {
		perror("mmap");
		goto err_mmap;
	}

	//6 画图
	switch (argc) {
	case 1:
		if (ldm.fix.ypanstep) {
			if (doublebuffer_test(&ldm, 100, 100, 1, 1, RGB888(255, 0, 0), RGB888(0, 0, 255)) < 0) {
				goto err_doublebuffer_test;
			}
		} else {
			clear_screen(&ldm, RGB888(0, 0, 255));
			draw_rect(&ldm, ldm.x / 5, ldm.y / 5, ldm.x - ldm.x / 3, ldm.y - ldm.y / 3, RGB888(255, 0, 0));
		}
		break;
	case 2:
		disp_bmp(&ldm, argv[1]);
		break;
	default:
		fprintf(stderr, "Usage: cmd [bmp_file_name]\n");
		break;
	}

	pause();

	munmap(ldm.addr, ldm.fix.smem_len);

	close(ldm.fd);
	return 0;

err_doublebuffer_test:
	munmap(ldm.addr, ldm.fix.smem_len);
err_mmap:
err_pixel_size:
err_ioctl:
	close(ldm.fd);
err_open:
	return -1;
}