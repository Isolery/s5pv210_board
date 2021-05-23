#ifndef KEY_H
#define KEY_H

#include "s5pv210.h"
#include "uart.h"
#include "int.h"

void eint_init(void);
void key_isr_eint2(void);
void key_isr_eint3(void);
void key_isr_eint16171819(void);

#endif