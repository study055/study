#include "config.h"

void start_armboot()
{
	printk("%s, %d, %s\n", __FUNCTION__, __LINE__, __FILE__);

	while (1) {}
}