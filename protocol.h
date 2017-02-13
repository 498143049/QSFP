#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include "types.h"
#define DebugKeyAddress 0xAB      //确定是调试代码
#define DebugKey     0x1F      //密码

bit InitFun(uint8_t IICValue,uint8_t * IsSend,uint8_t *Value);
void ReciverValue(uint8_t IICValue,uint8_t * IsSend,uint8_t *Value);
void StopReset();
uint8_t GetIICData();
#endif 