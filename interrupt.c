//interrupt.c

#include <LPC21xx.h>          // LPC214x microcontroller register definitions
#include <string.h>           // Standard string functions (strncmp, etc.)

#include "interrupt.h"        // External interrupt function declarations
#include "lcd.h"              // LCD driver functions
#include "delay.h"            // Delay functions
#include "spi.h"              // SPI communication functions
#include "KPM.h"              // Keypad module functions
#include "uart.h"             // UART communication functions
#include "types.h"            // User-defined data types

/* Variables declared in other source files */
extern char password[4];      // Stores current password
extern char checker[4];       // Stores password read from EEPROM

extern char temp[4];          // Buffer for entered password
extern char temp1[4];         // Buffer for current PIN verification
extern char new1[4];          // Buffer for new password
extern char new2[4];          // Buffer for confirm password

extern int checkflag;         // Indicates password match/mismatch
extern int highflag;          // Indicates authentication state
extern int enable;            // Touch enable status
extern int buzzer;            // Buzzer status
extern int d1;                // Device 1 status (Fan)
extern int d2;                // Device 2 status (Light)

/* External Interrupt 1 Service Routine */
void eint1_isr(void) __irq
{
    int a;                    // Loop variable
    int int_flag = 0;         // Indicates successful PIN verification
     // Clear LCD screen
    CmdLCD(0x01);

    // Display prompt to enter current PIN
    CmdLCD(0x80);
    StrLCD("Enter Current PIN");

    // Display instructions
    CmdLCD(0x94);
    StrLCD("'*' Enter");

                // Move cursor to second line
    CmdLCD(0xC0);

    CmdLCD(0xD4);
    StrLCD("'A' Backspace");

    // Read current PIN from keypad
    Password_Read(temp1);

    // Small delay after reading
    delay_ms(500);

    // Compare entered PIN with stored PIN
    if (strncmp(temp1, checker, 4) == 0)
    {
        // Correct PIN entered

        CmdLCD(0x01);
        CmdLCD(0x80);
        StrLCD("Unlocked");

        int_flag = 1;         // Mark authentication successful
     delay_s(1);
    }
    else
    {
        // Incorrect PIN

        CmdLCD(0x01);
        CmdLCD(0x80);
        StrLCD("Wrong PIN");

        delay_s(1);

        CmdLCD(0x01);
        CmdLCD(0x80);
        StrLCD("Re-Try");

        delay_s(1);
    }

    // If current PIN is correct, allow password update
    if (int_flag == 1)
    {
        while (1)
        {
            // Reset password mismatch flag
            checkflag = 0;

            // Ask user to enter new password
            CmdLCD(0x80);
            StrLCD("Enter New PIN");

            CmdLCD(0x94);
            StrLCD("'*' Enter");
 CmdLCD(0xD4);
            StrLCD("'A' Backspace");
            CmdLCD(0xC0);

            // Read new password
            Password_Read(new1);

            // Ask user to confirm password
            CmdLCD(0x01);
            CmdLCD(0x80);
            StrLCD("Conirm PIN");

            CmdLCD(0x94);
            StrLCD("'*' Enter");

            CmdLCD(0xD4);
            StrLCD("'A' Backspace");

                                                CmdLCD(0xC0);

            // Read confirmation password
            Password_Read(new2);

            // Compare new password and confirmation password
            for (a = 0; a < 4; a++)
            {
                if (new1[a] != new2[a])
                {
                    // Passwords do not match
                    checkflag = 1;
                }
            }
  // If passwords match
            if (checkflag == 0)
            {
                // Copy new password into password array
                for (a = 0; a < 4; a++)
                {
                    password[a] = new1[a];
                }

                // Display success message
                CmdLCD(0x01);
                CmdLCD(0x80);
                StrLCD("PIN Updated");

                delay_s(1);

                CmdLCD(0x01);

                // Save updated password to EEPROM
                EEPROM_WritePage(0x0000, (u8 *)password, 4);

                // Exit password update loop
                break;
            }
            else
            {
                // Passwords did not match
                CmdLCD(0x01);
                CmdLCD(0x80);
                StrLCD("PIN Not Matched");

                delay_s(1);
            }
   }
    }

    // If password updated successfully
    if (checkflag == 0 && int_flag == 1)
    {
        CmdLCD(0x01);
        CmdLCD(0x80);
        StrLCD("RESTART SYSTEM");

        // Reset all control flags
        highflag = 1;
        enable = 0;
        buzzer = 0;
        d1 = 0;
        d2 = 0;

        // Turn OFF buzzer, fan, and light outputs
        IOCLR0 = (1 << 22) | (1 << 23) | (1 << 25);

        delay_s(1);

        // Prompt user to enter updated PIN
        CmdLCD(0x01);
        CmdLCD(0x80);
        StrLCD("Enter PIN");

        CmdLCD(0x94);
        StrLCD("'*' Enter");

        CmdLCD(0xD4);
        StrLCD("'A' Backspace");
 delay_s(1);

        // Verify newly updated password
        while (1)
        {
            // Read password from keypad
            Password_Read(temp);

            // Reload password from EEPROM
            Password_LoadFromEEPROM();

            // Compare entered password with EEPROM password
            if (strncmp(temp, checker, 4) == 0)
            {
                CmdLCD(0x01);
                CmdLCD(0x80);
                StrLCD("Unlocked");

                delay_s(1);

                // Display all devices in OFF state
                CmdLCD(0x80);
                StrLCD("Touch: OFF");

                CmdLCD(0xC0);
                StrLCD("Buzzer: OFF");

                CmdLCD(0x94);
                StrLCD("Fan: OFF");

                CmdLCD(0xD4);
                StrLCD("Light: OFF");
   break;
            }
            else
            {
                // Prompt user to try again
                CmdLCD(0x01);
                CmdLCD(0x80);
                StrLCD("Re-Enter PIN");

                delay_s(1);
            }
        }
    }

    // Inform VIC that interrupt servicing is complete
    VICVectAddr = 0;

    // Clear External Interrupt 1 pending flag
    EXTINT = (1 << 1);
}

/* Function to configure External Interrupt 1 */
void eint1_enable(void)
{
    // Configure P0.3 as EINT1 function
    PINSEL0 |= (3 << 6);

    // Configure EINT1 as IRQ (not FIQ)
    VICIntSelect &= ~(1 << 15);

    // Enable interrupt channel 15
    VICIntEnable |= (1 << 15);
   // Enable vector slot 1 and assign interrupt number 15
    VICVectCntl1 = (1 << 5) | 15;

    // Set ISR address in vector slot 1
    VICVectAddr1 = (unsigned int)(eint1_isr);

    // Configure EINT1 as edge-triggered interrupt
    EXTMODE |= (1 << 1);

    // Configure EINT1 to trigger on falling edge
    EXTPOLAR &= ~(1 << 1);
}