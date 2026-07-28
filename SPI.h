#ifndef SPI_H
#define SPI_H

#include "types.h"

void Init_SPI(void);
u8 spi(u8 sDat);
void WriteEnable(void);
void WriteDisable(void);

void Spi_eeprom_bytewrite(u16 addr, u8 data);
u8 Spi_eeprom_byteread(u16 addr);

void EEPROM_WritePage(u16 addr, u8 *p, u8 nbytes);
void Spi_eeprom_seqread(u16 addr, u8 *p, u8 nbytes);
void Password_Read(char *str);
void Password_LoadFromEEPROM(void);

#endif

