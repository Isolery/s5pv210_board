#include "clock.h"

#define APLL_LOCK  *((volatile unsigned int *)(APLL_LOCK_OFFSET + ELFIN_CLOCK_POWER_BASE))
#define MPLL_LOCK  *((volatile unsigned int *)(MPLL_LOCK_OFFSET + ELFIN_CLOCK_POWER_BASE))
#define APLL_CON0  *((volatile unsigned int *)(APLL_CON0_OFFSET + ELFIN_CLOCK_POWER_BASE))
#define MPLL_CON0  *((volatile unsigned int *)(MPLL_CON_OFFSET + ELFIN_CLOCK_POWER_BASE))
#define CLK_SRC0   *((volatile unsigned int *)(CLK_SRC0_OFFSET + ELFIN_CLOCK_POWER_BASE))
#define CLK_DIV0   *((volatile unsigned int *)(CLK_DIV0_OFFSET + ELFIN_CLOCK_POWER_BASE))

void clock_init(void)
{
	// 设置LOCKTIME
    APLL_LOCK = 0x0FFFF;
    MPLL_LOCK = 0x0FFFF;

    // 设置分频
    CLK_DIV0 = (1 << 28) | (4 << 24) | (1 << 20) | (3 << 16) | (1 << 12) | (4 << 8) | (4 << 4);

    // 设置PLL
    APLL_CON0 = (1 << 31) | (125 << 16) | (3 << 8) | (1 << 0);
    MPLL_CON0 = (1 << 31) | (667 << 16) | (12 << 8) | (1 << 0);

    // 设置MUX
    CLK_SRC0 = (1 << 28) | (1 << 12) | (1 << 8) | (1 << 4) | (1 << 0);
}
