//Keyad.h - Header file for 4x4 Matrix Keypad functions

#ifndef KEYPAD_H
#define KEYPAD_H

#include "types.h"      //Include user-defined data types(u8,u32)
void InitKPM(void);     //Initializes the keypad by configuring the row pins as outputs
void ColScan(void);     //reads all column pins Detect whether any key is pressed or not
void RowCheck(void)     // Determines which keypad row contains the pressed key
void ColCheck(void)     // Determines which keypad column contains the pressed key
void KeyScan(void)      // Detects a key press and returns its value,returns ASCII value of the pressed key
void ReadNum(void)      //Read a multi-digit number from the keypad.Numeric Keys ('0' - '9') are accepted.

#endif
