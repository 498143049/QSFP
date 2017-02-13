//////////////////////////////////////////////////////////
// 鏂囦欢锛欶IFOQUEUE.C
//////////////////////////////////////////////////////////
//Queue Init
#include "FIFOQUEUE.h"
#include "c8051_usart.h"
void QueueInit(struct FifoQueue *Queue)
{
    Queue->front = Queue->rear;
    Queue->count = 0;
}
// Queue In
uint8_t QueueIn(struct FifoQueue *Queue,ElemType sdat)
{
    if((Queue->front == Queue->rear) && (Queue->count == QueueSize))
    {                    // full
        return QueueFull;
    }else
    {                    // in
        Queue->dat[Queue->rear] = sdat;
        Queue->rear = (Queue->rear + 1) % QueueSize;
        Queue->count = Queue->count + 1;
        return QueueOperateOk;
    }
}
// Queue Out
extern  volatile  uint8_t * dealNow;
extern struct FifoQueue MyQueue_dealBuff;                  //还未处理的队列结构体   	
extern struct FifoQueue MyQueue_leisureBuff;               //空闲的队列结构体  
uint8_t QueueOut(struct FifoQueue *Queue,ElemType *sdat)
{
    if((Queue->front == Queue->rear) && (Queue->count == 0))
    {                    // empty
        return QueueEmpty;
    }else
    {                    // out
        *sdat = Queue->dat[Queue->front];
        Queue->front = (Queue->front + 1) % QueueSize;
        Queue->count = Queue->count - 1;
        return QueueOperateOk;
    }
}
//判断队列是否为空
uint8_t IsEmpty(struct FifoQueue *Queue)
{
	if((Queue->front == Queue->rear) && (Queue->count == 0))
    {                    // empty
        return QueueEmpty;
    }
	else
	{
		 return QueueOperateOk;
	}

}
void readValue(struct FifoQueue *Queue,ElemType *sdat)
{
	  struct ReciverCmd  ** a= sdat;
	   *sdat = Queue->dat[Queue->front];
    /*	if(Queue==&MyQueue_dealBuff)
		{
			UASRT0_Transmit(2);
			UASRT0_Transmit(sdat);	
			UASRT0_Transmit(*sdat);		
			UASRT0_Transmit(&dealNow);  //发送dealNow数组的地址。
			UASRT0_Transmit(2);
		}*/
        Queue->front = (Queue->front + 1) % QueueSize;
        Queue->count = Queue->count - 1;
}
 