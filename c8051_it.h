#include "c8051f330.h"

#ifndef __C8051_IT_H
#define __C8051_IT_H

#include "types.h"

/* 
extern uint8_t usart_recvbuf[16];
extern uint8_t usart_sendbuf[16];
extern uint8_t recvbuf_index_stop; //��������ַ����ĸ��� 
extern  volatile uint8_t half_chars_passed;  //���ַ���������ÿ�ζ�ʱ���������Ҫ�����1
*/

/*extern struct FifoQueue xdata MyQueue_dealBuff;                  //��δ����Ķ��нṹ��   	
extern struct FifoQueue xdata MyQueue_leisureBuff;               //���еĶ��нṹ��   */

void Timer2_Init(void); //��ʱ��2�ĳ�ʼ��
void Timer0_Init(void); //��SM_BUS ʹ��
void Timer3_Init(void); //��Time3ʹ��

#endif /* __C8051_IT_H */