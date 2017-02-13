#include "c8051f330.h"

#ifndef __C8051_IT_H
#define __C8051_IT_H

#include "types.h"

/* 
extern uint8_t usart_recvbuf[16];
extern uint8_t usart_sendbuf[16];
extern uint8_t recvbuf_index_stop; //计入接受字符到的个数 
extern  volatile uint8_t half_chars_passed;  //半字符计算器，每次定时器进入后需要给其加1
*/
 

extern struct FifoQueue xdata MyQueue_dealBuff;                  //还未处理的队列结构体   	
extern struct FifoQueue xdata MyQueue_leisureBuff;               //空闲的队列结构体   

void Timer2_Init(void);  //定时器2的初始化
void Timer1_Init (void);
void Timer3_Init (void);
#endif /* __C8051_IT_H */