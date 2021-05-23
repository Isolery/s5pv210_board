#include "sdram.h"

void relocate(void)
{
    extern int __code_start, __bss_start;

	volatile unsigned int *dest = (volatile unsigned int *)&__code_start;
	volatile unsigned int *end = (volatile unsigned int *)&__bss_start;
	volatile unsigned int *src = (volatile unsigned int *)0x30000000;

	while (dest < end)
	{
		*dest++ = *src++;
	}
}
