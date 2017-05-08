#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/i2c.h>
#include <linux/input.h>
#include <mach/irqs.h>
#include <linux/interrupt.h>
#include <linux/slab.h> //kzalloc
#include <linux/workqueue.h>

#include <plat/ft5x0x_touch.h>
#include <linux/gpio.h> //gpio_to_irq

//内核中的mach-tiny4412.c已经注册了触摸屏的i2c_client，可以直接匹配，并将驱动关掉，也可以把i2c_client和驱动一起关掉
//只关驱动：将drivers/input/touchscreen/Makefile中的obj-$(CONFIG_TOUCHSCREEN_FT5X0X)	+= ft5x06_ts.o注释掉
//两者全关，make menuconfig -> Device Drivers -> Input device support -> Touchscreens -> FocalTech ft5x0x TouchScreen driver关闭

//cat /proc/bus/input/devices查看当前注册的输入设备的信息

#define DEVNAME "ldm"

enum {
	TD_STATUS = 2,
	TOUCH1_XH = 3,
	ID_G_CIPHER = 0xa3,
};

enum {
	PUT_DOWN = 0,
	PUT_UP = 1,
	CONTACT = 2,
};

struct ts_point_reg {
	u8 xh, xl, yh, yl;
	u16 dummy;
};

struct ts_point_data {
	u16 x, y; //最终坐标数据
	u8 event_flag; //按下抬起的动作状态
};

struct ldm_info {
	struct input_dev *dev;
	struct work_struct work;
	//指向本对象所在的i2c_client对象地址
	struct i2c_client *client;
	struct ft5x0x_i2c_platform_data *pdata;

	//从ft5206的寄存器中读出的各触摸点原始数据
	struct ts_point_reg pt_reg;
	//经过处理后的触摸点坐标和状态数据
	struct ts_point_data pt_data;
};

//本函数将在标准进程上下文中执行，而非中断上下文
static void ldm_work_func(struct work_struct *work)
{
	//printk(KERN_ALERT "%s\n", __FUNCTION__);

	struct ldm_info *ldm = container_of(work, struct ldm_info, work);

	struct i2c_client *client = ldm->client;

	//读取第一个触摸点的数据
	i2c_smbus_read_i2c_block_data(client, TOUCH1_XH, sizeof(struct ts_point_reg), (u8*)&ldm->pt_reg);

	ldm->pt_data.event_flag = ldm->pt_reg.xh >> 6;
	ldm->pt_data.x = ((u16)(ldm->pt_reg.xh & 0xf) << 8) | ldm->pt_reg.xl;
	ldm->pt_data.y = ((u16)(ldm->pt_reg.yh & 0xf) << 8) | ldm->pt_reg.yl;

	//根据触摸屏芯片内的event_flag信息获知按下抬起接触状态, 按下时发送value=1，抬起时value=0
	switch (ldm->pt_data.event_flag) {
	case PUT_DOWN:
	case CONTACT:
		input_report_key(ldm->dev, BTN_TOUCH, 1); //按下
		break;
	case PUT_UP:
		input_report_key(ldm->dev, BTN_TOUCH, 0); //抬起
	}

	//报告坐标
	input_report_abs(ldm->dev, ABS_X, ldm->pt_data.x);
	input_report_abs(ldm->dev, ABS_Y, ldm->pt_data.y);

	printk("x=%u, y=%u\n", ldm->pt_data.x, ldm->pt_data.y);

	//哨兵报告，表示描述这次动作的报告结束
	input_sync(ldm->dev);
}

static irqreturn_t ldm_handler(int irqno, void *data)
{
	struct i2c_client *client = (struct i2c_client *)data;
	struct ldm_info *ldm = i2c_get_clientdata(client);

	//退出中断模式后，立即执行work对象中绑定的func成员函数
	schedule_work(&ldm->work);

	return IRQ_HANDLED;
}

static int ldm_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	int ret = 0;

	//1 创建私有数据空间，并保存在i2c_client中
	struct ldm_info *ldm = kzalloc(sizeof(struct ldm_info), GFP_KERNEL);
	if (!ldm) {
		ret = -ENOMEM;
		printk(KERN_WARNING "kzalloc ldm failed\n");
		goto err_kzalloc_ldm;
	}

	//将ldm对象作为私有数据保存在i2c_client对象中来传递
	i2c_set_clientdata(client, ldm);
	ldm->client = client;

	//从i2c_client中获取在mach-tiny4412.c中注册的硬件私有数据，包括中断号和分辨率
	ldm->pdata = (struct ft5x0x_i2c_platform_data*)client->dev.platform_data;

	//获取中断号
	client->irq = (int)gpio_to_irq(ldm->pdata->gpio_irq);

	//2 创建input_dev对象
	ldm->dev = input_allocate_device();
	if (!ldm->dev) {
		printk(KERN_ALERT "input_allocate_device failed\n");
		ret = -ENOMEM;
		goto err_input_allocate_device;
	}

	//3 设置输入设备的输入信号类型
	//evbit等成员因为必须同时接收多种属性的设置，所以采用类似flags的位操作方法使得多种属性可以共存，并且通过数组的形式，扩展出无限多的位数来容纳无限多的属性。选中对应的属性只需将成员对应的位做置1操作即可
	//set_bit将某位做置1操作，可跨越32位的限制的位操作
	set_bit(EV_KEY, ldm->dev->evbit);
	set_bit(EV_ABS, ldm->dev->evbit);

	//4 注册所需的键值
	//触摸屏本身可看成一个按键，键值为BTN_TOUCH
	set_bit(BTN_TOUCH, ldm->dev->keybit);
	input_set_abs_params(ldm->dev, ABS_X, 0, ldm->pdata->screen_max_x - 1, 0, 0);
	input_set_abs_params(ldm->dev, ABS_Y, 0, ldm->pdata->screen_max_y - 1, 0, 0);

	//5 硬件操作，申请中断
	ret = request_irq(client->irq, ldm_handler, IRQF_TRIGGER_FALLING, DEVNAME, client);
	if (ret < 0) {
		printk(KERN_ERR "request_irq failed\n");
		goto err_request_irq;
	}

	//6 注册input设备
	ret = input_register_device(ldm->dev);
	if (ret < 0) {
		printk(KERN_ALERT "input_register_device failed\n");
		goto err_input_register_device;
	}

	//7 初始化工作队列的对象
	INIT_WORK(&ldm->work, ldm_work_func);

	return 0;

err_input_register_device:
	free_irq(client->irq, client);
err_request_irq:
	input_free_device(ldm->dev);
err_input_allocate_device:
	kfree(ldm);
err_kzalloc_ldm:
	return ret;
}

static int ldm_remove(struct i2c_client *client)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	struct ldm_info *ldm = i2c_get_clientdata(client);

	input_unregister_device(ldm->dev);
	free_irq(client->irq, client);
	input_free_device(ldm->dev);
	kfree(ldm);

	return 0;
}

static struct i2c_device_id ldm_id_table[] = {
	{"ft5x0x_ts",}, {},
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