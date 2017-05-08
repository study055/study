#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fb.h>
#include <linux/dma-mapping.h> //dma_alloc_writecombine
#include <mach/regs-clock.h> //EXYNOS4_CLKGATE_IP_LCD0
#include <linux/io.h> //ioremap

//加载驱动前必须先去除内核中自带的framebuffer驱动
//make menuconfig -> Device Drivers -> Graphics support -> Samsung S3C framebuffer support，按空格去掉选项前面的*
//去掉该选项后，drivers/video/Kconfig中，对应选项关闭后会造成FB_S3C_NR_BUFFERS也被关闭，而该宏在内核板级支持文件mach-tiny4412.c中被使用，该宏的消失将造成该板级文件无法编译，所以需要想办法保持该宏的存在

//因为内核中的framebuffer console依赖fb驱动，如果开启的话，将会在内核启动过程中段错误，所以需要将fb console功能关闭
//make menuconfig -> Device Drivers -> Graphics support -> Console display driver support -> Framebuffer Console support，关闭

//私有数据空间类型
struct ldm_info {
	struct fb_ops ops;
};

static struct fb_info *fb;

static int __init ldm_init(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	//1 创建fb_info对象和私有数据空间
	fb = framebuffer_alloc(sizeof(struct ldm_info), NULL);
	if (!fb) {

	}

	//2 填充对象成员
	fb->var.xres = ;
	fb->var.yres = ;
	fb->var.xres_virtual = ;
	fb->var.yres_virtual = ;
	fb->var.bits_per_pixel = ;
	fb->var.red
	fb->var.green
	fb->var.blue

	fb->fix.smem_len = ;
	fb->fix.line_length = ;

	fb->screen_size = ;

	//3 创建显存。为了保证cpu和dma在同时访问显存时的同步问题，需要特殊的cpu缓冲机制，使用dma_alloc_writecombine来创建显存
	//参数dma_handle将返回显存的物理地址， 函数返回值是虚拟地址
	fb->screen_base = dma_alloc_writecombine(NULL, fb->screen_size, (dma_addr_t*)&fb->fix.smem_start, );
	if (!fb->screen_base) {

	}

	//4 挂接操作方法
	fb->fbops =
	//下面三个方法已经在内核中实现，分别在内核源码中的drivers/video中的cfbcopyarea.c，cfbfillrect.c， cfbimgblt.c
	//但是三个函数所属的.c文件需要被编译链接进内核，我们才能访问这三个全局函数
	//内核中自带的framebuffer驱动一旦被去除后，默认将不会编译链接这3个文件，所以我们需要重新确保3个文件被编译链接
	fb->fbops->fb_fillrect = ;
	fb->fbops->fb_copyarea = cfb_copyarea;
	fb->fbops->fb_imageblit = ;

	//5 硬件的初始化
	//5.1 总线时钟的使能
	CLK_GATE_IP_LCD[0]
	//5.2 时钟初始化
	ioremap
	//5.2 lcd控制器的初始化
	ioremap

	//6 提交注册fb_info
	register_framebuffer();

	return 0;
}

static void __exit ldm_exit(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	unregister_framebuffer();
	dma_free_writecombine();
	framebuffer_release(fb);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");