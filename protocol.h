#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include "types.h"
#define DebugKeyAddress 0xAB      //ȷ���ǵ��Դ���
#define DebugKey     0x1F      //����

bit InitFun(uint8_t IICValue,uint8_t * IsSend,uint8_t *Value);
void ReciverValue(uint8_t IICValue,uint8_t * IsSend,uint8_t *Value);
void StopReset();
uint8_t GetIICData();
#endif 