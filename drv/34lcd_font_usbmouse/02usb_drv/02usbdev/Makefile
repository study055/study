obj-m := ldm.o
$(obj-m:.o=-objs) := drv.o

KERNEL_DIR := ~/work/4412/linux/linux-3.5/
NFSROOT := ~/work/nfsroot/
MODULE_DIR := $(NFSROOT)

ccflags-y += -Wno-declaration-after-statement

MAKE := make

all: build copy clean

build:
	$(MAKE) M=`pwd` -C $(KERNEL_DIR) modules

copy:
	cp $(obj-m:.o=.ko) $(MODULE_DIR)

clean:
	$(MAKE) M=`pwd` -C $(KERNEL_DIR) clean

.PHONY: all clean copy build
