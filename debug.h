#ifndef __DEBUG_H
#define __DEBUG_H
#include "types.h"

#define DEBUG_ADDRESS 0XAB
#define DebugKey     0x1F      //√‹¬Î

void DEBUG_StopFun();
void  Debug_InitFun(uint8_t  * IsSend,uint8_t  *Value);
void Debug_ReciverValue(uint8_t * Send,uint8_t *Value);
uint8_t Debug_ACK();
#endif 