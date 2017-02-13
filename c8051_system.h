#include "c8051f330.h"

#ifndef __C8051_SYSTEM_H
#define __C8051_SYSTEM_H

#define SYSCLK   24500000   //ϵͳʱ��Ƶ��



//�꺯��
//ȡ��8λ
#define BYTE_HIGH( u16 ) ( (uint8_t)( u16 >> 8 ) )
//ȡ��8λ
#define BYTE_LOW( u16 ) ( (uint8_t)( u16 & 0x00ff ) )
//�ϳ���
#define BYTES2WORD( byte_msb, byte_lsb ) ( ( (uint16_t)byte_msb << 8 ) | byte_lsb )


void SYSCLK_Init(void);
void Flash_init();
#endif /* __C8051_SYSTEM_H */