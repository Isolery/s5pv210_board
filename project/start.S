#define WTCON		0xE2700000
#define SVC_STACK	0xd0037d80

.text
.global _start

_start:

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
	ldr sp, =0x31000000

	ldr pc, =main				// ldr指令实现长跳转
	
// 汇编最后的这个死循环不能丢
	b .
