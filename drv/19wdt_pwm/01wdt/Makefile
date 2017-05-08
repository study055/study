TGT := boot.bin
OBJS := start.S _divsi3.S _modsi3.S _udivsi3.S _umodsi3.S
OBJC := main.c led.c uart.c printk.c interrupt.c timer.c

CROSS_COMPILER := arm-linux-
CC := $(CROSS_COMPILER)gcc
LD := $(CROSS_COMPILER)ld
OBJCOPY := $(CROSS_COMPILER)objcopy
OBJDUMP := $(CROSS_COMPILER)objdump

CFLAGS := -ffreestanding
LDFLAGS := -Tboot.lds

all: build

build:
	$(CC) $(CFLAGS) -c $(OBJS) $(OBJC)
	$(LD) $(LDFLAGS) $(OBJS:.S=.o) $(OBJC:.c=.o)
	$(OBJCOPY) -O binary a.out $(TGT)

clean:
	$(RM) $(OBJS:.S=.o) $(OBJC:.c=.o) $(TGT) a.out

.PHONY: all clean build
