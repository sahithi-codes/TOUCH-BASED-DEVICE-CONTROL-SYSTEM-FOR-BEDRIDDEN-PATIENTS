//delay.h - header file for software delay functions

#ifndef DELAY_H
#define DELAY_H

#include "types.h"               //user defined data types

void delay_us(u32);             //generates delay in microseconds
void delay_ms(u32);             //generates delay in milli seconds
void delay_s(u32);             //generates delay in seconds

#endif
