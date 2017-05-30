#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/i2c.h>

static int ldm_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);
	printk("match id table is %s, addr = 0x%x\n", id->name, client->addr);
	return 0;
}

static int ldm_remove(struct i2c_client *client)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);
	return 0;
}

static struct i2c_device_id ldm_id_table[] = {
	{"ldm",}, {},
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