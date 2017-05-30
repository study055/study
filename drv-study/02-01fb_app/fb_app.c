#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <linux/fb.h> // /usr/include/linux/fb.h

typedef unsigned long u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef signed long s32;
typedef signed short s16;
typedef signed char s8;

#define DEVNAME "/dev/fb0"

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

#define RGB888(r, g, b) ( (r) << 16 | (g) << 8 | (b))

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
static void draw_square(struct ldm_info *info, size_t x, size_t y, size_t xlen, size_t ylen, u32 color)
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
	draw_square(info, 0, 0, info->x - 1, info->y - 1, color);
}

int main()
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
	ldm.y = ldm.fix.smem_len / ldm.x / ldm.pixel_size;

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
	clear_screen(&ldm, RGB888(0, 0, 255));
	draw_square(&ldm, 100, 100, 800, 600, RGB888(255, 0, 0));

	pause();

	munmap(ldm.addr, ldm.fix.smem_len);

	close(ldm.fd);
	return 0;

err_mmap:
err_pixel_size:
err_ioctl:
	close(ldm.fd);
err_open:
	return -1;
}