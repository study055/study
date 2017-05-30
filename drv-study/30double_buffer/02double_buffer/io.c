#include "config.h"

#if 1
#define GPM4CON (*(volatile unsigned int*)0x110002e0)
#define GPM4DAT (*(volatile unsigned char*)0x110002e4)

#else

#define GPM4CON (*(volatile u32*)0x210002e0)
#define GPM4DAT (*(volatile u8*)0x210002e4)

#endif

#define GPX3CON (*(volatile u32*)0x11000c60)
#define EXT_INT43CON (*(volatile u32*)0x11000e0c)
#define EXT_INT43_MASK (*(volatile u8*)0x11000f0c)
#define EXT_INT43_PEND (*(volatile u8*)0x11000f4c)

static void delay(size_t n)
{
	while (n--) {}
}

void led_init()
{
	GPM4CON = (GPM4CON & ~0xffff) | 1 | 1 << 4 | 1 << 8 | 1 << 12;
}

static void led_on(u8 n)
{
	GPM4DAT = (GPM4DAT & ~0xf) | (n & 0xf);
}

static void led_test()
{
	while (1) {
		led_on(0b1010);
		delay(0xfffff);
		led_on(0b0101);
		delay(0xfffff);
	}
}

static void key_handler()
{
	printk("key %u triggered\n", (EXT_INT43_PEND >> 2) & 0xf);

	EXT_INT43_PEND |= 0xf << 2;
}

static void key_init()
{
	printk("%s\n", __FUNCTION__);

	//GPX3_2-5设为中断
	GPX3CON |= 0xffff << 8;
	//下降沿触发
	EXT_INT43CON = (EXT_INT43CON & ~(0xffff << 8)) | 0x2222 << 8;
	//EXT_INT43_FLTCON0
	//EXT_INT43_FLTCON1
	//清除中断标志，写1清0
	EXT_INT43_PEND |= 0xf << 2;
	//打开中断
	EXT_INT43_MASK &= ~(0xf << 2);

	request_irq(64, key_handler);
}


module_init(led_init);
module_init(key_init);
//module_init(led_test);