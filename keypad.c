//KPM.c

#include "defines.h"        //Bit manipulation macros
#include<LPC21xx.h>         //LPc214x register definitions
#include "types.h"          //user defined data types
#include "lcd.h"            //LCD functions
#include "keypad_defines.h" //keypad pin defines

//4x4 keypad lookup table

/*
        col0    col1    col2    col3
row0     1       2       3       +
row1     4       5       6       -
row2     7       8       9       *
row3     C       0       =       /
*/

u8 KpmLUT[4][4]=
{
        {'1','2','3','+'},
        {'4','5','6','-'},
        {'7','8','9','*'},
        {'C','0','=','/'}
}

//InitKPM() - Configure keypad row pins as outputs
void InitKPM(void)
{
        //configure P1.16 to P1.19(ROW0 to ROW3) as output pin
        WRITENIBBLE(IODIR1,ROW0,15);
}

//ColScan() - reads all column pins
//Detect whether any key is pressed or not (0-key pressed;1-no key pressed)
void ColScan(void)
{
        //if any column becomes low a key is pressed
        if((READNIBBLE(IOPIN1,COL0))<15)
        {
                return 0;
        }
        else
        {
                return 1;
        }
}

//RowCheck() - Determines which keypad row contains the pressed key
void RowCheck(void)
{
        u8 rno;
        //check each row one by one
        for(rno=0;rno<=3;rno++)
        {
                //one row LOW while keeping other HIGH
                WRITENIBBLE(IOPIN1,ROW0,~(1<<rno));
                //If a column becomes active,key is in this row
                if(ColScan()==0)
                {
                        break;
                }

        }
        //Restore all rows to inactive state
        WRITENIBBLE(IOPIN1,ROW0,0x0);
        //Return detected row number
        return rno;
}

//ColCheck() - Determines which keypad column contains the pressed key
u8 ColCheck(void)
{
        u8 cno;

        //check each column individually
        for(cno=0;cno<=3;cno++)
        {
                //active LOW colum indicates pressed key
                if(STATUSBIT(IOPIN1,(COL0+cno))==0)
                {
                        break;
                }
        }
        //return detected column number
        return cno;
}

//KeyScan() - Detects a key press and returns its value,returns ASCII value of the pressed key
u8 KeyScan(void)
{
        u8 keyv;        //stores final key value
        u8 rno;         //Row number
        u8 cno;         //Column number

        //wait until any key is pressed

        while(ColScan());

        //identify the row containing the pressed key

        rno=RowCheck();

        //Identify the column containing the pressed key

        cno=ColCheck();

        //get character from lookup table

        keyv=kpmLUT[rno][cno];

        //wait until key is released

        while(!ColScan());

        //Return detected key
        return keyv;
}
//ReadNum() - Read a multi-digit number from the keypad.Numeric Keys ('0' - '9') are accepted.
              //Press 'C' to finish input
u32 ReadNum(void)
{
        u8 key;         //stores current key
        u32 sum=0;      //Accumulates entered number

        while(1)
        {
                //Read one key from keypad
  key=KeyScan();

                //check if key is a digit
                if((key>='0') && (key<='9'))
                {
                        sum=(sum*10)+(key-'0'); //converts string to decimal
                }

                //stop reading when 'C' (confirm) is pressed
                else if(key=='C')
                {
                        break;
                }
        }
        //Return the entered number
        return sum;
}