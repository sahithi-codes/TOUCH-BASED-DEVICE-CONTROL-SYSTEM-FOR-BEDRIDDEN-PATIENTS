//lcd_defines.h         //lcd pin definitions

#include "types.h"
#include<lpc214x.h>

#define LCD_DATA                 8              //LCD_DATA pins from P0.8 - P0.15
#define RS                      16              //RS pin for port 0 pin 16 (1<<16)
#define EN                      17              //EN pin for port 0 pin 17 (1<<17)
#define RW                      18              //RW pin for port 0 pin 18 (1<<18)
#define LCD_CLEAR               0x01            //clear display and return to cursor home
#define LCD_HOME                0x80            //return curosr to home position
#define MODE_4BIT_1LINE         0x20            //4 bit mode 1 line display
#define MODE_4BIT_2LINE         0x28            //4 bit mode 2 line display
#define MODE_8BIT_1LINE         0x30            //8 bit mode 1 line display
#define MODE_8BIT_2LINE         0x38            //8 bit mode 2 line display
#define Disp_OFF                0x08            //display off
#define Disp_ON_CUR_OFF         0x0C            //display on cursor off
#define Disp_ON_CUR_ON          0x0E            //display on cursor on
#define Disp_ON_CUR_BLINK       0x0F            //display on cursor blink
#define GOTO_LINE1_POS0         0x80            //move curoser to line 1 position 0
#define GOTO_LINE2_POS0         0xC0            //move curoser to line 2 position 0
#define GOTO_CGRAM              0x40            //set cgram address
#define SHIFT_CUR_RIGHT         0x06            //increment cursor position after each character


