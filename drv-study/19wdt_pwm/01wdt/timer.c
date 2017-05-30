#include "config.h"

#define WTCON (*(volatile u16*)0x10060000)
#define WTDAT (*(volatile u16*)0x10060004)
#define WTCNT (*(volatile u16*)0x10060008)
#define WTCLRINT (*(volatile u16*)0x1006000c)

enum {
	PCLK = 100000000, //100MHZ，见手册P446
	PRESCALER = 255,
	CLK_SEL = 3, //0-16;1-32;2-64;3-128
	//1秒钟计时所需的倒计时数
	WDT_HZ = PCLK / (PRESCALER + 1) / (16 << CLK_SEL),
};

static void wdt_handler()
{
	printk("%s\n", __FUNCTION__);

	//任意写操作清除中断标志
	WTCLRINT = 0;
}

static void wdt_init()
{
	WTCON = 1 << 2 | CLK_SEL << 3 | PRESCALER << 8;
	WTDAT = WDT_HZ * 2;
	WTCNT = WDT_HZ * 2;

	request_irq(75, wdt_handler);

	//打开watchdog
	WTCON |= 1 << 5;
}
module_init(wdt_init);
