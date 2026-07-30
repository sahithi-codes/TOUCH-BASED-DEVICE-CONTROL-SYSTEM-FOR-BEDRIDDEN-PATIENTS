#define LOADVAL (PCLK / SPI_SPEED)


//SPI Control Register Bit Positions

// Clock Phase (CPHA) bit position in the SPI Control Register
#define CPHA 3

// Clock Polarity (CPOL) bit position in the SPI Control Register
#define CPOL 4

// Master Mode Select (MSTR) bit position
// Setting this bit configures the LPC214x as SPI Master
#define MSTR 5

// SPI Transfer Complete Flag (SPIF) bit position in the SPI Status Register
// This bit becomes 1 when an SPI transfer is complete
#define SPIF 7


//SPI EEPROM Command Codes

// EEPROM WRITE command (writes data to memory)
#define WRITE 0x02

// EEPROM WRITE ENABLE command (enables write operations)
#define WREN  0x06

// EEPROM WRITE DISABLE command (disables write operations)
#define WRDI  0x04

// EEPROM READ command (reads data from memory)
#define READ  0x03

