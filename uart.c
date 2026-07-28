//uart.c

#include <LPC21xx.H>      // LPC21xx microcontroller register definitions
#include "lcd_defines.h"  // LCD command definitions
#include "lcd.h"          // LCD driver function declarations
#include "types.h"        // User-defined data types
#include "delay.h"        // Delay function declarations
#include <string.h>       // Standard string handling functions

// Buffer to store received UART data
char str[23];

// Dummy variable used to clear interrupt status
char dummy;

// Variable to store the received character
char ch;

// Global variables
unsigned int i = 0;       // Index for storing received characters
unsigned int r_flag = 0;  // Flag indicating that a complete string has been received

// Enable UART interrupts (1 = enabled, 0 = disabled)
#define UART_INT_ENABLE 1


// UART0 Interrupt Service Routine (ISR)
void UART0_isr(void) __irq
{
    // Check whether the interrupt is a Receive Data Available interrupt
    if (U0IIR & 0x04)
    {
        // Read received character from UART Receive Buffer Register
        // Reading U0RBR automatically clears the receive interrupt
        ch = U0RBR;

        // Check whether Enter key (Carriage Return) has NOT been pressed
        if (ch != 0x0D)
        {
            // Store received character into buffer
            str[i++] = ch;
        }
        else
        {
            // End of string detected

            // Append null character to make it a valid C string
            str[i] = '\0';

            // Reset index for next reception
            i = 0;

            // Set receive-complete flag
            r_flag = 1;
        }
    }
    else
    {
        // If interrupt is not receive interrupt,
        // read U0IIR to clear transmit interrupt status

            dummy = U0IIR;
    }

    // Inform the VIC that interrupt processing is complete

    VICVectAddr = 0;
}

// Function to initialize UART0
void InitUART0(void)
{
    // Configure P0.0 as TXD0 and P0.1 as RXD0

        PINSEL0 = 0x00000005;

    // Enable access to Divisor Latch registers (DLAB = 1)
    // Configure UART for:
    // - 8 data bits
    // - No parity
    // - 1 stop bit

    U0LCR = 0x83;

    // Set baud rate divisor for 9600 baud
    // (Assuming VPB clock = CCLK/4)

    U0DLL = 97;

    // Disable DLAB while keeping 8-bit, no parity, 1 stop bit configuration

    U0LCR = 0x03;

#if UART_INT_ENABLE > 0

    // Configure UART0 interrupt as IRQ (not FIQ)

    VICIntSelect = 0x00000000;

    // Assign UART0 ISR address to Vector Slot 0

    VICVectAddr0 = (unsigned)UART0_isr;

    // Enable Vector Slot 0 and associate it with UART0 interrupt number (6)
    VICVectCntl0 = 0x20 | 6;

    // Enable UART0 interrupt in the VIC

    VICIntEnable = (1 << 6);

    // Optional FIFO configuration (currently disabled)
    // U0IIR = 0xC0;
    // U0FCR = 0xC7;

    // Enable UART Receive Data Available interrupt
    // and THRE (Transmit Holding Register Empty) interrupt

    U0IER = 0x03;

#endif
}