//touch.c

#include <LPC21xx.h>
#include <string.h>

#include "touch.h"
#include "lcd.h"
#include "uart.h"
#include "delay.h"

// External state and buffer variables
extern char uart_rx_buf[23];
extern char x_str[10], y_str[10], z_str[10];
extern unsigned int uart_idx;
extern unsigned int rx_complete;
extern int sys_enabled;
extern int buzzer_active;
extern int fan_active;
extern int light_active;
extern int last_x;
extern int last_y;
extern int is_new_touch;

void Touch_Process(void)
{
    int idx, sub_idx;
    int x_val, y_val, z_val;

    // Reset UART reception state
    rx_complete = 0;
    uart_idx = 0;

    while (rx_complete == 0); // Wait for UART frame
    delay_s(1);              // Frame stability delay
    // Extract X string
    for (idx = 0; idx < 6; idx++)
        x_str[idx] = uart_rx_buf[idx];
    x_str[idx] = '\0';

    // Extract Y string
    for (sub_idx = 0, idx = 8; idx < 13; idx++, sub_idx++)
        y_str[sub_idx] = uart_rx_buf[idx];
    y_str[sub_idx] = '\0';

    // Extract Z string
    for (sub_idx = 0, idx = 15; idx < 20; idx++, sub_idx++)
        z_str[sub_idx] = uart_rx_buf[idx];
    z_str[sub_idx] = '\0';

    // Clear flags for next transfer
    rx_complete = 0;
    uart_idx = 0;

    // Convert ASCII to integer values
    x_val = ((x_str[4] - '0') * 10 + (x_str[5] - '0'));
    y_val = ((y_str[3] - '0') * 10 + (y_str[4] - '0'));
    z_val = ((z_str[3] - '0') * 10 + (z_str[4] - '0'));

    // Check panel touch (pressure threshold)
    if (z_val < 15)
    {
        if (last_x != x_val || last_y != y_val)
        {
            last_x = x_val;
            last_y = y_val;
            is_new_touch = 1; // Valid new touch detected
        }
    }
    // Handle touch actions
    if (is_new_touch == 1)
    {
        is_new_touch = 0; // Consume event

        // Zone 1: Toggle System Enable
        if ((x_val < 11) && (y_val >= 14))
        {
            sys_enabled ^= 1;
        }

        // Zone 2: Toggle Buzzer

        else if ((x_val > 11) && (y_val >= 14))
        {
            if (sys_enabled)
            {
                buzzer_active ^= 1;
                if (buzzer_active)
                    IOSET0 = (1 << 25); // P0.25 HIGH
                else
                    IOCLR0 = (1 << 25); // P0.25 LOW
            }
        }
        // Zone 3: Fan ON

        else if ((x_val < 11) && (y_val < 14 && y_val >= 8))
        {
            if (sys_enabled)
            {
                fan_active = 1;
                IOSET0 = (1 << 23);     // P0.23 HIGH
            }
        }
        // Zone 3: Fan OFF

        else if ((x_val > 11) && (y_val < 14 && y_val >= 8))
        {
            if (sys_enabled)
            {
                fan_active = 0;
                IOCLR0 = (1 << 23);     // P0.23 LOW
            }
        }
        // Zone 4: Light ON
        else if ((x_val < 11) && (y_val < 8))
        {
            if (sys_enabled)
            {
                light_active = 1;
                IOSET0 = (1 << 22);     // P0.22 HIGH
            }
        }
        // Zone 4: Light OFF
        else if ((x_val > 11) && (y_val < 8))
        {
            if (sys_enabled)
            {
                light_active = 0;
                IOCLR0 = (1 << 22);     // P0.22 LOW
            }
        }
    }

    // Reset coordinates on touch release
    if (z_val >= 15)
    {
        last_x = -1;
	 last_y = -1;
        is_new_touch = 0;
    }

    // Refresh LCD status display
    CmdLCD(0x01); // Clear display

    if (sys_enabled)
        StrLCD("Control: ON");
    else
        StrLCD("Control: OFF");

    CmdLCD(0xC0); // Move to line 2

    if (buzzer_active)
        StrLCD("Buzzer: ON");
    else
        StrLCD("Buzzer: OFF");

    CmdLCD(0x94); // Move to line 3

    if (fan_active)
        StrLCD("Fan: ON");
    else
        StrLCD("Fan: OFF");

    CmdLCD(0xD4); // Move to line 4

    if (light_active)
        StrLCD("Light: ON");
    else
        StrLCD("Light: OFF");
}
                                                                                                                                 