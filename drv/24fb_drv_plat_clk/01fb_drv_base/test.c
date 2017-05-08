#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fb.h>
#include <linux/dma-mapping.h> //dma_alloc_writecombine
#include <mach/regs-clock.h> //EXYNOS4_CLKGATE_IP_LCD0
#include <mach/regs-gpio.h>
#include <linux/io.h> //ioremap

//加载驱动前必须先去除内核中自带的framebuffer驱动
//make menuconfig -> Device Drivers -> Graphics support -> Samsung S3C framebuffer support，按空格去掉选项前面的*
//去掉该选项后，drivers/video/Kconfig中，对应选项关闭后会造成FB_S3C_NR_BUFFERS也被关闭，而该宏在内核板级支持文件mach-tiny4412.c中被使用，该宏的消失将造成该板级文件无法编译，所以需要想办法保持该宏的存在

//因为内核中的framebuffer console依赖fb驱动，如果开启的话，将会在内核启动过程中段错误，所以需要将fb console功能关闭
//make menuconfig -> Device Drivers -> Graphics support -> Console display driver support -> Framebuffer Console support，关闭

#define CLKGATE_IP_LCD0 (*(volatile u32*)EXYNOS4_CLKGATE_IP_LCD0)

#define CLK_SRC_LCD0 (*(volatile u32*)EXYNOS4_CLKSRC_LCD0)
#define CLK_DIV_LCD (*(volatile u32*)EXYNOS4_CLKDIV_LCD0)
#define LCDBLK_CFG (*(volatile u32*)(ldm->lcdblk_reg))

#define GPF0CON (*(volatile u32*)(S5P_VA_GPIO + 0x0180))
#define GPF1CON (*(volatile u32*)(S5P_VA_GPIO + 0x01a0))
#define GPF2CON (*(volatile u32*)(S5P_VA_GPIO + 0x01c0))
#define GPF3CON (*(volatile u32*)(S5P_VA_GPIO + 0x01e0))

#define LCD_BASE (ldm->reg)
#define VIDCON0 	(*(volatile u32 *)(LCD_BASE + 0x0000))
#define VIDCON1 	(*(volatile u32 *)(LCD_BASE + 0x0004))
#define VIDCON2 	(*(volatile u32 *)(LCD_BASE + 0x0008))
#define VIDCON3 	(*(volatile u32 *)(LCD_BASE + 0x000C))
#define VIDTCON0 	(*(volatile u32 *)(LCD_BASE + 0x0010))
#define VIDTCON1 	(*(volatile u32 *)(LCD_BASE + 0x0014))
#define VIDTCON2 	(*(volatile u32 *)(LCD_BASE + 0x0018))
#define VIDTCON3 	(*(volatile u32 *)(LCD_BASE + 0x001C))
#define WINCON0 	(*(volatile u32 *)(LCD_BASE + 0x0020))
#define WINCON1 	(*(volatile u32 *)(LCD_BASE + 0x0024))
#define WINCON2 	(*(volatile u32 *)(LCD_BASE + 0x0028))
#define WINCON3 	(*(volatile u32 *)(LCD_BASE + 0x002C))
#define WINCON4 	(*(volatile u32 *)(LCD_BASE + 0x0030))
#define SHADOWCON 	(*(volatile u32 *)(LCD_BASE + 0x0034))
#define WINCHMAP2 	(*(volatile u32 *)(LCD_BASE + 0x003C))
#define VIDOSD0A 	(*(volatile u32 *)(LCD_BASE + 0x0040))
#define VIDOSD0B 	(*(volatile u32 *)(LCD_BASE + 0x0044))
#define VIDOSD0C 	(*(volatile u32 *)(LCD_BASE + 0x0048))
#define VIDOSD1A 	(*(volatile u32 *)(LCD_BASE + 0x0050))
#define VIDOSD1B 	(*(volatile u32 *)(LCD_BASE + 0x0054))
#define VIDOSD1C 	(*(volatile u32 *)(LCD_BASE + 0x0058))
#define VIDOSD1D 	(*(volatile u32 *)(LCD_BASE + 0x005C))
#define VIDOSD2A 	(*(volatile u32 *)(LCD_BASE + 0x0060))
#define VIDOSD2B 	(*(volatile u32 *)(LCD_BASE + 0x0064))
#define VIDOSD2C 	(*(volatile u32 *)(LCD_BASE + 0x0068))
#define VIDOSD2D 	(*(volatile u32 *)(LCD_BASE + 0x006C))
#define VIDOSD3A 	(*(volatile u32 *)(LCD_BASE + 0x0070))
#define VIDOSD3B 	(*(volatile u32 *)(LCD_BASE + 0x0074))
#define VIDOSD3C 	(*(volatile u32 *)(LCD_BASE + 0x0078))
#define VIDOSD4A 	(*(volatile u32 *)(LCD_BASE + 0x0080))
#define VIDOSD4B 	(*(volatile u32 *)(LCD_BASE + 0x0084))
#define VIDOSD4C 	(*(volatile u32 *)(LCD_BASE + 0x0088))
#define VIDW00ADD0B0 	(*(volatile u32 *)(LCD_BASE + 0x00A0))
#define VIDW00ADD0B1 	(*(volatile u32 *)(LCD_BASE + 0x00A4))
#define VIDW00ADD0B2 	(*(volatile u32 *)(LCD_BASE + 0x20A0))
#define VIDW01ADD0B0 	(*(volatile u32 *)(LCD_BASE + 0x00A8))
#define VIDW01ADD0B1 	(*(volatile u32 *)(LCD_BASE + 0x00AC))
#define VIDW01ADD0B2 	(*(volatile u32 *)(LCD_BASE + 0x20A8))
#define VIDW02ADD0B0 	(*(volatile u32 *)(LCD_BASE + 0x00B0))
#define VIDW02ADD0B1 	(*(volatile u32 *)(LCD_BASE + 0x00B4))
#define VIDW02ADD0B2 	(*(volatile u32 *)(LCD_BASE + 0x20B0))
#define VIDW03ADD0B0 	(*(volatile u32 *)(LCD_BASE + 0x00B8))
#define VIDW03ADD0B1 	(*(volatile u32 *)(LCD_BASE + 0x00BC))
#define VIDW03ADD0B2 	(*(volatile u32 *)(LCD_BASE + 0x20B8))
#define VIDW04ADD0B0 	(*(volatile u32 *)(LCD_BASE + 0x00C0))
#define VIDW04ADD0B1 	(*(volatile u32 *)(LCD_BASE + 0x00C4))
#define VIDW04ADD0B2 	(*(volatile u32 *)(LCD_BASE + 0x20C0))
#define VIDW00ADD1B0 	(*(volatile u32 *)(LCD_BASE + 0x00D0))
#define VIDW00ADD1B1 	(*(volatile u32 *)(LCD_BASE + 0x00D4))
#define VIDW00ADD1B2 	(*(volatile u32 *)(LCD_BASE + 0x20D0))
#define VIDW01ADD1B0 	(*(volatile u32 *)(LCD_BASE + 0x00D8))
#define VIDW01ADD1B1 	(*(volatile u32 *)(LCD_BASE + 0x00DC))
#define VIDW01ADD1B2 	(*(volatile u32 *)(LCD_BASE + 0x20D8))
#define VIDW02ADD1B0 	(*(volatile u32 *)(LCD_BASE + 0x00E0))
#define VIDW02ADD1B1 	(*(volatile u32 *)(LCD_BASE + 0x00E4))
#define VIDW02ADD1B2 	(*(volatile u32 *)(LCD_BASE + 0x20E0))
#define VIDW03ADD1B0 	(*(volatile u32 *)(LCD_BASE + 0x00E8))
#define VIDW03ADD1B1 	(*(volatile u32 *)(LCD_BASE + 0x00EC))
#define VIDW03ADD1B2 	(*(volatile u32 *)(LCD_BASE + 0x20E8))
#define VIDW04ADD1B0 	(*(volatile u32 *)(LCD_BASE + 0x00F0))
#define VIDW04ADD1B1 	(*(volatile u32 *)(LCD_BASE + 0x00F4))
#define VIDW04ADD1B2 	(*(volatile u32 *)(LCD_BASE + 0x20F0))
#define VIDW00ADD2 	(*(volatile u32 *)(LCD_BASE + 0x0100))
#define VIDW01ADD2 	(*(volatile u32 *)(LCD_BASE + 0x0104))
#define VIDW02ADD2 	(*(volatile u32 *)(LCD_BASE + 0x0108))
#define VIDW03ADD2 	(*(volatile u32 *)(LCD_BASE + 0x010C))
#define VIDW04ADD2 	(*(volatile u32 *)(LCD_BASE + 0x0110))
#define VIDINTCON0 	(*(volatile u32 *)(LCD_BASE + 0x0130))
#define VIDINTCON1 	(*(volatile u32 *)(LCD_BASE + 0x0134))
#define W1KEYCON0 	(*(volatile u32 *)(LCD_BASE + 0x0140))
#define VIDW0ALPHA0 	(*(volatile u32 *)(LCD_BASE + 0x021C))
#define VIDW0ALPHA1 	(*(volatile u32 *)(LCD_BASE + 0x0220))
#define VIDW1ALPHA0 	(*(volatile u32 *)(LCD_BASE + 0x0224))
#define VIDW1ALPHA1 	(*(volatile u32 *)(LCD_BASE + 0x0228))
#define VIDW2ALPHA0 	(*(volatile u32 *)(LCD_BASE + 0x022C))
#define VIDW2ALPHA1 	(*(volatile u32 *)(LCD_BASE + 0x0230))
#define VIDW3ALPHA0 	(*(volatile u32 *)(LCD_BASE + 0x0234))
#define VIDW3ALPHA1 	(*(volatile u32 *)(LCD_BASE + 0x0238))
#define VIDW4ALPHA0 	(*(volatile u32 *)(LCD_BASE + 0x023C))
#define VIDW4ALPHA1 	(*(volatile u32 *)(LCD_BASE + 0x0240))

#define WPALCON_L 	(*(volatile u32 *)(LCD_BASE + 0x01a0))
#define WIN0PAL(n) 	(*(volatile u32 *)(LCD_BASE + 0x2400 + (n) * 4))
#define WIN1PAL(n) 	(*(volatile u32 *)(LCD_BASE + 0x2800 + (n) * 4))
#define WIN2PAL(n) 	(*(volatile u32 *)(LCD_BASE + 0x2c00 + (n) * 4))
#define WIN3PAL(n) 	(*(volatile u32 *)(LCD_BASE + 0x3000 + (n) * 4))
#define WIN4PAL(n) 	(*(volatile u32 *)(LCD_BASE + 0x3400 + (n) * 4))

enum {
	//分辨率
	XSIZE = 800,
	YSIZE = 480,

	LCD_SCLK_SRC = 800000000, //时钟源800MHZ，取自MPLL
	//lcd控制器时序图P1816，lcd屏手册时序图P13
	//以下参数查阅lcd屏的手册
	VCLK = 33300000, //像素时钟，该时钟每个周期传输一个像素的24bit数据, 33.3MHZ

	//极性, 0-normal极性相同,1-inverted极性相反
	IVCLK = 1, //0-下降沿，1-上升沿
	IHSYNC = 1,
	IVSYNC = 1,
	IVDEN = 0,

	//时序
	VBPD = 13,
	VFPD = 22,
	VSPW = 10,
	HBPD = 26,
	HFPD = 210,
	HSPW = 20,
};

struct win_info {

#define RGB565_BPP_MODE 0b0101
#define RGB888_BPP_MODE 0b1011
#define PAL8_BPP_MODE 0b0011
	u8 bpp;

	size_t pixel_size; //每个像素占用的字节数

#define RGB565_SWAP_MODE 0b0010
#define PAL8_SWAP_MODE 0b0100
	u8 swap_mode; //相邻的几个像素的排列顺序,看手册, 如果是RGB565，看P1779,如果是RGB888，P1774

	u8 alpha_val; //4位alpha值,数值越小，透明度越高
};

static struct win_info win = {
	.bpp = RGB565_BPP_MODE,
	.pixel_size = 2,
	.swap_mode = RGB565_SWAP_MODE,
};

//私有数据空间类型
struct ldm_info {
	struct fb_ops ops;
	u32 reg; //lcd控制器寄存器群的基地址虚拟地址
	u32 lcdblk_reg;
};

static struct fb_info *fb;

static void lcd_on(void)
{
	struct ldm_info *ldm = (struct ldm_info*)fb->par;

	//开启对应window
	WINCON0 |= 1;

	//打开lcd控制器
	VIDCON0 |= 0x3;
}

static void lcd_off(void)
{
	struct ldm_info *ldm = (struct ldm_info*)fb->par;

	//关闭所有window
	WINCON0 &= ~1;
	WINCON1 &= ~1;
	WINCON2 &= ~1;
	WINCON3 &= ~1;
	WINCON4 &= ~1;

	//关闭lcd控制器
	VIDCON0 &= ~0x3;
}

static void hardware_init(void)
{
	struct ldm_info *ldm = (struct ldm_info*)fb->par;

	//1 GPIO功能转换lcd控制器的功能管脚
	GPF0CON = 0x22222222;
	GPF1CON = 0x22222222;
	GPF2CON = 0x22222222;
	GPF3CON = (GPF3CON & ~0xffff) | 0x2222;

	//2 配置输入时钟sclk, 手册P457右上角图
	//选择MPLL输出做时钟源,800MHZ
	CLK_SRC_LCD0 = (CLK_SRC_LCD0 & ~0xf) | 0b0110;
	//选择FIMD的输入时钟源分频值,不分频
	CLK_DIV_LCD &= ~0xf;

	//3 配置lcd系统时钟由FIMD来使用, LCDBLK_CFG[1], 手册P884
	ldm->lcdblk_reg = (u32)ioremap(0x10010210, 4);
	LCDBLK_CFG |= 1 << 1;

	//4 lcd控制器初始化
	//关闭lcd控制器和所有window，在初始化完毕后再打开
	lcd_off();

	/* VIDCON0[1:0]: lcd控制器开关，基本上都是同时置1或清0的
	 * [5]: 像素时钟VCLK的开关是否受lcd控制器开关的影响，无所谓
	 * [13:6]: 分频值，LCD_SCLK_SRC / (CLKVAL+1) = VCLK, VCLK的值从lcd屏的手册上查找
	 * [16]: 如果改变了分频值，那么新的分频值什么时候生效，无所谓
	 * [18]: RGB总线的接法是串行还是并行
	 * [25:20]: i80相关，么用
	 * [28:26]: 总线类型，RGB interface
	 */
	VIDCON0 = (LCD_SCLK_SRC / VCLK - 1) << 6;

	/* VIDCON1[7:4]： 主要是时序管脚的极性设置，lcd控制器的管脚极性必须和lcd屏的参数接近或吻合，才能匹配, lcd控制器时序图P1816，lcd屏手册时序图P13
	 * [14:13]: 可读出当前时刻在时序的哪一个位置,可用于轮询式的双缓冲
	 */
	VIDCON1 = IVDEN << 4 | IVSYNC << 5 | IHSYNC << 6 | IVCLK << 7;

	//设置时序，注意单位。HSPW， HFPD, HBPD, VSPW, VFPD, VBPD
	//lcd控制器时序图P1816，lcd屏手册时序图P13
	VIDTCON0 = VSPW | VFPD << 8 | VBPD << 16;
	VIDTCON1 = HSPW | HFPD << 8 | HBPD << 16;
	//分辨率，从时序图上看
	VIDTCON2 = (XSIZE - 1) | (YSIZE - 1) << 11;

	//打开chanel0，即window0的数据通道，需要打开其他数据通道所绑定的chanel和window时，设置相应位即可。chanel和window之间的连接关系由， WINCHMAP2,只负责连接关系的确认。Local Path需要关闭，和dma冲突
	SHADOWCON = 0x1f;

	//=================window0==============
	//初始化window，任选一个window，注意window0作为最底下的window，不能设置透明度
	/* WINCON0[0]: window使能开关
	 * [1][6]: window0不能设alpha，所以无效
	 * [5:2]: 色深
	 * [10:9]: dma的burst模式，连续传输数据，设最大值
	 * [13]: 色彩空间选择,RGB
	 * [14][21:19][31:30]: 多重显存指定机制，无所谓，一般不使用
	 * [18:15]:swap相关，看手册, 如果是RGB565，看P1779,如果是RGB888，P1774
	 * [22]: dma
	 */
	WINCON0 = win.bpp << 2 | win.swap_mode << 15;

	//指定window显示区域的坐标范围
	VIDOSD0A = 0; //显示区域左上角坐标
	VIDOSD0B = (YSIZE - 1) | (XSIZE - 1) << 11; //右下角
	//指定的显示区域的大小, 注!意!单!位!
	VIDOSD0C = XSIZE * YSIZE * win.pixel_size / 4;

	//指定显存的首末地址
	//W00表示windows0，ADD0表示起始地址，ADD1表示显存终止地址，B0中的0表示buf0的显存首地址
	VIDW00ADD0B0 = fb->fix.smem_start;
	//显存终止地址,如果需要支持横向滚动的话，终止地址中将包含offsize的大小
	VIDW00ADD1B0 = fb->fix.smem_start + XSIZE * YSIZE * win.pixel_size;
	//手册P1813，有关横向移动显示区域相关，OFFSIZE指允许的横向移动范围, PAGEWIDTH指X分辨率。一般不用支持横向移动。如果不打算支持横向移动，那么OFFSIZE_F = 0
	VIDW00ADD2 = XSIZE * win.pixel_size;


	//打开lcd控制器
	lcd_on();
}

static int __init ldm_init(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	int ret = 0;

	//1 创建fb_info对象和私有数据空间
	fb = framebuffer_alloc(sizeof(struct ldm_info), NULL);
	if (!fb) {
		printk(KERN_WARNING "framebuffer_alloc failed\n");
		ret = -ENOMEM;
		goto err_framebuffer_alloc;
	}

	struct ldm_info *ldm = (struct ldm_info*)fb->par;

	//2 填充对象成员
	fb->var.xres = XSIZE;
	fb->var.yres = YSIZE;
	fb->var.xres_virtual = XSIZE;
	fb->var.yres_virtual = YSIZE;
	fb->var.bits_per_pixel = win.pixel_size * 8;
	fb->var.red.offset = 11;
	fb->var.red.length = 5;
	fb->var.green.offset = 5;
	fb->var.green.length = 6;
	fb->var.blue.offset = 0;
	fb->var.blue.length = 5;

	fb->fix.smem_len = XSIZE * YSIZE * win.pixel_size;
	fb->fix.line_length = XSIZE * win.pixel_size;

	fb->screen_size = fb->fix.smem_len;

	//3 创建显存。为了保证cpu和dma在同时访问显存时的同步问题，需要特殊的cpu缓冲机制，使用dma_alloc_writecombine来创建显存
	//参数dma_handle将返回显存的物理地址， 函数返回值是虚拟地址
	fb->screen_base = dma_alloc_writecombine(NULL, fb->screen_size, (dma_addr_t*)&fb->fix.smem_start, GFP_KERNEL);
	if (!fb->screen_base) {
		printk(KERN_WARNING "dma_alloc_writecombine failed\n");
		ret = -ENOMEM;
		goto err_dma_alloc_writecombine;
	}

	//4 挂接操作方法
	fb->fbops = &ldm->ops;
	//下面三个方法已经在内核中实现，分别在内核源码中的drivers/video中的cfbcopyarea.c，cfbfillrect.c， cfbimgblt.c
	//但是三个函数所属的.c文件需要被编译链接进内核，我们才能访问这三个全局函数
	//内核中自带的framebuffer驱动一旦被去除后，默认将不会编译链接这3个文件，所以我们需要重新确保3个文件被编译链接
	fb->fbops->fb_fillrect = cfb_fillrect;
	fb->fbops->fb_copyarea = cfb_copyarea;
	fb->fbops->fb_imageblit = cfb_imageblit;

	//5 硬件的初始化
	//5.1 总线时钟的使能
	CLKGATE_IP_LCD0 |= 1;
	//5.2 为lcd控制器的初始化映射寄存器
	ldm->reg = (u32)ioremap(0x11C00000, 0x8000);
	//5.3 硬件初始化
	hardware_init();

	//6 提交注册fb_info
	ret = register_framebuffer(fb);
	if (ret < 0) {
		printk(KERN_WARNING "register_framebuffer failed\n");
		goto err_register_framebuffer;
	}

	return 0;

err_register_framebuffer:
	dma_free_writecombine(NULL, fb->screen_size, fb->screen_base, (dma_addr_t)fb->fix.smem_start);
err_dma_alloc_writecombine:
	framebuffer_release(fb);
err_framebuffer_alloc:
	return ret;
}

static void __exit ldm_exit(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	unregister_framebuffer(fb);
	dma_free_writecombine(NULL, fb->screen_size, fb->screen_base, (dma_addr_t)fb->fix.smem_start);
	framebuffer_release(fb);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");