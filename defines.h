//defines.h

/* Bit Manipulation Macros */

#define SETBIT(WORD, BP)            (WORD |= (1 << BP))                                         // Set bit at position BP
#define CLRBIT(WORD, BP)            (WORD &= ~(1 << BP))                                        // Clear bit at position BP
#define STATUSBIT(WORD, BP)         ((WORD >> BP) & 1)                                          // Read status of bit at BP (returns 0 or 1)
#define WRITEBIT(WORD, BP, BIT)     (WORD = ((WORD & ~(1 << BP)) | (BIT << BP)))                // Write 0 or 1 to bit at BP
#define WRITENIBBLE(WORD, SBP, NIB) (WORD = ((WORD & ~(0xF << SBP)) | (NIB << SBP)))            // Write 4-bit nibble starting at SBP
#define WRITEBYTE(WORD, SBP, BYTE)  (WORD = ((WORD & ~(0xFF << SBP)) | (BYTE << SBP)))          // Write 8-bit byte starting at SBP
#define WRITEHWORD(WORD, SBP, HW)   (WORD = ((WORD & ~(0xFFFF << SBP)) | (HW << SBP)))          // Write 16-bit half-word starting at SBP
#define SSETBIT(WORD, BP)           (WORD = (1 << BP))                                          // Assign WORD with only bit BP set
#define SCLRBIT(WORD, BP)           (WORD = (1 << BP))                                          // Assign WORD with only bit BP set
#define READNIBBLE(WORD, BP)        ((WORD >> BP) & 0xF)                                        // Read 4-bit nibble starting at BP