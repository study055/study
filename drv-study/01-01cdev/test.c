#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/device.h> //class_create device_create

/* 测试方法
 * 手工创建设备结点
 * mknod <devname> c <major number> <minor number>
 * 其中c表示字符设备
 */

enum {
	DEVNO_MAJOR = 1, //原计划主设备号，实际可能注册不了，要由系统分配
};

#define DEVNAME "ldm"

struct ldm_info {
	struct cdev dev;
	struct file_operations fops;
	struct class *cls;
};
static struct ldm_info ldm;

static int ldm_open(struct inode *pnode, struct file *fp)
{
	printk("%s, %d\n", __FUNCTION__, __LINE__);
	return 0;
}

static int __init ldm_init(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	int ret = 0;

	//1 初始化cdev
	//cdev_init() or cdev_alloc();
	cdev_init(&ldm.dev, &ldm.fops);

	//2 获取设备号，查阅内核文档Documentation/devices.h，在文档中列出了当前linux中已经被指定为专有的设备号，找出还未被指定的设备号，用于我们驱动的设备号的注册
	//MKDEV：将主次设备号合并成一个完整的dev_t
	//MAJOR：从dev_t中提取主设备号，主设备号表示设备类型
	//MINOR：从dev_t中提取次设备号，同类型设备之间的编号
	//注册自己指定的设备号
	ldm.dev.dev = MKDEV(DEVNO_MAJOR, 0);
	ret = register_chrdev_region(ldm.dev.dev, 1, DEVNAME);
	if (ret < 0) {
		//如果失败，则由系统自动分配一个目前没人使用的设备号
		ret = alloc_chrdev_region(&ldm.dev.dev, 0, 1, DEVNAME);
		if (ret < 0) {
			printk(KERN_ALERT "alloc_chrdev_region failed\n");
			goto err_alloc_chrdev_region;
		}
	}

	//分配成功后，可通过cat /proc/devices查看设备号注册情况
	printk("device number major=%d, minor=%d\n", MAJOR(ldm.dev.dev), MINOR(ldm.dev.dev));

	//3 填充file operations
	ldm.fops.open = ldm_open;

	//4 可能存在的寄存器操作

	//5 注册设备
	ret = cdev_add(&ldm.dev, ldm.dev.dev, 1);
	if (ret < 0) {
		printk(KERN_ALERT "cdev_add failed\n");
		goto err_cdev_add;
	}

	//6 如需自动创建设备结点，需要另外调用2个函数
	ldm.cls = class_create(THIS_MODULE, DEVNAME);
	if (IS_ERR(ldm.cls)) {
		printk(KERN_ALERT "class_create failed\n");
		ret = PTR_ERR(ldm.cls);
		goto err_class_create;
	}

	//在/sys/class/下创建了ldm目录，以及内容，其中包括了设备号
	//根文件系统中有个叫做mdev的服务，会动态扫描/sys/class的内容，根据内容创建设备结点
	struct device *dev = device_create(ldm.cls, NULL, ldm.dev.dev, NULL, DEVNAME);
	if (IS_ERR(dev)) {
		printk(KERN_ALERT "device_create failed\n");
		ret = PTR_ERR(dev);
		goto err_device_create;
	}

	return 0;

err_device_create:
	class_destroy(ldm.cls);
err_class_create:
	cdev_del(&ldm.dev);
err_cdev_add:
	unregister_chrdev_region(ldm.dev.dev, 1);
err_alloc_chrdev_region:
	return ret;
}

static void __exit ldm_exit(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	device_destroy(ldm.cls, ldm.dev.dev);
	class_destroy(ldm.cls);
	//不管设备结点是自动创建还是手工mknod创建，都会自动删除该设备结点，但是要求设备结点名字和注册设备号时用的名字相同
	cdev_del(&ldm.dev);
	unregister_chrdev_region(ldm.dev.dev, 1);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");