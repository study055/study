#define TTB 0x46000000 //页表首地址

.section .text

.global _start
_start:
	//填充异常向量表,arm v7手册P1149
	b reset
	nop
	ldr pc, _swi_handler
	nop
	ldr pc, _data_abort_handler
	nop
	ldr pc, _irq_handler
	nop

_swi_handler:
	.word swi_handler
_data_abort_handler:
	.word data_abort_handler
_irq_handler:
	.word irq_handler

reset:

	//在cp15寄存器中，指定异常向量表的位置在0xffff0000，a8手册P124
	MRC p15, 0, r0, c1, c0, 0
	orr r0, #(1 << 13)
	MCR p15, 0, r0, c1, c0, 0

	//uboot把栈指针定位在虚拟地址，所以在mmu关闭前，需要重新定位到物理地址上
	ldr sp, =0x45000000

	//添加异常向量表的映射,将位于0xffff0000的异常向量表映射到0x40000000

	ldr r0, =TTB
	bl create_translation_table

	//将domain全部设为manager，这样在页表项中就无需填充AP和XN权限管理位，而直接将权限全开
	bl set_domain_all_manager

	//填充页表首地址到TTBR0中，并由TTBCR决定mmu始终使用TTBR0的值作为页表首地址
	ldr r0, =TTB
	bl set_ttb

	bl mmu_on


	//将异常向量表复制到0xffff0000的位置
	ldr r0, =0xffff0000
	ldr r1, =_start
	ldr r2, =(reset - _start)
	bl memcpy


	//设置irq模式下的栈指针寄存器，并打开cpu中断使能
	//将当前模式从svc切换到irq
	mrs r0, cpsr
	//模式位格式a8手册P65
	bic r0, #1
	//打开cpu中的irq使能
	bic r0, #(1 << 7)
	msr cpsr, r0
	//设置irq模式下的sp
	ldr sp, =0x48000000
	//从irq模式切换回svc
	orr r0, #1
	msr cpsr, r0

	bl clear_bss
	bl do_initcall

	//执行swi指令
	swi #123

	//触发data abort
	mov r0, #0
	str r1, [r0]

	bl start_armboot





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


.global irq_off
irq_off:
	push {lr}

	mrs r0, cpsr
	orr r0, #(1 << 7)
	msr cpsr, r0

	pop {pc}

//============mmu===============

.global mmu_off
mmu_off:
	push {lr}

	MRC p15, 0, r0, c1, c0, 0
	bic r0, #1
	MCR p15, 0, r0, c1, c0, 0

	pop {pc}

.global mmu_on
mmu_on:
	push {lr}

	MRC p15, 0, r0, c1, c0, 0
	orr r0, #1
	MCR p15, 0, r0, c1, c0, 0

	pop {pc}

set_domain_all_manager:
	push {lr}

	mvn r0, #0
	MCR p15, 0, r0, c3, c0, 0

	pop {pc}

set_ttb:
	push {lr}

	//将页表首地址写入TTBR0,因为TTBCR.N默认为0，所以mmu始终使用TTBR0作为页表首地址
	MCR p15, 0, r0, c2, c0, 0

	pop {pc}


swi_handler:
	push {lr}

	mrs r1, cpsr
	ldr r0, =fmt_irq
	bl printk

	//将swi指令中的操作数(swi指令的低24位)打印出来，swi指令本身通过lr寄存器来获取。swi跳转时，cpu自动将swi的下一条指令的地址保存在lr寄存器中
	//回到原先的运行过程中
	ldr r0, =fmt_swi
	ldr r1, [lr, #-4]
	ldr r2, =0xffffff
	and r1, r2
	bl printk

	ldmfd sp!, {pc}^

data_abort_handler:
	//发生异常时，模式将自动切换，这里切换成了abort模式，不同模式下的栈指针是独立的
	ldr sp, =0x47000000
	//在发生异常时，lr的值被cpu自动填充为返回地址，但需要修正，arm v7手册P1152
	sub lr, #4

	//r0-r3需要压栈，保护原主程序的运算中间值。如果在这里使用了r4-r12，那么同样也要压栈
	push {r0-r3, lr}


	ldr r0, =fmt_data_abort
	bl printk

	//发生异常时，原模式的程序状态（cpsr）需要保护，原cpsr将自动备份到新模式的spsr中，比如从svc模式切换到abort模式，那么原cpsr将会备份到abort的spsr中

	//尖角号表示出栈同时将spsr恢复到cpsr中，不过此时如果使用pop这个伪指令，那么尖角号将不起作用，这是个编译器bug
	//spsr恢复到cpsr时，除了运算过程的状态被恢复外，还包括了cpu模式位，所以同时，模式也将恢复到原模式
	//pop {r0-r3, pc}^
	ldmfd sp!, {r0-r3, pc}^

irq_handler:
	sub lr, #4
	push {r0-r3, lr}
	bl do_irq //c语言实现do_irq
	ldmfd sp!, {r0-r3, pc}^

.section .rodata

fmt_swi:
	.asciz "swi number = %d\n"
fmt_data_abort:
	.asciz "data abort\n"
fmt_irq:
	.asciz "irq handler, cpsr=0x%x\n"