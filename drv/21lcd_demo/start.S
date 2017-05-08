#define TTB 0x46000000 //页表首地址

.global _start
_start:
	//异常向量表（Exception vectors），cpu发生异常时会自动跳转到异常向量表中对应的地址上执行该地址上的指令
	//异常向量表的格式见arm v7手册P1149

	b reset
	nop
	ldr pc, =swi_handler
	nop
	ldr pc, =data_abort_handler
	nop
	ldr pc, =irq_handler
	nop

reset:
	push {lr}

	//指定异常向量表基地址为本程序的首地址, a8手册P195
	MRC p15, 0, r0, c12, c0, 0
	ldr r0, =0x40000000
	MCR p15, 0, r0, c12, c0, 0

	bl clear_bss
	bl do_initcall

	//触发swi异常
	swi 0x123456

	//触发data_abort异常
	mov r0, #0
	str r1, [r0] //向irom设备做写操作，触发数据异常

	//打开cpu内的中断总开关
	mrs r0, cpsr
	bic r0, #(1 << 7)
	msr cpsr, r0

	//设置中断模式下的栈
	//从svc模式切换到中断模式，参考a8手册P65
	mrs r0, cpsr
	bic r0, #1
	msr cpsr, r0
	ldr sp, =0x49500000 //在中断模式下设置栈
	//切回svc模式
	orr r0, #1
	msr cpsr, r0



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

data_abort_handler:
	//异常发生后，cpu模式自动切换到相应异常模式，每种cpu模式下的sp和lr都是独立的，所以要为每个模式下的sp设定不同的值
	ldr sp, =0x49000000

	//发生异常时，cpu切换模式，并且将异常的返回地址自动赋值给lr，但是有一些偏差，具体偏差值参考a8手册P67表格，实测偏差值是4不是手册上的8
	sub lr, #4

	//将r0-r3压栈是因为handler内覆盖了r0-r3的值，当异常处理结束返回时，原模式流程的r0值将消失，可能破坏原流程的逻辑，所以需要保护r0-r3的原值。但r4-r12不用压，因为本handler没有改变r4-r12的值，同时根据ATPCS规范，printk也不会改变r4-r12
	push {r0-r3, lr}

	ldr r0, =data_abort_fmt
	bl printk

	//因为异常导致的模式切换，cpu除了填充lr外，还会将cpsr备份到异常模式的spsr中
	//^表示将spsr_abt覆盖回cpsr，由于cpsr[4:0]就是cpu模式位（a8手册P65），所以此时模式将自动发生切换，切回原先的模式
	//pop指令作为伪指令，加上^无效，必须用原生指令
	//pop {r0-r3, pc}
	ldmfd sp!, {r0-r3, pc}^


irq_handler:
	sub lr, #4
	push {r0-r3, lr}

	//do_irq的实现在interrupt.c中
	bl do_irq

	ldmfd sp!, {r0-r3, pc}^

swi_fmt:
	.asciz "swi number = 0x%x\n"
data_abort_fmt:
	.asciz "data_abort\n"