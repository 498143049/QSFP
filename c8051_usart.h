#include "c8051f330.h"

#ifndef __C8051_USART_H
#define __C8051_USART_H

//����0�����ʵ�����Ϊ9600
#define  BAUDRATE 56000  
#define  BITNUMS 10  		//һ�δ��� 10 ���ַ� 


void UASRT0_Init(void); //UASRT0�ĳ�ʼ��
void UASRT0_Transmit(unsigned char Data); //UASRT0���͵��ֽ�
void UASRT0_SendStr(unsigned char *Data); //UASRT0�����ַ���



#endif /* __C8051_USART_H */