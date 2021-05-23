#define WTCON		0xE2700000
#define SVC_STACK	0xd0037d80

.text
.global _start
.global IRQ_handle	

_start:
	b  reset

IRQ_handle:
	/* 执行到这里之前:
	 * 1. lr_irq保存有被中断模式中的下一条即将执行的指令的地址
	 * 2. SPSR_irq保存有被中断模式的CPSR
	 * 3. CPSR中的M4-M0被设置为10010, 进入到irq模式
	 * 4. 跳到0x18的地方执行程序 
	 */

	/* sp_irq未设置, 先设置它 */
	ldr sp, =0x32000000

	/* 保存现场 */
	/* 在irq异常处理函数中有可能会修改r0-r12, 所以先保存 */
	/* lr-4是异常处理完后的返回地址, 也要保存 */
	sub lr, lr, #4
	stmdb sp!, {r0-r12, lr}  
	
	/* 处理irq异常 */
	bl irq_handler
	
	/* 恢复现场 */
	ldmia sp!, {r0-r12, pc}^  /* ^会把spsr_irq的值恢复到cpsr里 */

reset:
// 第1步：关看门狗（向WTCON的bit5写入0即可）
	ldr r0, =WTCON
	ldr r1, =0x0
	str r1, [r0]

	// 第2步：设置SVC栈
	ldr sp, =SVC_STACK
	
	// 第3步：开/关icache
	mrc p15,0,r0,c1,c0,0;			// 读出cp15的c1到r0中
	//bic r0, r0, #(1<<12)			// bit12 置0  关icache
	orr r0, r0, #(1<<12)			// bit12 置1  开icache
	mcr p15,0,r0,c1,c0,0;

	// 第4步：初始化ddr
	//bl sdram_asm_init
	
	// 第5步：重定位
	//bl relocate

	// 清bss段，其实就是在链接地址处把bss段全部清零
clean_bss:
	ldr r0, =__bss_start					
	ldr r1, =__bss_end
	cmp r0, r1				// 如果r0等于r1，说明bss段为空，直接下去
	beq run_on_dram			// 清除bss完之后的地址
	mov r2, #0
clear_loop:
	str r2, [r0], #4		// 先将r2中的值放入r0所指向的内存地址（r0中的值作为内存地址），
	cmp r0, r1				// 然后r0 = r0 + 4
	bne clear_loop

run_on_dram:	
	// 长跳转到led_blink开始第二阶段

	/* 复位之后, cpu处于svc模式
	 * 现在, 切换到usr模式
	 */
	mrs r0, cpsr         /* 读出cpsr */
	bic r0, r0, #0xf     /* 修改M4-M0为0b10000, 进入usr模式 */
	bic r0, r0, #(1<<7)  /* 清除I位, 使能中断 */
	msr cpsr, r0

	/* 设置 sp_usr */
	ldr sp, =0x31000000

	ldr pc, =main				// ldr指令实现长跳转
	
// 汇编最后的这个死循环不能丢
	b .
