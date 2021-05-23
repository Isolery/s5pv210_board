#include "s5pv210.h"
#include "led.h"
#include "clock.h"
#include "uart.h"
#include "key.h"
#include "int.h"

#define KEY_DOWN		NUM_EINT2		// down
#define KEY_BACK		NUM_EINT16_31	// gongyong 

void delay(volatile unsigned int i)
{
    while(i--);
}

void system_init(void);

int main(void)
{
    system_init();

    system_init_exception();
	
	eint_init();
	
	// 绑定isr到中断控制器硬件
	intc_setvectaddr(KEY_DOWN, key_isr_eint2);
	intc_setvectaddr(KEY_BACK, key_isr_eint16171819);
	intc_enable(KEY_DOWN);
	intc_enable(KEY_BACK);

    while(1)
    {
        led_control(LED0, ON);
        led_control(LED1, OFF);
        led_control(LED2, ON);
        led_control(LED3, OFF);

        delay(0xFFFFFF);

        led_control(LED0, OFF);
        led_control(LED1, ON);
        led_control(LED2, OFF);
        led_control(LED3, ON);

        delay(0xFFFFFF);

        //printf("I Love Gaoman!\n");
    }

    return 0;
}

void system_init(void)
{
    led_init();
    clock_init();
    uart2_init();
}
