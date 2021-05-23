#include "uart.h"

#define GPA1CON		*((volatile unsigned int *)(ELFIN_GPIO_BASE + GPA1CON_OFFSET))
#define ULCON2 		*((volatile unsigned int *)(ELFIN_UART_BASE + ULCON2_OFFSET))
#define UCON2 		*((volatile unsigned int *)(ELFIN_UART_BASE + UCON2_OFFSET))
#define UMCON2 		*((volatile unsigned int *)(ELFIN_UART_BASE + UMCON2_OFFSET))
#define UFCON2 		*((volatile unsigned int *)(ELFIN_UART_BASE + UFCON2_OFFSET))
#define UBRDIV2 	*((volatile unsigned int *)(ELFIN_UART_BASE + UBRDIV2_OFFSET))
#define UDIVSLOT2	*((volatile unsigned int *)(ELFIN_UART_BASE + UDIVSLOT2_OFFSET))
#define UTRSTAT2	*((volatile unsigned int *)(ELFIN_UART_BASE + UTRSTAT2_OFFSET))
#define UTXH2		*((volatile unsigned int *)(ELFIN_UART_BASE + UTXH2_OFFSET))	 
#define URXH2		*((volatile unsigned int *)(ELFIN_UART_BASE + URXH2_OFFSET))	

void uart2_init(void)
{
    // 配置GPIO引脚为TX, RX
    GPA1CON &= ~(0xFF <<0);
    GPA1CON |= (2 << 0) | (2 << 4);

    // 8,N,1,None 
    ULCON2 = (3 << 0) | (0 << 2) | (0 << 3) | (0 << 6);

    // 设置时钟源为PCLK_PSYS = 66.7MHz
    // 发生和接收均常用中断或查询模式
    UCON2 = (1 << 0) | (1 << 2) | (0 << 10);

    // 禁止FIFO, 禁止流控
    UMCON2 = 0;
    UFCON2 = 0;

    // 波特率115200
    UBRDIV2 = 35;
    UDIVSLOT2 = 0x0888;
}

void putc(unsigned char c)
{
    while (!(UTRSTAT2 & (1<<1)));
	UTXH2 = c;
}

void putchar(unsigned char c)
{
    while (!(UTRSTAT2 & (1<<1)));
	UTXH2 = c;
}

unsigned char getchar(void)
{
    while (!(UTRSTAT2 & (1<<0)));
    return (URXH2 & 0x0f);
}

int raise(void)
{
    return 0;
}