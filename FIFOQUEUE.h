//////////////////////////////////////////////////////////
// 文件：FIFOQUEUE.h
//////////////////////////////////////////////////////////
#ifndef _FIFOQUEUE_H
#define _FIFOQUEUE_H
#include "types.h"
#define ElemType       struct ReciverCmd *
#define QueueSize      10 
#define QueueFull      0
#define QueueEmpty     1
#define QueueOperateOk 2
struct FifoQueue
{
    uint16_t front;
    uint16_t rear;
    uint16_t count;
    ElemType dat[QueueSize];
};
//Queue Initalize
extern void QueueInit(struct FifoQueue *Queue);
// Queue In
extern uint8_t QueueIn(struct FifoQueue *Queue,ElemType sdat);
// Queue Out
extern uint8_t QueueOut(struct FifoQueue *Queue,ElemType *sdat);
extern uint8_t IsEmpty(struct FifoQueue *Queue);
extern void readValue(struct FifoQueue *Queue,ElemType *sdat);

#endif