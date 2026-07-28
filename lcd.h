//lcd.h


void S32LCD(s32 n);


/* BinLCD() Displays a number in binary format.
 n   - Number to display.
 nbd - Number of bits to display*/

void BinLCD(u32 n, u8 nbd);

/*BuildCGRAM() Creates custom characters in the LCD's CGRAM.
p      - Pointer to character pattern data.
nBytes - Number of bytes to write*/

void BuildCGRAM(u8 *p, u8 nBytes);


//f32LCD() - Displays a floating-point number.

void f32LCD(f32 fnum, u8 ndp);

//OctLCD() - Displays a number in octal format.

void OctLCD(u32 n);

//HexLCD() - Displays a number in hexadecimal format.

void HexLCD(u32 n);

//ScrollTitle() - Scrolls a predefined text message across the LCD display.
void ScrollTitle(void);

#endif   // End of LCD_H include guard

