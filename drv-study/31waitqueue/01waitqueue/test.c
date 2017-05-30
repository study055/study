#include <linux/module.h> //module_license
#include <linux/kernel.h> //types.h printk.h
#include <linux/init.h> //__init __exit module_init module_exit

#include <linux/interrupt.h>
#include <mach/irqs.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

//===================================
#include <linux/sched.h>
#include <linux/wait.h> //wait_queue

static wait_queue_head_t wq;
static bool wq_condition;
//===================================

struct key_info {
	const char *name;
	int irqno;
};

static struct key_info key[4] = {
	[0] = {"key0", IRQ_EINT(26)},
	[1] = {"key1", IRQ_EINT(27)},
	[2] = {"key2", IRQ_EINT(28)},
	[3] = {"key3", IRQ_EINT(29)},
};

static irqreturn_t key_handler(int irqno, void *arg)
{
	printk("%s\n", __FUNCTION__);

	struct key_info *pkey = (struct key_info *)arg;

	printk("%s\n", pkey->name);

	//===================================
	//唤醒, wake_up唤醒之前，会先判断condition是否已经为true，如果为false，那么根本不会去唤醒
	wq_condition = 1;
	wake_up_interruptible(&wq);
	//===================================

	return IRQ_HANDLED;
}

static ssize_t ldm_read(struct file *fp, char __user *buf, size_t size, loff_t *offset)
{
	//打印当前进程号和进程名，通过进程号和进程名可知，当前即将被挂起的进程是一个应用层进程
	printk("%s, pid=%u, comm=%s\n", __FUNCTION__, current->pid, current->comm);

	//===================================
	//无条件挂起
	wq_condition = 0;
	wait_event_interruptible(wq, wq_condition);
	//===================================

	return 0;
}

static struct file_operations fops = {
	.read = ldm_read,
};

static struct miscdevice led_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "ldm",
	.fops = &fops,
};

static int __init ldm_init(void)
{
	printk("%s\n", __FUNCTION__);

	int ret = 0;

	//注册中断，成功后可在/proc/interrupt中查看到
	ssize_t i = 0;
	for (i = 0; i < ARRAY_SIZE(key); ++i) {
		ret = request_irq(key[i].irqno, key_handler, IRQF_TRIGGER_FALLING, "ldm", key + i);
		if (ret < 0) {
			printk("request_irq failed\n");
			goto err_request_irq;
		}
	}

	//===================================
	//初始化waitqueue对象
	init_waitqueue_head(&wq);
	//===================================

	ret = misc_register(&led_dev);
	if (ret < 0) {
		printk("misc_register failed\n");
		goto err_misc_register;
	}

	return 0;

err_misc_register:
err_request_irq:
	for (i = i - 1; i >= 0; --i) {
		free_irq(key[i].irqno, key + i);
	}
	return ret;
}

static void __exit ldm_exit(void)
{
	printk("%s\n", __FUNCTION__);

	misc_deregister(&led_dev);
	ssize_t i = 0;
	for (i = ARRAY_SIZE(key) - 1; i >= 0; --i) {
		free_irq(key[i].irqno, key + i);
	}
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");