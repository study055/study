#include "config.h"

void start_armboot()
{
	printk("%s, %d, %s\n", __FUNCTION__, __LINE__, __FILE__);

	bootm(0x40008000);

	while (1) {}
}