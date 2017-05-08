		应用层
=================================================
	设备驱动，我们来完成
	总线驱动，控制SoC的ohci usb host controller
================================================s
	usb固件驱动 usb gadget driver

usb主机控制器(host controller)规范
低速1.5Mbps（usb1.0）/全速12Mbps（usb1.1）
UHCI：intel，一般用于pc平台
OHCI：微软 低速/全速，一般用于嵌入式系统

EHCI：高速480Mbps（usb2.0）
OTG

usb设备的firmware，不能主动发送请求，只能被动回应总线驱动的请求，也没有外部中断可以发给usb总线控制器

即插即用的实现:
1 通过硬件电路来获知设备的连接时机以及基本速率
usb host口中d-和d+都通过15k电阻下拉，usb设备的接口中，全速设备1.1将d-用5k上拉，高速设备2.0将d+用5k上拉，由此改变usb host端d-和d+管脚的电平
2 总线驱动主动发送询问请求，设备返回对应的信息，包括设备id和厂商id，包括设备描述符，具备4种描述符:
device(struct _DEVICE_DESCRIPTOR_STRUCT)
config(struct _CONFIGURATION_DESCRIPTOR_STRUCT)
interface(struct _INTERFACE_DESCRIPTOR_STRUCT)
endpoint(struct _ENDPOIN_DESCRIPTOR_STRUCT)（基本数据传输通道，单向，四种类型）。总线驱动会根据这个抽象模型创建出对应的interface
                 device
       config                  config
interface   interface         interface
endpoint   endpoint endpoint    endpoint

每个usb设备的endpoint有自己的编号地址，由主机控制器分配，主机控制器发出的所有命令都带有对应编号

在总线驱动在设备插入后发送控制请求，获悉设备的数据ID信息包括读写方法，分配编号地址，新插入usb设备在未分配地址前用0作为默认编号地址

端点endpoint：数据传输的入口和出口
每个端点都是单向的，除端点0外，端点0用于控制传输
每个端点都只有一种传输类型

endpoint传输类型
1 CONTROL 控制传输：先进先出队列方式处理，usb设备的识别操作
2 INTERRUPT 中断传输：需要主机以固定的时间间隔来轮询，输入设备
3 BULK 批量传输：可靠，非实时，网卡，U盘，打印机，数码相机
4 ISOCHRONOUS 实时传输：以固定的速率不间断的传输数据，摄像头

usb驱动框架
          usb_bus_type
usb_interface        usb_driver
                  .id_table   .probe

测试模块时，需要把内核中的鼠标驱动去掉，以免冲突
make menuconfig
老版本的内核2.6.36
Device Drivers -> HID Devices -> USB Human Interface Device (full HID) support去掉
新版本的内核3.10.2
Device Drivers -> HID Support -> USB HID support -> USB HID transport layer
