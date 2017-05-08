#include "config.h"

#define CLK_SRC_LCD0 (*(volatile u32*)0x1003c234)
#define CLK_DIV_LCD (*(volatile u32*)0x1003c534)
#define LCDBLK_CFG (*(volatile u32*)0x10010210)

#define GPF0CON (*(volatile u32*)0x11400180)
#define GPF1CON (*(volatile u32*)0x114001a0)
#define GPF2CON (*(volatile u32*)0x114001c0)
#define GPF3CON (*(volatile u32*)0x114001e0)

#define LCD_BASE 0x11C00000
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

#define RGB888(r, g, b) ( (r) << 16 | (g) << 8 | (b))

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

	u32 start_addr; //显存起始地址
	u8 alpha_val; //4位alpha值,数值越小，透明度越高

	/* @x, y: 画出的对应像素点的坐标
	 * @color: RGB888格式的颜色值
	 */
	void (*draw_pixel)(struct win_info *, size_t x, size_t y, u32 color);
};

/* 用16位色画一个像素
 * @info: 目标window对象
 * @x, y: 目标像素的坐标
 * @color: 标准RGB888格式的颜色值，在函数内部会将该颜色值转换成RGB565
 */
static void draw_pixel_565(struct win_info *info, size_t x, size_t y, u32 color)
{
	//调整RGB888格式的颜色到当前屏幕的色深格式
	u8 red = ((color >> 16) & 0xff) >> (8 - 5);
	u8 green = ((color >> 8) & 0xff) >> (8 - 6);
	u8 blue = ((color) & 0xff) >> (8 - 5);

	*((u16*)info->start_addr + y * XSIZE + x) = red << 11 | green << 5 | blue;
}

static void draw_pixel_pal8(struct win_info *info, size_t x, size_t y, u32 pal_offset)
{
	*((u8*)info->start_addr + y * XSIZE + x) = (u8)pal_offset;
}

/* 画一个矩形，利用draw_pixel方法屏蔽底层屏幕参数的细节
 * @info: 屏幕主对象
 * @x, y: 矩形左上角坐标
 * @xlen, ylen: 矩形边长
 * @color: 矩形颜色，RGB888格式
 */
static void draw_rect(struct win_info *info, size_t x, size_t y, size_t xlen, size_t ylen, u32 color)
{
	size_t ix, iy;
	for (iy = y; iy < y + ylen; ++iy) {
		for (ix = x; ix < x + xlen; ++ix) {
			info->draw_pixel(info, ix, iy, color);
		}
	}
}

static inline void clear_screen(struct win_info *info, u32 color)
{
	draw_rect(info, 0, 0, XSIZE - 1, YSIZE - 1, color);
}

static struct win_info win[] = {
	[0] = {
		.bpp = RGB565_BPP_MODE,
		.pixel_size = 2,
		.swap_mode = RGB565_SWAP_MODE,
		.start_addr = 0x50000000,
		.draw_pixel = draw_pixel_565,
	},
	[1] = {
		.bpp = PAL8_BPP_MODE,
		.pixel_size = 1,
		.swap_mode = PAL8_SWAP_MODE,
		.start_addr = 0x50100000,
		.draw_pixel = draw_pixel_pal8,
		.alpha_val = 5,
	},
};

static void lcd_on()
{
	//开启对应window
	WINCON0 |= 1;
	WINCON1 |= 1;

	//打开lcd控制器
	VIDCON0 |= 0x3;
}

static void lcd_off()
{
	//关闭所有window
	WINCON0 &= ~1;
	WINCON1 &= ~1;
	WINCON2 &= ~1;
	WINCON3 &= ~1;
	WINCON4 &= ~1;

	//关闭lcd控制器
	VIDCON0 &= ~0x3;
}

static void lcd_init()
{
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
	WINCON0 = win[0].bpp << 2 | win[0].swap_mode << 15;

	//指定window显示区域的坐标范围
	VIDOSD0A = 0; //显示区域左上角坐标
	VIDOSD0B = (YSIZE - 1) | (XSIZE - 1) << 11; //右下角
	//指定的显示区域的大小, 注!意!单!位!
	VIDOSD0C = XSIZE * YSIZE * win[0].pixel_size / 4;

	//指定显存的首末地址
	//W00表示windows0，ADD0表示起始地址，ADD1表示显存终止地址，B0中的0表示buf0的显存首地址
	VIDW00ADD0B0 = win[0].start_addr;
	//显存终止地址,如果需要支持横向滚动的话，终止地址中将包含offsize的大小
	VIDW00ADD1B0 = win[0].start_addr + XSIZE * YSIZE * win[0].pixel_size;
	//手册P1813，有关横向移动显示区域相关，OFFSIZE指允许的横向移动范围, PAGEWIDTH指X分辨率。一般不用支持横向移动。如果不打算支持横向移动，那么OFFSIZE_F = 0
	VIDW00ADD2 = XSIZE * win[0].pixel_size;

	//===================window1====================
	WINCON1 = win[1].bpp << 2 | win[1].swap_mode << 15;

	//指定window显示区域的坐标范围
	VIDOSD1A = 0; //显示区域左上角坐标
	VIDOSD1B = (YSIZE - 1) | (XSIZE - 1) << 11; //右下角
	//指定的显示区域的大小, 注!意!单!位!
	VIDOSD1D = XSIZE * YSIZE * win[1].pixel_size / 4;

	VIDW01ADD0B0 = win[1].start_addr;
	VIDW01ADD1B0 = win[1].start_addr + XSIZE * YSIZE * win[1].pixel_size;
	VIDW01ADD2 = XSIZE * win[1].pixel_size;

	//================颜色表================
	/* WPALCON_L和WPALCON_H：颜色表控制
	 * WPALCON_L[9]: 0-normal mode: 正常工作模式，此时window使用颜色表的内容来显示画面中各像素的颜色；1-update：初始化和填充颜色表的元素内容，此时不可开启对应的window
	 * WPALCON[5:3]: 颜色表元素的颜色格式
	 */
	//将颜色表设置为更新模式，以初始化并填充颜色表
	WPALCON_L |= 1 << 9;
	//window1的颜色表的颜色格式为RGB888, 24位色
	WPALCON_L = (WPALCON_L & ~(7 << 3)) | 0b001 << 3;

	//填充颜色表元素的内容，4412手册P1910，各window颜色表的存放地址
	//两个地址都可以访问颜色表元素，比如window1的颜色表0号元素既可以用0x11c02800来访问，也可以用0x11c00800来访问
	//色深设为颜色表对应位后，只需在显存中填入颜色表下标即可显示
	WIN1PAL(0) = RGB888(255, 0, 0);
	WIN1PAL(1) = RGB888(0, 255, 0);
	WIN1PAL(2) = RGB888(0, 0, 255);
	WIN1PAL(3) = RGB888(255, 255, 0);
	WIN1PAL(4) = RGB888(255, 255, 255);
	WIN1PAL(5) = RGB888(0, 0, 0);

	//将颜色表设为normal工作模式
	WPALCON_L &= ~(1 << 9);

	//===============alpha透明度===============
	/* WINCON1[6]: 0-整个window使用统一的透明度，需结合VIDOSD1C使用；1-每个像素需要单独设置透明度，需要支持alpha值的色深才行。有两种色深可以提供8位和4位alpha设置，其他可提供1位alpha值供2选1，需结合VIDOSD1C使用
	 * [1]: 如果[6]选择统一透明度，则该位用于选择VIDOSD1C里所设置的2个透明度中的一个。如果[6]选择每个像素单独设alpha值，那么该位必须和之前选择的色深吻合。即如果选择1位alpha的色深，那么该位必须是0；如果选择多位alpha的色深，则该位必须是1
	 */
	//当前WINCON1[6]和[1]都是0，即window统一透明度模式并且选择在VIDOSD1C寄存器中设置的0号alpha值

	/* VIDOSD1C设置2个透明度，结合之前的WINCON相关设定来使用 */
	VIDOSD1C = win[1].alpha_val << 12 | win[1].alpha_val << 16 | win[1].alpha_val << 20;

	//=====================================

	//打开lcd控制器
	lcd_on();

	//测试
	//win0
	clear_screen(win, RGB888(0, 0, 255));
	draw_rect(win, 10, 10, 400, 200, RGB888(255, 0, 0));

	//win1
	clear_screen(win + 1, 3);
	draw_rect(win + 1, 100, 100, 400, 200, 4);
}

module_init(lcd_init);