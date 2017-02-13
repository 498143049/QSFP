#ifndef __C8051_FLASH_H
#define __C8051_FLASH_H

#include "types.h"

typedef uint16_t FLADDR; 


void FLASH_ByteWrite (FLADDR addr, uint8_t byte);
uint8_t FLASH_ByteRead (FLADDR addr);
void FLASH_PageErase (FLADDR addr);
void FLASH_WriteData(FLADDR addr,uint8_t *buffer,uint8_t length);
void FLASH_ReadData(FLADDR addr,uint8_t *buffer,uint8_t length);

#endif 