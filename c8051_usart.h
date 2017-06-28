#include "c8051f330.h"

#ifndef __C8051_USART_H
#define __C8051_USART_H

//串口0波特率的设置为9600
#define  BAUDRATE 56000  
#define  BITNUMS 10  		//一次传输 10 个字符 


void UASRT0_Init(void); //UASRT0的初始化
void UASRT0_Transmit(unsigned char Data); //UASRT0发送单字节
void UASRT0_SendStr(unsigned char *Data); //UASRT0发生字符串



#endif /* __C8051_USART_H */