#include "config.h"

#define WTCON (*(volatile u16*)0x10060000)
#define WTDAT (*(volatile u16*)0x10060004)
#define WTCNT (*(volatile u16*)0x10060008)
#define WTCLRINT (*(volatile u16*)0x1006000c)

enum {
	WDT_PCLK = 100000000, //100MHZ，见手册P446
	WDT_PRESCALER = 255,
	WDT_CLK_SEL = 3, //0-16;1-32;2-64;3-128
	//1秒钟计时所需的倒计时数
	WDT_HZ = WDT_PCLK / (WDT_PRESCALER + 1) / (16 << WDT_CLK_SEL),
};

static void wdt_handler()
{
	printk("%s\n", __FUNCTION__);

	//任意写操作清除中断标志
	WTCLRINT = 0;
}

static void wdt_init()
{
	WTCON = 1 << 2 | WDT_CLK_SEL << 3 | WDT_PRESCALER << 8;
	WTDAT = WDT_HZ * 2;
	WTCNT = WDT_HZ * 2;

	request_irq(75, wdt_handler);

	//打开watchdog
	WTCON |= 1 << 5;
}



#define GPD0CON (*(volatile u32*)0x114000a0)

#define TCFG0 (*(volatile u32*)0x139d0000)
#define TCFG1 (*(volatile u32*)0x139d0004)
#define TCON (*(volatile u32*)0x139d0008)
#define TCNTB0 (*(volatile u32*)0x139d000c)
#define TCMPB0 (*(volatile u32*)0x139d0010)
#define TCNTB4 (*(volatile u32*)0x139d003c)
#define TINT_CSTAT (*(volatile u32*)0x139d0044)

enum {
	PWM_PCLK = 100000000, //100MHZ，见手册P446
	PWM_PRESCALER0 = 99,
	PWM_PRESCALER1 = 99,

	PWM0_DIV = 3, //实际分频值是1 << PWM0_DIV
	PWM0_KHZ = PWM_PCLK / (PWM_PRESCALER0 + 1) / (1 << PWM0_DIV) / 1000,

	TIMER4_DIV = 3,
	TIMER4_HZ = PWM_PCLK / (PWM_PRESCALER1 + 1) / (1 << TIMER4_DIV),
};

static void prescaler_init()
{
	TCFG0 = (TCFG0 & ~0xffff) | PWM_PRESCALER0 | PWM_PRESCALER1 << 8;
}

static void pwm0_init()
{
	printk("%s\n", __FUNCTION__);
	//GPD0_0设为pwm输出,P136
	GPD0CON = (GPD0CON & ~0xf) | 0x2;

	//2级分频
	TCFG1 = (TCFG1 & ~0xf) | PWM0_DIV;

	TCNTB0 = PWM0_KHZ / 3; //3KHZ
	TCMPB0 = TCNTB0 / 50;

	/* TCON
	 * [0]: 对应定时器的开关
	 * [1]: 手动将TCMPB和TCNTB的值装填到定时器控制单元里不可见的TCMP和TCNT，TCMP和TCNT分别做实际的定时器占空比控制和计数周期控制。该功能通常用于初始化时的赋初值操作，只需在一瞬间使该位为1即可完成装填TCMP和TCNT的操作。在定时器正式打开工作前，必须将此功能关闭，否则无法计时
	 * [2]: 每个计数周期起始输出电平是否翻转极性，见4412手册P1300的图
	 * [3]:one-shot表示一次计数周期结束后不再重新开启新的计数；auto-reload表示计数周期结束后将自动装载TCNTB和TCMPB的值到TCNT和TCMP之中，重新开启一次新的计数
	 */

	//填充TCMPB和TCNTB的值到定时器内部，只需一瞬间就能完成填充操作，紧接着就可以清除该位。如果不清除该位，那么装填操作将会持续不断，导致倒计时值恒定不变，无法计数
	TCON |= 1 << 1;
	TCON &= ~(1 << 1);

	//auto reload, invert off(P1300,占空比等于TCMPB / TCNTB), 总开关打开
	TCON |= 1 << 3 | 1;
}

static void timer4_handler()
{
	printk("%s\n", __FUNCTION__);

	static int buzz_stat = 0;

	if (buzz_stat) {
		TCNTB0 = PWM0_KHZ / 2; //2KHZ
		TCMPB0 = TCNTB0 / 10;
	} else {
		TCNTB0 = PWM0_KHZ / 4; //4KHZ
		TCMPB0 = TCNTB0 / 5;
	}
	buzz_stat = !buzz_stat;

	//清除中断标志
	TINT_CSTAT |= 1 << 9;
}

static void timer4_init()
{
	printk("%s\n", __FUNCTION__);

	//2级分频
	TCFG1 = (TCFG1 & ~(0xf << 16)) | TIMER4_DIV << 16;

	TCNTB4 = TIMER4_HZ;

	//清除timer4中断标志
	TINT_CSTAT |= 1 << 9;
	request_irq(73, timer4_handler);
	//打开timer4中断
	TINT_CSTAT |= 1 << 4;

	TCON |= 1 << 21;
	TCON &= ~(1 << 21);

	TCON |= 1 << 22 | 1 << 20;
}

//module_init(wdt_init);
module_init(prescaler_init);
module_init(pwm0_init);
module_init(timer4_init);