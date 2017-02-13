#include "c8051f330.h"

#ifndef __C8051_SYSTEM_H
#define __C8051_SYSTEM_H

#define SYSCLK   24500000   //系统时钟频率



//宏函数
//取高8位
#define BYTE_HIGH( u16 ) ( (uint8_t)( u16 >> 8 ) )
//取低8位
#define BYTE_LOW( u16 ) ( (uint8_t)( u16 & 0x00ff ) )
//合成字
#define BYTES2WORD( byte_msb, byte_lsb ) ( ( (uint16_t)byte_msb << 8 ) | byte_lsb )


void SYSCLK_Init(void);
void Flash_init();
#endif /* __C8051_SYSTEM_H */