#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <mach/irqs.h>
#include <linux/i2c.h>

//参考内核文档Documentation/i2c/instantiating-devices

#define I2C_BUSNUM 1

//从机地址列表
static const unsigned short normal_i2c[] = { 0x2c, 0x38, I2C_CLIENT_END };

static struct i2c_board_info ldm_i2c_board_info = {
	.type = "ldm", //与i2c_driver匹配的关键词
	.addr = 0x123, //i2c从机地址，使用i2c_new_probed_device时，addr成员不再起作用
	.irq = IRQ_EINT(14),
};

static struct i2c_client *ldm_i2c_client;

static int __init ldm_init(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	int ret = 0;

	//1 通过i2c总线编号找到对应的i2c_adapter
	struct i2c_adapter *i2c_adap = i2c_get_adapter(I2C_BUSNUM);

	//2 创建并注册i2c_client，注意第二个参数不是数组，而是一个单独的i2c_board_info对象
	//第三个参数是从机地址列表数组，能尝试匹配多个不同从机地址，并从硬件上检验这些设备的总线号和从机地址的正确性，当然一般只检查设备存在与否，至于是否是匹配驱动对应的设备，将交由驱动来确定
	//最后的参数是探测函数指针，用于检查从机地址是否正确，如果填入NULL，则由总线框架自动提供默认探测函数，发送7位从机地址，根据ack是否到来来判断从机地址是否正确，但不判断设备是否正确
	ldm_i2c_client = i2c_new_probed_device(i2c_adap, &ldm_i2c_board_info, normal_i2c, NULL);
	if (!ldm_i2c_client) {
		printk(KERN_WARNING "i2c_new_probed_device failed\n");
		ret = -ENODEV;
		goto err_i2c_new_probed_device;
	}

	//3 销毁i2c_adap
	i2c_put_adapter(i2c_adap);

	return 0;
err_i2c_new_probed_device:
	return ret;
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