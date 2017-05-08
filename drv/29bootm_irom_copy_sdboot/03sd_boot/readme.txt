=========4412开发板启动u-boot过程，4412手册P95==============
1 上电后首先执行irom固化程序
2 根据OM跳线的选择，初始化对应的启动设备，比如sd卡和emmc
3 从指定启动设备中的固定位置(位置参考Android_Exynos4412_iROM_Secure_Booting_Guide_Ver.1.00.00手册P24)读出bl1，通常这是个三星的闭源二进制文件，来自uboot源码目录sd_fuse/tiny4412/E4412_N_bl1.bin
4 读出的bl1存放到SoC内部集成的iram（stepping stone）中的0x02021400并执行。iram的地址分布参考Android_Exynos4412_iROM_Secure_Booting_Guide_Ver.1.00.00手册P14
5 bl1将自动根据OM跳线的选择，继续从启动设备中读取bl2到0x02023400并执行bl2。bl2.bin就是u-boot.bin的前14k内容，用u-boot源码目录下的sd_fuse/V310-EVT1-mkbl2.c所编译出的mkbl2制作而成
6 bl2初始化时钟和内存，并根据OM跳线选择，使用irom预留的读取函数，将u-boot.bin从启动设备中读到片外ddr内存对应的位置并执行

=============u-boot烧写进sd卡的过程，全过程全部都在u-boot源码目录下的sd_fuse/tiny4412/sd_fusing.sh================
