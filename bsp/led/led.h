#ifndef LED_H
#define LED_H

#include "s5pv210.h"

#define LED0    3
#define LED1    4
#define LED2    5
#define LED3    1

#define ON      1
#define OFF     0

void led_init(void);
void led_control(unsigned char lednum, unsigned char flag);

#endif
