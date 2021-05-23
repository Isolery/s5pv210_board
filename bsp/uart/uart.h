#ifndef UART_H
#define UART_H

#include "stdio.h"
#include "s5pv210.h"

void uart2_init(void);
void putc(unsigned char c);
void putchar(unsigned char c);
unsigned char getchar(void);
int raise(void);

#endif