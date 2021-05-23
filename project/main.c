#include "s5pv210.h"
#include "led.h"
#include "clock.h"
#include "uart.h"

void delay(volatile unsigned int i)
{
    while(i--);
}

void system_init(void);

int main(void)
{
    system_init();

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

        printf("I Love Gaoman!\n");
    }

    return 0;
}

void system_init(void)
{
    led_init();
    clock_init();
    uart2_init();
}
