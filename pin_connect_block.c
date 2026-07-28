// pin_connect_block.c - Functions for configuring LPC214x pin functions

#include <LPC21xx.h>    // Include LPC214x register definitions

/*
cfgportpinfunc() - Configure the function of a GPIO pin using
            the PINSEL registers.
   PortNo - Port number (currently supports only Port 0)
   PinNo  - Pin number within the port
   fn     - Function selection value

*/
void cfgportpinfunc(int PortNo, int PinNo, int fn)
{
    // Check whether the selected port is Port 0
    if (PortNo == 0)
    {
        // Pins P0.0 to P0.15 are controlled by PINSEL0
        if (PinNo < 16)
        {
            // Clear the existing 2-bit function field
            // and write the new function value.
            PINSEL0 =
                ((PINSEL0 & ~(3 << (PinNo * 2))) |
                 (fn << (PinNo * 2)));
        }
        else
        {
            // Pins P0.16 to P0.31 are controlled by PINSEL1

            // Convert pin number to PINSEL1 bit position,
            // clear existing function bits,
            // and write the new function value.
            PINSEL1 =  ((PINSEL1 & ~(3 << ((PinNo - 16) * 2))) |(fn << ((PinNo - 16) * 2)));
        }
    }
    else
    {
        // No action taken for unsupported ports.
        // (Currently only Port 0 configuration is implemented.)
    }
}
