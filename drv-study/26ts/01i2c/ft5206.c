#include "config.h"

enum {
	TS_I2C_BUS_NUM = 1,
	I2C_SLAVE_ADDR = 0x38,
};

enum {
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
}
module_init(ts_init);