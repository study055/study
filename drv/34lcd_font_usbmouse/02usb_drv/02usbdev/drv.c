#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/usb/input.h>
#include <linux/hid.h>
#include <linux/usb.h>
#include <linux/slab.h>

struct ldm_info {
	struct urb *urb;
	dma_addr_t data_phys;
	int maxp; //当前endpoint最大包的字节数
} info;

static void irq_handler(struct urb *urb)
{
	//urb的数据缓冲区的虚拟首地址
	unsigned char *data = (unsigned char*)urb->context;
	int i = 0;

	//for (; i < 8; ++i) {
	for (; i < info.maxp; ++i) {
		printk("%x ", data[i]);
	}
	printk("\n");

	//中断上下文，不允许有可能导致休眠的函数
	usb_submit_urb(urb, GFP_ATOMIC);
}

//usb鼠标设备的设备号信息
static struct usb_device_id usb_mouse_id_table [] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID, USB_INTERFACE_SUBCLASS_BOOT,
			USB_INTERFACE_PROTOCOL_MOUSE) },
	{ }	/* Terminating entry */
};

static int usb_mouse_probe(struct usb_interface *intf,
		const struct usb_device_id *id)
{
	printk(KERN_EMERG"%s\n", __FUNCTION__);

	//将interface转成usc_device对象来打印设备信息
	struct usb_device *dev = interface_to_usbdev(intf);
	struct usb_device_descriptor *desc = &dev->descriptor;

	printk("idVendor=%x, idProduct=%x, class=%x, subclass=%x\n", desc->idVendor, desc->idProduct, desc->bDeviceClass, desc->bDeviceSubClass);

	//通过获取interface的setting来获取endpointer，不同setting会有不同的endpoint
	//intf->altsetting成员指向该interface的所有setting集合，排列是无序的。而cur_altsetting指当前正在使用的setting
	struct usb_host_interface *interface = intf->cur_altsetting;
	printk("cur interface setting have %d endpoint\n", interface->desc.bNumEndpoints);
	//usb鼠标唯一的endpoint，输入类型，中断类型
	struct usb_endpoint_descriptor *endpoint = &interface->endpoint[0].desc;

	//=======为usb_fill_int_urb获取所需参数============
	//1 创建urb, usb request block，是usb总线数据的载体
	//类型为interrupt、bulk、control的urb，第一个参数为0
	info.urb = usb_alloc_urb(0, GFP_KERNEL);

	//2 设置urb的数据来源endporint的管道，usb_fill_int_urb需要的数据源格式是int类型
	//pipe包括endpoint的类型和地址以及数据方向，以bit的形式出现在该int型常量中
	/* pipe[31:30]: endpoint类型
	 * [7:0]: 数据传输方向to host(0x80) or to device(0)
	 * [14:8]: 所属设备在usb总线上的号码
	 * [29:15]:endpoint编号，在设备插入时由usb总线驱动分配
	 */
	int pipe = usb_rcvintpipe(dev, endpoint->bEndpointAddress);
	printk("endpoint address = %d\n", endpoint->bEndpointAddress);

	//3 设置目标endpoint的包的最大字节数，从端点描述符中可获知，是usb设备固件的属性
	//info.maxp = usb_maxpacket(dev, pipe, usb_pipeout(pipe));
	info.maxp = endpoint->wMaxPacketSize;
	printk("maxp = %d\n", info.maxp);

	//4 创建驱动内接收数据包的数据空间，得到目标内存的虚拟地址和物理地址
	unsigned char *data = usb_alloc_coherent(dev, info.maxp, GFP_KERNEL, &info.data_phys);

	//5 填充urb(usb request block)成员
	//context是urb结构体中的成员，可以作为中断处理函数的参数
	//interval指中断型endpoint轮循usb设备的间隔时间
	usb_fill_int_urb(info.urb, dev, pipe, data, info.maxp, irq_handler, data, endpoint->bInterval);
	printk("endpoint interval = %d\n", endpoint->bInterval);

	//补充初始化usb_fill_int_urb函数没有初始化到的urb成员
	//urb的数据缓冲区的物理地址，需要写入寄存器
	info.urb->transfer_dma = info.data_phys;
	info.urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

	//6 提交urb
	usb_submit_urb(info.urb, GFP_KERNEL);

	return 0;
}

static void usb_mouse_disconnect(struct usb_interface *intf)
{
	printk(KERN_EMERG"%s\n", __FUNCTION__);

	printk("Zzzzzzzzzzzzzzzzzzzz============\n");

	usb_kill_urb(info.urb);
	usb_free_urb(info.urb);
	usb_free_coherent(interface_to_usbdev(intf), info.maxp, info.urb->context, info.data_phys);
}

static struct usb_driver usb_mouse_driver = {
	.name		= "ldm",
	.probe		= usb_mouse_probe,
	.disconnect	= usb_mouse_disconnect,
	.id_table	= usb_mouse_id_table,
};


static int __init usb_init(void)
{
	printk(KERN_EMERG"%s\n", __FUNCTION__);

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
