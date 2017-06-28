#include "c8051f330.h"

#ifndef __C8051_SYSTEM_H
#define __C8051_SYSTEM_H

#define SYSCLK 24500000 //ϵͳʱ��Ƶ��

#define HXTTYPE 11
#define HXRTYPE 12

/************************************�����ĺ꺯��**********************************/
#define BIT_CLEAN(var, n) (var) &= ~(1 << (n))  //����var ��nλ������n+1λ����0
#define BIT_SET(var, n) (var) |= (1 << (n))     //����var ��nλ������n+1λ����1
#define BIT_GET(var, n) (((var) >> (n)) & 0x01) //��ȡ����var ��nλ������n+1λ��

//�꺯��
//ȡ��8λ
#define BYTE_HIGH(u16) ((uint8_t)(u16 >> 8))
//ȡ��8λ
#define BYTE_LOW(u16) ((uint8_t)(u16 & 0x00ff))
//�ϳ���
#define BYTES2WORD(byte_msb, byte_lsb) (((uint16_t)byte_msb << 8) | byte_lsb)

#define reset_system() RSTSRC |= 0x10
void SYSCLK_Init(void);

#endif /* __C8051_SYSTEM_H */