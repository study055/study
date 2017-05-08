#define TTB 0x46000000 //页表首地址

.global _start
_start:
	//异常向量表（Exception vectors），cpu发生异常时会自动跳转到异常向量表中对应的地址上执行该地址上的指令
	//异常向量表的格式见arm v7手册P1149

	b reset
	nop
	ldr pc, =swi_handler
	nop
	nop
	nop
	nop
	nop

reset:
	push {lr}

	//指定异常向量表基地址为本程序的首地址, a8手册P195
	MRC p15, 0, r0, c12, c0, 0
	ldr r0, =0x40000000
	MCR p15, 0, r0, c12, c0, 0

	bl clear_bss
	bl do_initcall

	swi 0x123456

	bl start_armboot

	pop {pc}

clear_bss:
	push {lr}

	ldr r0, =__bss_start__
	ldr r1, =__bss_end__
	mov r2, #0

	//排除bss段为空的情况
	cmp r0, r1
	beq 2f

1:
	str r2, [r0]
	add r0, #4
	cmp r0, r1
	bne 1b

2:
	pop {pc}

do_initcall:
	push {r4, r5, lr}

	ldr r4, =_INITCALL_START
	ldr r5, =_INITCALL_END

	//排除initcall段为空的情况
	cmp r4, r5
	beq 2f

1:
	ldr r0, [r4]
	mov lr, pc
	mov pc, r0

	add r4, #4
	cmp r4, r5
	bne 1b

2:
	pop {r4, r5, pc}


//==============exception handlers================
//software interrupt软中断，系统调用就是基于该机制实现。执行swi指令就会立即触发
//系统调用中，利用swi指令的操作数作为系统调用号来区分不同的系统调用操作，所以，必然要有取系统调用号的操作
swi_handler:
	push {lr}

	//将swi指令的操作数取出来，调用printk打印
	//lr保存着整个swi_handler的返回地址，也就是swi指令的下一条指令，所以lr-4即swi指令的地址
	ldr r1, [lr, #-4] //r1此时就是swi指令本身的内容
	bic r1, #(0xff << 24) //swi操作数为指令的低24位，保留下来
	ldr r0, =swi_fmt
	bl printk

	//同在SVC下触发和处理，不发生模式切换
	pop {pc}

swi_fmt:
	.asciz "swi number = 0x%x\n"
