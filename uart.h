//uart.h

#ifndef __UART_H__
#define __UART_H__

#include <LPC21xx.h>

/* Global variables */
extern unsigned int i,r_flag;

extern char str[23],dummy,ch;
/* UART function prototypes */

void InitUART0(void);

void UART0_isr(void) __irq;

#endif

