#include "c8051f330.h"

#ifndef __C8051_SYSTEM_H
#define __C8051_SYSTEM_H

#define SYSCLK 24500000 //系统时钟频率

#define HXTTYPE 11
#define HXRTYPE 12

/************************************操作的宏函数**********************************/
#define BIT_CLEAN(var, n) (var) &= ~(1 << (n))  //变量var 的n位（即第n+1位）清0
#define BIT_SET(var, n) (var) |= (1 << (n))     //变量var 的n位（即第n+1位）置1
#define BIT_GET(var, n) (((var) >> (n)) & 0x01) //读取变量var 的n位（即第n+1位）

//宏函数
//取高8位
#define BYTE_HIGH(u16) ((uint8_t)(u16 >> 8))
//取低8位
#define BYTE_LOW(u16) ((uint8_t)(u16 & 0x00ff))
//合成字
#define BYTES2WORD(byte_msb, byte_lsb) (((uint16_t)byte_msb << 8) | byte_lsb)

#define reset_system() RSTSRC |= 0x10
void SYSCLK_Init(void);

#endif /* __C8051_SYSTEM_H */