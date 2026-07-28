//project_main.c

#include <LPC21xx.H>          // LPC214x microcontroller register definitions
#include <string.h>           // String handling functions like strncmp()
#include "lcd_defines.h"      // LCD command definitions
#include "lcd.h"              // LCD driver functions
#include "delay.h"            // Delay functions (ms and sec)
#include "types.h"            // User-defined data types
#include "spi.h"              // SPI communication functions
#include "KPM.h"              // Keypad module functions
#include "uart.h"             // UART communication functions
#include "interrupt.h"        // Interrupt configuration functions
#include "touch.h"            // Touch panel related functions

#define PASSWORD_LEN 4        // Password length is fixed to 4 characters

// External variables defined in other source files
extern char str[23], dummy, ch;

// Buffers to store X, Y, and Z coordinate strings
char x_buf[10];
char y_buf[10];
char z_buf[10];

// Password arrays stored in other files
extern char password[4], checker[4];

// Temporary arrays for password handling
char temp[4], temp1[4], new1[4], new2[4];

// External variables used for interrupt/status handling
extern unsigned int i, r_flag;

// Flags used in the application
int checkflag = 0;    // Indicates password check status
int highflag = 0;     // Prevents repeated password verification

// Variables for device control states
int enable = 0;
int buzzer = 0;
int d1 = 0;
int d2 = 0;

// Variables to store previous touch coordinates
int prev_x = -1, prev_y = -1;

// Flag to indicate whether current touch has been processed
int touch_processed = 0;

int main()
{
    int m = 1;    // Variable used to control password authentication loop

    // Configure P0.25 (buzzer), P0.22 (device2), P0.23 (device1) as outputs
    IODIR0 |= (1 << 25) | (1 << 22) | (1 << 23);

    // Initialize UART communication
    InitUART0();

    // Initialize LCD module
    InitLCD();

    // Initialize keypad
    InitKPM();

    // Initialize SPI interface
    Init_SPI();


    // Display scrolling title on LCD
    ScrollTitle();

    // Enable external interrupt (used by touch controller)
    eint1_enable();

    // Uncomment below to write default password into EEPROM
    // EEPROM_WritePage(0x0000, "7989", PASSWORD_LEN);

    // Load saved password from EEPROM into memory
    Password_LoadFromEEPROM();

    // Wait for EEPROM operation to complete
    delay_s(1);

    /*
    // Debug code to display EEPROM password on LCD
    cmdLCD(0x01);
    cmdLCD(0x80);
    strLCD((u8 *)password);
    delay_s(1);
    */

    // Clear LCD
    CmdLCD(0x01);

    // Move cursor to first line
    CmdLCD(0x80);

    // Check whether touch controller is detected
    if (r_flag)
    {
        // If touch panel responds properly
        if (str[0] != '\0')
        {
            StrLCD("Touch Panel Ready");
            delay_s(1);
        }
    }
    else
    {
        // If touch panel initialization failed
        StrLCD("Touch Not Found");
        delay_s(1);
    }

    // Initially turn OFF Device1(LED) and Device2(FAN) outputs
    IOCLR0 = (1 << 23) | (1 << 22);

    // Display PIN entry instructions
    CmdLCD(0x01);
    CmdLCD(0x80);
    StrLCD("ENTER PIN");

    CmdLCD(0x94);
    StrLCD("'*' Enter");      // '*' key confirms password

    CmdLCD(0xD4);
    StrLCD("'A' Backspace");  // 'A' key deletes previous digit

    delay_s(1);

    // Main program loop
    while (1)
    {
        // Password verification loop
        while (m == 1)
        {
            // Perform authentication only once
            if (highflag == 0)
            {
                // Read password entered by user
                Password_Read(temp);

                // Reload stored password from EEPROM
                Password_LoadFromEEPROM();

                // Compare entered password with stored password
                if (strncmp(temp, checker, PASSWORD_LEN) == 0)
                {
                    // Password matched

                    m = 0;                 // Exit authentication loop
                    highflag = 1;          // Prevent future authentication

                    // Display success message
                    CmdLCD(0x01);
                    CmdLCD(0x80);
                    StrLCD("Access Granted");

                    CmdLCD(0x01);
                    delay_s(2);

                    // Display initial status of all devices
                    CmdLCD(0x80);
                    StrLCD("Control: OFF");

                    CmdLCD(0xC0);
                    StrLCD("Buzzer: OFF");

                    CmdLCD(0x94);
 StrLCD("Fan: OFF");

                    CmdLCD(0xD4);
                    StrLCD("Light: OFF");

                    // Exit password loop
                    break;
                }
                else
                {
                    // Password mismatch
                    CmdLCD(0x01);
                    CmdLCD(0x80);
                    StrLCD("Invalid PIN");

                    // Wait before asking again
                    delay_s(1);
                }
            }
        }


        delay_ms(500);// Small delay before touch processing


        while (1)       // Infinite loop to continuously process touch events
        {
            Touch_Process();   // Handle touch inputs and device control
        }
    }
}