#include <LPC21xx.h>              // LPC214x microcontroller register definitions
#include "spi_defines.h"          // SPI command and constant definitions
#include "pin_connect_block.h"    // Pin configuration functions
#include "lcd.h"                  // LCD driver functions
#include "KPM.h"                  // Keypad module functions
#include "delay.h"                // Delay functions
#include "types.h"                // User-defined data types

// Length of the password stored in EEPROM
#define PASSWORD_LEN 4

// Arrays to store password and verification password
char password[4], checker[4];

// Variables used in old password input function
int o, oflag;


//Init_SPI() - Initialize SPI interface for EEPROM

void Init_SPI(void)
{
    // Configure P0.4 as SPI Clock (SCLK)
    cfgportpinfunc(0, 4, 1);

    // Configure P0.5 as SPI MISO (Master In Slave Out)
    cfgportpinfunc(0, 5, 1);

    // Configure P0.6 as SPI MOSI (Master Out Slave In)
    cfgportpinfunc(0, 6, 1);

    // Make Chip Select (CS) HIGH (EEPROM inactive)
    IOSET0 = 1 << CS;

    // Configure CS pin as output
    IODIR0 |= 1 << CS;

    // Set SPI clock divider
    S0SPCCR = LOADVAL;

    // Configure SPI in Master Mode and Mode-3
    S0SPCR = (1 << MSTR) | (3 << CPHA);
}

//spi() - Send one byte through SPI and receive one byte

u8 spi(u8 sDat)
{
    // Load transmit data into SPI Data Register
    S0SPDR = sDat;

    // Wait until SPI transfer is complete
    while (((S0SPSR >> SPIF) & 1) == 0);

    // Return received byte
    return S0SPDR;
}

//Enable EEPROM Write Operation

void WriteEnable(void)
{
    // Select EEPROM
    IOCLR0 = 1 << CS;

    // Send Write Enable command (WREN)
    spi(WREN);
	
    // Deselect EEPROM
    IOSET0 = 1 << CS;
}

// Disable EEPROM Write Operation

void WriteDisable(void)
{
    // Select EEPROM
    IOCLR0 = 1 << CS;

    // Send Write Disable command (WRDI)
    spi(WRDI);

    // Deselect EEPROM
    IOSET0 = 1 << CS;
}

//Write one byte into EEPROM

void Spi_eeprom_bytewrite(u16 wBuffAddr, u8 sDat)
{
    // Enable writing
    WriteEnable();

    // Select EEPROM
    IOCLR0 = 1 << CS;

    // Send WRITE command
    spi(WRITE);

    // Send high byte of address
    spi(wBuffAddr >> 8);
    // Send low byte of address
    spi(wBuffAddr);

    // Send data byte
    spi(sDat);

    // Deselect EEPROM
    IOSET0 = 1 << CS;

    // Wait for internal write completion
    delay_ms(5);

    // Disable writing
    WriteDisable();
}

//Read one byte from EEPROM

u8 Spi_eeprom_byteread(u16 rBuffAddr)
{
    u8 rDat;

    // Select EEPROM
    IOCLR0 = 1 << CS;

    // Send READ command
    spi(READ);

    // Send address MSB
    spi(rBuffAddr >> 8);

    // Send address LSB
    spi(rBuffAddr);

    // Read data by sending dummy byte
    rDat = spi(0x00);

    // Deselect EEPROM
    IOSET0 = 1 << CS;

    // Return received byte
    return rDat;
}

 //Write multiple bytes to EEPROM

void EEPROM_WritePage(u16 wbuffstartaddr, u8 *p, u8 nbytes)
{
    u8 i;

    // Enable EEPROM write operation
    WriteEnable();

    // Select EEPROM
    IOCLR0 = 1 << CS;

    // Send WRITE command
    spi(WRITE);

    // Send starting address
    spi(wbuffstartaddr >> 8);
    spi(wbuffstartaddr);

    // Write all bytes sequentially
    for(i = 0; i < nbytes; i++)
    {
        spi(p[i]);
    }

    // Deselect EEPROM
    IOSET0 = 1 << CS;

    // Wait until EEPROM completes writing
    delay_ms(5);

    // Disable writing
    WriteDisable();
}

//Sequential EEPROM Read

void Spi_eeprom_seqread(u16 rbuffaddr, u8 *p, u8 nbytes)
{
    u8 i;

    // Select EEPROM
    IOCLR0 = 1 << CS;

    // Send READ command
    spi(READ);

    // Send starting address
    spi(rbuffaddr >> 8);
    spi(rbuffaddr);

    // Read requested number of bytes
    for(i = 0; i < nbytes; i++)
    {
        p[i] = spi(0x00);     // Dummy write to receive data
    }

    // Deselect EEPROM
    IOSET0 = 1 << CS;
}

// Load Password from EEPROM

void Password_LoadFromEEPROM()
{
    // Read password into password array
    Spi_eeprom_seqread(0x0000, (u8 *)password, PASSWORD_LEN);

    delay_ms(5);

    // Read same password into checker array
    Spi_eeprom_seqread(0x0000, (u8 *)checker, PASSWORD_LEN);
}


/* Read Password from Keypad
 * '+' -> Backspace
 * 'C' -> Confirm/Enter */
void Password_Read(char *str)
{
    char key;      // Stores key pressed
    int pos = 0;   // Current password position

    // Move LCD cursor to second line
    CmdLCD(0xC0);

    while(1)
    {
        // Scan keypad
        key = KeyScan();

        // Backspace key pressed
        if(key == '+')
        {
            if(pos > 0)
            {
                pos--;

                // Remove previous character
                str[pos] = '\0';

                // Erase '*' from LCD
                CmdLCD(0xC0 + pos);
                CharLCD(' ');

                // Move cursor back
                CmdLCD(0xC0 + pos);
            }
        }

        // Enter key pressed
        else if(key == 'C')
        {
            // Accept only after 4 digits entered
            if(pos == 4)
            {
                break;
            }
        }

        // Numeric key pressed
        else if(key >= '0' && key <= '9')
        {
            if(pos < 4)
            {
                // Store digit
		  str[pos] = key;

                // Display '*' instead of actual digit
                CmdLCD(0xC0 + pos);
                CharLCD('*');

                pos++;
            }
        }

        // Ignore invalid keys
        else
        {
            continue;
        }

        // Debounce delay
        delay_ms(200);
    }

    // Null terminate password string
    str[4] = '\0';
}
                                                                
