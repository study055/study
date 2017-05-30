#include "config.h"

#if 1
#define GPA0CON (*(volatile u32*)0x11400000)

#define ULCON (*(volatile u32*)0x13800000)
#define UCON (*(volatile u32*)0x13800004)
#define UFCON (*(volatile u32*)0x13800008)
#define UTRSTAT (*(volatile u32*)0x13800010)
#define UFSTAT (*(volatile u32*)0x13800018)
#define UTXH (*(volatile u8*)0x13800020)
#define URXH (*(volatile u8*)0x13800024)
#define UBRDIV (*(volatile u32*)0x13800028)
#define UFRACVAL (*(volatile u32*)0x1380002c)
#define UINTP (*(volatile u32*)0x13800030)
#define UINTSP (*(volatile u32*)0x13800034)
#define UINTM (*(volatile u8*)0x13800038)

#else

#define GPA0CON (*(volatile u32*)0x21400000)

#define ULCON (*(volatile u32*)0x23800000)
#define UCON (*(volatile u32*)0x23800004)
#define UFCON (*(volatile u32*)0x23800008)
#define UTRSTAT (*(volatile u32*)0x23800010)
#define UFSTAT (*(volatile u32*)0x23800018)
#define UTXH (*(volatile u8*)0x23800020)
#define URXH (*(volatile u8*)0x23800024)
#define UBRDIV (*(volatile u32*)0x23800028)
#define UFRACVAL (*(volatile u32*)0x2380002c)
#define UINTP (*(volatile u32*)0x23800030)
#define UINTSP (*(volatile u32*)0x23800034)
#define UINTM (*(volatile u8*)0x23800038)

#endif

#define SCLK_UART 100000000 //100MHZ，手册P1393，串口时钟
#define BAUD_RATE 115200

static void send_ch(u8 ch)
{
	while (!(UTRSTAT & (1 << 1))) {}
	//判断UTRSTAT
	UTXH = ch;
}

//串口协议遵循windows对于回车的标准，串口的一个回车是由\n\r两个字符组成
void print_ch(u8 ch)
{
	if (ch == '\n') {
		send_ch('\r');
	} else if (ch == '\r') {
		send_ch('\n');
	}
	send_ch(ch);
}

void print_str(const char *str)
{
	for (; *str; ++str) {
		print_ch(*str);
	}
}

u8 uart_recv_poll()
{
	while (!(UTRSTAT & 1)) {}
	//根据UTRSTAT来判断是否收到数据
	return URXH;
}

//中断处理函数
static void uart_handler()
{
	//printk("%s\n", __FUNCTION__);

	//处理rx_fifo中的数据，直到rx_fifo为空
	while ( (UFSTAT & 0xff) || (UFSTAT & (1 << 8)) ) {
		print_ch(URXH);
	}
	//清除中断标志
	UINTP = UINTP;
}

static void uart_init()
{
	//1 管脚GPIO设置GPA0_0和GPA0_1
	GPA0CON = (GPA0CON & ~0xff) | 0x22;

	//2 初始化寄存器
	/* ULCON: 8N1 */
	ULCON = 3;

	/* UCON
	 * [3:0]: 中断或轮询，非DMA模式
	 * [4]: 是否发送停止信号，不发送
	 * [5]: 非环回模式，环回loop-back即自发自收
	 * [6]: 错误中断使能，关
	 * [7]: 接收超时是否产生中断，其实就是接收中断，如果工作在中断模式则必开。因为中断模式基本和fifo一起开
	 * [9:8]: 中断触发类型，只能是电平触发
	 * [10]: 接收超时的dma休眠使能
	 * [11]: 关
	 * [15:12]: 接收超时中断的触发间隔时间
	 * [18:16][22:20]: dma burst size
	 */
	UCON = 0b0101 | 1 << 7 | 1 << 8 | 1 << 9 | 3 << 12;

	/* UFCON[0]: 开关
	 * [2:1]: 发送接收fifo的重置
	 * [6:4]: 当rx_fifo中字节数大于等于该设定值时，触发接收中断
	 * [10:8]: 当tx_fifo中的字节数小于等于该设定值时，触发发送中断
	 */
	//reset fifo
	UFCON |= 1 << 1 | 1 << 2;
	while (UFCON & (3 << 1)) {}

	//rx_fifo满中断触发点是32字节，tx_fifo满中断触发点是32字节
	UFCON = 1 | 1 << 8;

	/* UBRDIV UFRACVAL, 分频得到波特率，详情和公式见手册P1410 */
	//DIV_VAL = SCLK_UART / (bps * 16) - 1 = 100MHZ / (115200 * 16) - 1 = 53.25
	//DIV_VAL = UBRDIV + UFRACVAL/16
	//UBRDIV是DIV_VAL的整数部分53，UFRACVAL/16是小数部分，UFRACVAL = 0.25 * 16 = 4
	UBRDIV = 53;
	UFRACVAL = 4;

	UINTP |= 0xf; //清除中断标志
	UINTM = 0b1110; //开接收中断

	request_irq(84, uart_handler);
}

module_init(uart_init);