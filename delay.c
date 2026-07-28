#include "types.h"

//delay_us - generates delay in microseconds

void delay_us(u32 tdly)
{
        tdly*=12;       //multiplies by 12 to approximate a 1-microsecond delay

        while(tdly--); //wait loop until counter reaches zero
}

//delay_ms - generates delay in milliseconds

void delay_ms(u32 tdly)
{
        tdly*=12000;    //multiply by 12000 to convert milliseconds into loop iterations

        while(tdly--);  //wait loop until counter reaches zero
}

//delay_s - generates delay in seconds

void delay_s(u32 tdly)
{
        tdly*=12000000;         //multiply by 12000000 to convert seconds into loop iteration

        while(tdly--);          //wait loop until counter reaches zero
}