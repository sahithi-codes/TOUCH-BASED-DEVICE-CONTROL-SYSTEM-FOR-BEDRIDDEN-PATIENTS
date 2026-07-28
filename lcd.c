// lcd.c - LCD Driver Functions for LPC214x

#include "defines.h"          // Bit manipulation macros
#include "types.h"            // User-defined data types
#include <LPC21xx.h>          // LPC214x register definitions
#include "lcd_defines.h"      // LCD command definitions
#include "delay.h"            // Delay function declarations


/*WriteLCD() - Write one byte (command/data) to the LCD*/

void WriteLCD(u8 data)
{
    // Set RW = 0 (Write operation)
    SCLRBIT(IOCLR0, RW);

    // Place data on LCD data pins
    WRITEBYTE(IOPIN0, LCD_DATA, data);

    // Generate Enable pulse (EN = 1)
    SSETBIT(IOSET0, EN);

    // Small delay for LCD timing
    delay_us(1);

    // EN = 0, LCD latches the data
    SCLRBIT(IOCLR0, EN);

    // Wait for LCD internal processing
    delay_ms(2);
}


/*CmdLCD() - send a command to the LCD */
void CmdLCD(u8 cmd)
{
    // RS = 0 selects Command Register
    SCLRBIT(IOCLR0, RS);

    // Send command byte
    WriteLCD(cmd);
}


/*CharLCD() - Display one character on the LCD */
void CharLCD(u8 ascii)
{
    // RS = 1 selects Data Register
    SSETBIT(IOSET0, RS);

    // Send ASCII character
    WriteLCD(ascii);
}


/* InitLCD() - Initialize the LCD in 8-bit mode */

void InitLCD(void)
{
    // Configure LCD data pins (P0.8 P0.15) as outputs
    WRITEBYTE(IODIR0, LCD_DATA, 0xFF);

    // Configure RS, RW, and EN pins as outputs
    SETBIT(IODIR0, RS);
    SETBIT(IODIR0, RW);
    SETBIT(IODIR0, EN);

    // Wait after power-up                                                                                          
	    delay_ms(15);

    // Initialization sequence as per LCD datasheet
    CmdLCD(MODE_8BIT_1LINE);
    delay_ms(5);

    CmdLCD(MODE_8BIT_1LINE);
    delay_us(100);

    CmdLCD(MODE_8BIT_1LINE);

    // Configure LCD for 8-bit, 2-line mode
    CmdLCD(MODE_8BIT_2LINE);

    // Display ON, Cursor OFF
    CmdLCD(DISP_ON_CUR_OFF);

    // Clear LCD screen
    CmdLCD(CLEAR_LCD);

    // Set cursor move direction to right
    CmdLCD(SHIFT_CUR_RIGHT);
}


/* StrLCD() - Display a string on the LCD */
void StrLCD(u8 *str)
{
    while (*str)
    {
        // Display one character
        CharLCD(*str++);

        // Move to next character
    }
}


/* U32LCD() - Display an unsigned integer on the LCD */
void U32LCD(u32 n)
{
    u8 a[10];
    s32 i = 0;

    // Special case for zero
    if (n == 0)
    {
        CharLCD('0');
    }
    else
    {
        // Extract digits in reverse order
        while (n)
        {
            a[i++] = (n % 10) + '0';
            n /= 10;
        }

        // Display digits in correct order
        for (--i; i >= 0; i--)
        {
            CharLCD(a[i]);
        }
    }
}


/* S32LCD() - Display a signed integer */
void S32LCD(s32 n)
{
    // If negative, print minus sign
    if (n < 0)
    {
        CharLCD('-');

        // Convert to positive
        n = -n;

        // Display number
        U32LCD(n);
    }
}


/* BinLCD() - Display binary representation of a number */
void BinLCD(u32 n, u8 nbd)
{
    s32 i;

    // Print each bit from MSB to LSB
    for (i = (nbd - 1); i >= 0; i--)
    {
        CharLCD(((n >> i) & 1) + '0');
    }
}


/* BuildCGRAM() - Store custom characters in LCD CGRAM */
void BuildCGRAM(u8 *p, u8 nBytes)
{
    s32 i;

    // Select CGRAM address
    CmdLCD(GOTO_CGRAM);

    // Write custom character pattern
    for (i = 0; i < nBytes; i++)
    {
        CharLCD(p[i]);
    }

    // Return cursor to DDRAM
    CmdLCD(GOTO_LINE2_POS0);
}


/* f32LCD() - Display floating-point number */
void f32LCD(f32 fnum, u8 ndp)
{
    u32 inum;

    // Handle negative numbers
    if (fnum < 0.0)
    {
        CharLCD('-');
        fnum = -fnum;
    }

    // Print integer part
    inum = fnum;
    U32LCD(inum);

    // Print decimal point
    CharLCD('.');


    // Print fractional digits
    while (ndp)
    {
        fnum = (fnum - inum) * 10;
        inum = fnum;
        CharLCD(inum + '0');
        ndp--;
    }
}


/* HexLCD() - Display hexadecimal number */
void HexLCD(u32 n)
{
    u8 a[8], rem;
    s32 i = 0;

    if (n == 0)
    {
        CharLCD('0');
    }
    else
    {
        while (n)
        {
            rem = n % 16;

            // Convert digit to ASCII
            (rem < 10) ? (rem += '0') : (rem += 55);

            a[i++] = rem;
            n /= 16;
        }

        // Print in correct order
        for (--i; i >= 0; i--)
        {
            CharLCD(a[i]);
        }
    }
}


/* OctLCD() - Display octal number */
void OctLCD(u32 n)
{
    s32 i = 0;
    u8 a[12];

    if (n == 0)
    {
        CharLCD('0');
    }
    else
    {
        while (n)
        {
            a[i++] = (n % 8) + '0';
            n /= 8;
        }

        // Display digits
        for (--i; i >= 0; i--)
        {
            CharLCD(a[i]);
        }
    }
}


/* DataLCD() - Display a single character */
void DataLCD(char data)
{
    CharLCD(data);
}


/* ScrollTitle() - Scroll project title across the LCD */
void ScrollTitle(void)
{
    // Project title string
    char title[] =
        " TOUCH-BASED DEVICE CONTROL SYSTEM FOR BEDRIDDEN PATIENTS ";

    int i, j;

    // Scroll window across the string
    for (i = 0; title[i + 19] != '\0'; i++)
    {
        // Move cursor to first line
        CmdLCD(0x80);

        // Display 20 visible characters
        for (j = 0; j < 20; j++)
        {
            CharLCD(title[i + j]);
        }

        // Delay to create scrolling effect
        delay_ms(10);
    }
 // Clear LCD after scrolling completes
    CmdLCD(0x01);
}