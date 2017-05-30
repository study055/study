#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static void ldm_work_handler()
{
	i2c_smbus_read_i2c_block_data();

	//根据触摸屏芯片内的event_flag信息获知按下抬起接触状态, 按下时发送value=1，抬起时value=0
	switch (event_flag) {
	case PUT_DOWN:
	case CONTACT:
		input_report_key(); //按下
		break;
	case PUT_UP:
		input_report_key(); //抬起
	}

	input_report_abs(, ABS_X, );
	input_report_abs(, ABS_Y, );

	//哨兵报告，表示描述这次动作的报告结束
	input_sync();
}

static irqreturn_t ldm_handler(int irqno, void *data)
{
	schedule_work();

	return IRQ_HANDLED;
}

static int __init ldm_init(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	//1 创建input_dev对象
	input_allocate_device();

	//2 设置输入设备的输入信号类型
	//evbit等成员因为必须同时接收多种属性的设置，所以采用类似flags的位操作方法使得多种属性可以共存，并且通过数组的形式，扩展出无限多的位数来容纳无限多的属性。选中对应的属性只需将成员对应的位做置1操作即可
	//set_bit将某位做置1操作，可跨越32位的限制的位操作
	set_bit(EV_KEY, evbit);
	EV_ABS

	//3 注册所需的按键键值
	set_bit(BTN_TOUCH, keybit);
	input_set_abs_params(, ABS_X, 0, 800-1, 0, 0);
	input_set_abs_params(, ABS_Y, 0, 480-1, 0, 0);

	//4 注册input设备
	ret = input_register_device(ldm.dev);
	if (ret < 0) {
		printk(KERN_ALERT "input_register_device failed\n");
		goto err_input_register_device;
	}

	//5 硬件操作，申请中断
	ret = request_irq();

	return 0;

err_request_irq:
	input_unregister_device(ldm.dev);
err_input_register_device:
	input_free_device(ldm.dev);
err_input_allocate_device:
	return ret;
}

static void __exit ldm_exit(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	free_irq(ldm.key[i].irqno, ldm.key + i);
	input_unregister_device(ldm.dev);
	input_free_device(ldm.dev);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");