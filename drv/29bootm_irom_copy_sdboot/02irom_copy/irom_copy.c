#include "config.h"

/* sd_readblk: 借用irom留下的存储设备读取接口来读取sd卡中的数据到内存中
 * 参考Android_Exynos4412_iROM_Secure_Booting_Guide_Ver.1.00.00.pdf P21
 * @src_blk: 从sd卡第几个块开始读
 * @num_of_blk: 读取多少个块
 * @dst_addr: 读出的数据存放到内存的什么地址
 * @return: True=1/False=0
 */
int sd_readblk(u32 src_blk, u32 num_of_blk, u32 dst_addr)
{
	//根据手册说明，0x02020030的地址中的4个字节内容为函数入口
	int (*pfunc)(u32, u32, u32) = (int (*)(u32, u32, u32)) (*(u32*)0x02020030);

	return pfunc(src_blk, num_of_blk, dst_addr);
}

void emmc_readblk(u32 src_blk, u32 num_of_blk, u32 dst_addr)
{
	int (*pfunc)(u32, u32) = (int (*)(u32, u32)) (*(u32*)0x02020044);

	pfunc(num_of_blk, dst_addr);
}