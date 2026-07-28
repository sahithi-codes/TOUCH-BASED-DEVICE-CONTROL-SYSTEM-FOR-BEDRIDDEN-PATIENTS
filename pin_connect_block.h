// pin_connect_block.h - Functions for configuring LPC214x pin functions

#ifndef PIN_CONNECT_BLOCK_H
#define PIN_CONNECT_BLOCK_H

/*
cfgportpinfunc() - Configure the function of a GPIO pin using
            the PINSEL registers.
   PortNo - Port number (currently supports only Port 0)
   PinNo  - Pin number within the port
   fn     - Function selection value

*/
void cfgportpinfunc(int PortNo, int PinNo, int fn);

#endif