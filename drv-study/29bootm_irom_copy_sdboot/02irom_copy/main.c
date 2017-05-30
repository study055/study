#include "config.h"

void start_armboot()
{
	printk("%s, %d, %s\n", __FUNCTION__, __LINE__, __FILE__);

	//读取zImage到内存
	sd_readblk(1057, 9335, 0x40008000);

	//从emmc的第0x384（900）个块开始，读出0x2477（9335）个块到内存0x40008000
	//emmc_readblk(9335, 0x40008000);

	bootm(0x40008000);

	while (1) {}
}