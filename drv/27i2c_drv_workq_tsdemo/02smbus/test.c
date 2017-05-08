#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/i2c.h>

//通过i2c读写接口访问触摸屏的寄存器。读写函数参考内核文档Documentation/i2c/smbus-protocol
//i2c_smbus_read_i2c_block_data

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


static int ldm_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);
	printk("match id table is %s, addr = 0x%x\n", id->name, client->addr);

	struct ts_misc_info misc_info = {0};
	i2c_smbus_read_i2c_block_data(client, ID_G_CIPHER, sizeof(misc_info), (u8*)&misc_info);

	printk("chip_vendor_id=0x%x, firmware_id=0x%x, ctpm_vendor_id=0x%x\n", misc_info.chip_vendor_id, misc_info.firmware_id, misc_info.ctpm_vendor_id);

	if (misc_info.interrupt_status) {
		printk("interrupt is trigger mode\n");
	} else {
		printk("interrupt is polling mode\n");
	}

	return 0;
}

static int ldm_remove(struct i2c_client *client)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);
	return 0;
}

static struct i2c_device_id ldm_id_table[] = {
	{"ldm",}, {},
};

static struct i2c_driver ldm_drv = {
	.probe = ldm_probe,
	.remove = ldm_remove,
	.id_table = ldm_id_table,
	.driver = {
		.name = "abc",
	},
};

static int __init ldm_init(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	return i2c_add_driver(&ldm_drv);
}

static void __exit ldm_exit(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	i2c_del_driver(&ldm_drv);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");