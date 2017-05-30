#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/blkdev.h>
#include <linux/vmalloc.h>
#include <linux/spinlock.h> //自旋锁，不会造成休眠
#include <linux/fs.h> //READ宏定义

/* 测试驱动
 * 1 使用mkdosfs来格式化设备
 * mkdosfs /dev/XXX
 * 2 使用mount来挂载该设备
 * mount dev/XXX /mnt
 */

//参考内核驱动drivers/block/z2ram.c

enum {
	SECTOR_SIZE = 512,
};

static void do_ldm_req(struct request_queue *q)
{
	//从请求队列上获取一个请求对象
	req = blk_fetch_request(q);
	while (req) {
		//从第几个扇区开始操作
		blk_rq_pos();
		//本次请求还剩下多少自己未操作
		blk_rq_cur_bytes()
		while (len) {
			if (rq_data_dir(req) == READ) {
				memcpy();
				printk();
			} else {
				memcpy();
				printk();
			}
		}
		if (!__blk_end_request_cur()) {
			req = blk_fetch_request(q);
		}
	}
}

static int __init ldm_init(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	//1 注册申请设备号
	register_blkdev();

	//2 创建块设备gendisk的主对象，函数参数是最大分区数
	alloc_disk();

	//3 填充成员
	major = ;
	sprintf(disk_name);
	fops = ; //包含分区所需的方法

	//4 创建块设备的存储空间
	vmalloc(); //分配的内存不连续，减少内存的实际消耗，适合大块的数据创建

	//填充gendisk相关成员，注意单位是扇区
	set_capacity();

	//5 注册块设备所需的操作队列,提供框架所需的自旋锁对象
	spin_lock_init();
	blk_init_queue(do_ldm_req, );

	return 0;
}

static void __exit ldm_exit(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");