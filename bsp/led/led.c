#include "led.h"

#define GPJ0CON	    *((volatile unsigned int *)(ELFIN_GPIO_BASE + GPJ0CON_OFFSET))
#define GPJ0DAT	    *((volatile unsigned int *)(ELFIN_GPIO_BASE + GPJ0DAT_OFFSET))
#define GPJ0PUD 	*((volatile unsigned int *)(ELFIN_GPIO_BASE + GPJ0PUD_OFFSET))

#define GPD0CON	    *((volatile unsigned int *)(ELFIN_GPIO_BASE + GPD0CON_OFFSET))
#define GPD0DAT	    *((volatile unsigned int *)(ELFIN_GPIO_BASE + GPD0DAT_OFFSET))
#define GPD0PUD 	*((volatile unsigned int *)(ELFIN_GPIO_BASE + GPD0PUD_OFFSET))

void led_init(void)
{
	// 设置GPJ0_3, GPJ0_4, GPJ0_5, GPD0_1为输出引脚
	GPJ0CON |= (1 << 3*4) | (1 << 4*4) | (1 << 5*4);
	GPD0CON |= (1 << 1*4);

	// 上拉
	GPJ0PUD &= ~((3 << 3*2) | (3 << 4*2) | (3 << 5*2));
	GPD0PUD &= ~(3 << 1*2);

	GPJ0PUD |= (2 << 3*2) | (2 << 4*2) | (2 << 5*2);
	GPD0PUD |= (2 << 1*2);
}

void led_control(unsigned char lednum, unsigned char flag)
{
	if(flag == ON)
	{
		if(lednum != LED3)
			GPJ0DAT &= ~(1 << lednum);
		else
			GPD0DAT &= ~(1 << lednum);
	}

	else if(flag == OFF)
	{
		if(lednum != LED3)
			GPJ0DAT |= (1 << lednum);
		else
			GPD0DAT |= (1 << lednum);
	}
}
