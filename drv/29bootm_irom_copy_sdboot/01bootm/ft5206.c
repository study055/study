#include "config.h"
#include "lcd.h"

extern struct win_info win[];

#define GPX1CON (*(volatile u32*)0x11000c20)
#define EXT_INT41CON (*(volatile u32*)0x11000e04)
#define EXT_INT41_MASK (*(volatile u8*)0x11000f04)
#define EXT_INT41_PEND (*(volatile u8*)0x11000f44)

enum {
	TS_I2C_BUS_NUM = 1,
	I2C_SLAVE_ADDR = 0x38,
};

enum {
	TD_STATUS = 2,
	TOUCH1_XH = 3,
	ID_G_CIPHER = 0xa3,
};

struct ts_misc_info {
	u8 chip_vendor_id;
	u8 interrupt_status;
	u8 power_consume_mode;
	u8 firmware_id;
	u8 running_state;
	u8 ctpm_vendor_id;
};

struct ts_point_reg {
	u8 xh, xl, yh, yl;
	u16 dummy;
};

struct ts_point_data {
	u16 x, y; //最终坐标数据
	u8 event_flag; //按下抬起的动作状态
};

struct ts_info {
	//从ft5206的寄存器中读出的各触摸点原始数据
	struct ts_point_reg pt_reg[5];
	//经过处理后的触摸点坐标和状态数据
	struct ts_point_data pt_data[5];
	u8 num_of_pt; //当前触摸点的数量
};
static struct ts_info ts;

static void ts_handler()
{
	u8 buf = 0;
	//读出触摸点的数量
	i2c_block_read(TS_I2C_BUS_NUM, I2C_SLAVE_ADDR, &buf, sizeof(buf), TD_STATUS);
	ts.num_of_pt = buf & 0xf;
	//printk("num of point is %u\n", ts.num_of_pt);

	if (!ts.num_of_pt) {
		ts.num_of_pt = 1;
	}

	//读出当前所有触摸点的原始数据
	i2c_block_read(TS_I2C_BUS_NUM, I2C_SLAVE_ADDR, ts.pt_reg, sizeof(struct ts_point_reg) * ts.num_of_pt, TOUCH1_XH);

	//处理触摸点数据
	size_t i = 0;
	for (i = 0; i < ts.num_of_pt; ++i) {
		ts.pt_data[i].event_flag = ts.pt_reg[i].xh >> 6;
		ts.pt_data[i].x = ((u16)(ts.pt_reg[i].xh & 0xf) << 8) | ts.pt_reg[i].xl;
		ts.pt_data[i].y = ((u16)(ts.pt_reg[i].yh & 0xf) << 8) | ts.pt_reg[i].yl;

		switch (ts.pt_data[i].event_flag) {
		case 0:
			//printk("pt%u Put Down ", i);
			break;
		case 1:
			//printk("pt%u Put Up ", i);
			break;
		case 2:
			//printk("pt%u Contact ", i);
			break;
		}
		//printk("x=%u, y=%u\n", ts.pt_data[i].x, ts.pt_data[i].y);

		draw_rect(win, ts.pt_data[i].x, ts.pt_data[i].y, 5, 5, RGB888(255, 0, 0));
	}

	EXT_INT41_PEND |= 1 << 6; //清除中断标志
}

static void ts_init()
{
	i2c_init(TS_I2C_BUS_NUM);

	struct ts_misc_info misc_info = {0};
	i2c_block_read(TS_I2C_BUS_NUM, I2C_SLAVE_ADDR, &misc_info, sizeof(misc_info), ID_G_CIPHER);

	printk("chip_vendor_id=0x%x, firmware_id=0x%x, ctpm_vendor_id=0x%x\n", misc_info.chip_vendor_id, misc_info.firmware_id, misc_info.ctpm_vendor_id);

	if (misc_info.interrupt_status) {
		printk("interrupt is trigger mode\n");
	} else {
		printk("interrupt is polling mode\n");
	}

	//打开EINT14,即GPX1_6, 设为下降沿
	GPX1CON |= 0xf << 24; //外部中断模式
	EXT_INT41CON = (EXT_INT41CON & ~(7 << 24)) | 0x2 << 24; //下降沿触发
	EXT_INT41_PEND |= 1 << 6; //清除中断标志
	EXT_INT41_MASK &= ~(1 << 6); //开中断
	request_irq(62, ts_handler);
}
//module_init(ts_init);