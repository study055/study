#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <mach/irqs.h>
#include <linux/i2c.h>

//参考内核文档Documentation/i2c/instantiating-devices

#define I2C_BUSNUM 1

static struct i2c_board_info ldm_i2c_board_info = {
	.type = "ldm", //与i2c_driver匹配的关键词
	.addr = 0x38, //i2c从机地址
	.irq = IRQ_EINT(14),
};

static struct i2c_client *ldm_i2c_client;

static int __init ldm_init(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	//1 通过i2c总线编号找到对应的i2c_adapter
	struct i2c_adapter *i2c_adap = i2c_get_adapter(I2C_BUSNUM);

	//2 创建并注册i2c_client，注意第二个参数不是数组，而是一个单独的i2c_board_info对象
	//无法从硬件上检验总线号和从机地址的正确性
	ldm_i2c_client = i2c_new_device(i2c_adap, &ldm_i2c_board_info);

	//3 销毁i2c_adap
	i2c_put_adapter(i2c_adap);

	return 0;
}

static void __exit ldm_exit(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	//i2c_new_device的逆函数
	i2c_unregister_device(ldm_i2c_client);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");