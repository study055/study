#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/usb/input.h>
#include <linux/hid.h>
#include <linux/usb.h>

//参考内核drivers/hid/usbhid/usbmouse.c

/* 测试模块时，需要把内核中的鼠标驱动去掉，以免冲突
 * make menuconfig
 * 老版本的内核2.6.36
 * Device Drivers -> HID Devices -> USB Human Interface Device (full HID) support去掉
 * 新版本的内核3.10.2
 * Device Drivers -> HID Support -> USB HID support -> USB HID transport layer
 */

//usb鼠标设备的匹配信息。USB_INTERFACE_INFO这个宏用来从usb_device_id中选择匹配的成员，用这些成员来匹配对应的usb_interface。实际上用哪些成员来匹配取决与usb_device_id里的match_flag成员，由macth_flag指定的几个成员必须与usb_interface中相对应的成员全部匹配，有一个不匹配就不算匹配
//usb_interface是由设备插入时，usb总线驱动自动创建并注册的
static struct usb_device_id usb_mouse_id_table[] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,
			USB_INTERFACE_PROTOCOL_MOUSE) },
	{ }	/* Terminating entry */
};

static int usb_mouse_probe(struct usb_interface *intf,
		const struct usb_device_id *id)
{
	printk("%s\n", __FUNCTION__);

	struct usb_device *usbdev = interface_to_usbdev(intf);
	struct usb_device_descriptor *desc = &usbdev->descriptor;

	printk("Ssssssssssssssssss============\n");

	printk("idVendor=%x, idProduct=%x, class=%x, subclass=%x\n", desc->idVendor, desc->idProduct, desc->bDeviceClass, desc->bDeviceSubClass);

	return 0;
}

static void usb_mouse_disconnect(struct usb_interface *intf)
{
	printk("%s\n", __FUNCTION__);

	printk("Zzzzzzzzzzzzzzzzzzzz============\n");
}

static struct usb_driver usb_mouse_driver = {
	.name		= "usbmouse",
	.probe		= usb_mouse_probe,
	.disconnect	= usb_mouse_disconnect,
	.id_table	= usb_mouse_id_table,
};


static int __init usb_init(void)
{
	printk("%s\n", __FUNCTION__);

	if (usb_register(&usb_mouse_driver) < 0) {
		printk("%s: usb_register failed", __FUNCTION__);
		goto err_usb_register;
	}

	return 0;
err_usb_register:
	return -1;
}

static void __exit usb_exit(void)
{
	printk(KERN_EMERG"%s\n", __FUNCTION__);

	usb_deregister(&usb_mouse_driver);
}

module_init(usb_init);
module_exit(usb_exit);

MODULE_LICENSE("GPL");
