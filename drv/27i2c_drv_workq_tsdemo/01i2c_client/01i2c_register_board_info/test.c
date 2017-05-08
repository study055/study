#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <mach/irqs.h>
#include <linux/i2c.h>

//参考内核文档Documentation/i2c/instantiating-devices
//i2c_register_board_info只能在内核初始化时执行，不能以模块方式加载

#define I2C_BUSNUM 1

static struct i2c_board_info ldm_i2c_board_info[] = {
	[0] = {
		.type = "ldm", //与i2c_driver匹配的关键词
		.addr = 0x38, //i2c从机地址
		.irq = IRQ_EINT(14),
	},
	[1] = {
		//也可以用该宏进行初始化
		I2C_BOARD_INFO("isp1301_omap", 0x2d),
	},
};

static int __init ldm_init(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	//只能链接在zImage中才能运行，所以没有逆函数
	i2c_register_board_info(I2C_BUSNUM, ldm_i2c_board_info, ARRAY_SIZE(ldm_i2c_board_info));

	return 0;
}

static void __exit ldm_exit(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");