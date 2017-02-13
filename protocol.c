//此文件用户文件的解析
#include "QSFP_PROTOCOL.h"
#include "debug.h"
//总线初始化的操作  
//其中需要返回3个参数，第一是返回NACK 是否需要给给参数赋值,此处可以增加协议，里面可以进行协内部的预处理过程
uint8_t xdata buff[10]={0};
uint8_t xdata ReiciverbufferSize=0;
//发送NOACK
bit InitFun(uint8_t IICValue,uint8_t * IsSend,uint8_t *Value)
{
	bit temp = QSFP_InitFun(IICValue,IsSend,Value);
	if(temp)                  
    	return 1;
	else 
	   temp=Debug_InitFun(IICValue,IsSend,Value);
	   if(temp)
	   	 return 1;
	   else 
	   	 return 0;
}
void ReciverValue(uint8_t IICValue,uint8_t * Send,uint8_t *Value)
{
	buff[ReiciverbufferSize++] = IICValue;     //recode value 
	QSFP_ReciverValue();
	Debug_ReciverValue(IICValue,Send,Value);
}
//返回一个有效的数据
uint8_t GetIICData()
{
	uint8_t TempValue,IsGetValue=0;	
	IsGetValue=QSFP_ACK(&TempValue);     //First Ask the QSFP Modul need SendData 
	if(IsGetValue)                       //如果不是QSFP响应,则询问是否是
	{
		return TempValue;               
	}
	else 
	{
		IsGetValue=Debug_ACK(&TempValue);
		if(IsGetValue)
		{
			return TempValue;     
		}
	}

}
void StopReset()
{
	StopFun(); 
	DEBUG_StopFun();
	ReiciverbufferSize=0;				//q清楚
}
//如果发送NOACK则需要有的时候需要复位变量与值
void ResetValue()
{
	
}