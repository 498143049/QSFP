//////////////////////////////////////////////////////////
// 文件：FIFOQUEUE.C
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
extern struct FifoQueue MyQueue_dealBuff;                  //��δ����Ķ��нṹ��   	
extern struct FifoQueue MyQueue_leisureBuff;               //���еĶ��нṹ��  
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
//�ж϶����Ƿ�Ϊ��
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
			UASRT0_Transmit(&dealNow);  //����dealNow����ĵ�ַ��
			UASRT0_Transmit(2);
		}*/
        Queue->front = (Queue->front + 1) % QueueSize;
        Queue->count = Queue->count - 1;
}
 