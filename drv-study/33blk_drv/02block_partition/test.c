#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/blkdev.h>
#include <linux/vmalloc.h>
//#include <linux/spinlock.h> //自旋锁，不会造成休眠
#include <linux/fs.h> //READ宏定义
//=========================================
#include <linux/hdreg.h>
//=========================================

/* 测试驱动
 * 1 使用mkdosfs来格式化设备
 * mkdosfs /dev/XXX
 * 2 使用mount来挂载该设备
 * mount dev/XXX /mnt
 */

//参考内核驱动drivers/block/z2ram.c

//分区功能参考drivers/block/hd.c

enum {
	SECTOR_SIZE = 512,
	DEV_SIZE = (SECTOR_SIZE) * 2048,
};

#define DEVNAME "ldm_blk"

struct ldm_info {
	struct gendisk *disk;
	struct block_device_operations ops;
	u8 *dev_addr; //虚拟的存储设备的数据访问入口
};
static struct ldm_info ldm;

static void do_ldm_req(struct request_queue *q)
{
	//从请求队列上获取一个请求对象
	struct request *req = blk_fetch_request(q);
	while (req) {
		//从第几个扇区开始操作
		size_t start = blk_rq_pos(req) * SECTOR_SIZE;
		//本次请求还剩下多少字节未操作
		size_t len = blk_rq_cur_bytes(req);
		if (len) {
			if (rq_data_dir(req) == READ) {
				memcpy(req->buffer, ldm.dev_addr + start, len);
				printk("read %u bytes from %u\n", len, start);
			} else {
				memcpy(ldm.dev_addr + start, req->buffer, len);
				printk("write %u bytes to %u\n", len, start);
			}
		}
		//__blk_end_request_cur：返回false表示当前req的所有操作都完成了，于是下面试图调用blk_fetch_request再从队列上获取新的请求，如果获取不到，则req得到NULL将退出循环；
		//返回true的话说明当前req操作还没完成，继续循环执行
		//err参数可以独立改变__blk_end_request_cur的返回值，当发生其他错误时可以用err参数来决定程序的走向
		if (!__blk_end_request_cur(req, 0)) {
			req = blk_fetch_request(q);
		}
	}
}

//=========================================
static int ldm_getgeo(struct block_device *bdev, struct hd_geometry *geo)
{
	geo->heads = 2;
	geo->cylinders = 30;
	geo->sectors = DEV_SIZE / SECTOR_SIZE / geo->heads / geo->cylinders;
	printk("heads=%u, cylinders=%u setcors=%u\n", geo->heads, geo->cylinders, geo->sectors);
	return 0;
}
//=========================================

static int __init ldm_init(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	int ret = 0;

	//1 创建块设备gendisk的主对象，函数参数是最大分区数
	ldm.disk = alloc_disk(5);
	if (!ldm.disk) {
		printk("alloc_disk failed\n");
		ret = -ENOMEM;
		goto err_alloc_disk;
	}

	//2 注册申请设备号
	//第一个参数为0将返回自动分配的设备号，如果是1-255则手动分配，返回0表示成功
	ldm.disk->major = register_blkdev(0, DEVNAME);
	if (ldm.disk->major < 0) {
		printk("register_blkdev failed\n");
		ret = ldm.disk->major;
		goto err_register_blkdev;
	}

	//3 填充成员
	sprintf(ldm.disk->disk_name, DEVNAME);
	ldm.disk->fops = &ldm.ops; //包含分区所需的方法
	//=========================================
	ldm.ops.getgeo = ldm_getgeo;
	//=========================================

	//4 创建块设备的存储空间，我们分配一块内存来模拟存储设备
	//分配的内存在物理上不连续，但虚拟地址是连续的，实际是内存碎片拼接而成，可减少内存的实际消耗，适合大块的数据创建
	ldm.dev_addr = vzalloc(DEV_SIZE);
	if (!ldm.dev_addr) {
		printk("vzalloc failed\n");
		ret = -ENOMEM;
		goto err_vzalloc;
	}

	//在gendisk中设置设备存储空间的大小，注意单位是扇区
	set_capacity(ldm.disk, DEV_SIZE / SECTOR_SIZE);

	//5 注册块设备所需的操作队列,提供框架所需的自旋锁对象
	//spin_lock_init();
	ldm.disk->queue = blk_init_queue(do_ldm_req, NULL);
	if (!ldm.disk->queue) {
		printk("blk_init_queue failed\n");
		ret = -EINVAL;
		goto err_blk_init_queue;
	}

	//6 注册块设备gendisk对象
	add_disk(ldm.disk);

	return 0;

	blk_cleanup_queue(ldm.disk->queue);
err_blk_init_queue:
	vfree(ldm.dev_addr);
err_vzalloc:
	unregister_blkdev(ldm.disk->major, DEVNAME);
err_register_blkdev:
	put_disk(ldm.disk);
err_alloc_disk:
	return ret;
}

static void __exit ldm_exit(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	del_gendisk(ldm.disk);
	blk_cleanup_queue(ldm.disk->queue);
	vfree(ldm.dev_addr);
	unregister_blkdev(ldm.disk->major, DEVNAME);
	put_disk(ldm.disk);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");