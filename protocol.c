//���ļ��û��ļ��Ľ���
#include "QSFP_PROTOCOL.h"
#include "debug.h"
//���߳�ʼ���Ĳ���  
//������Ҫ����3����������һ�Ƿ���NACK �Ƿ���Ҫ����������ֵ,�˴���������Э�飬������Խ���Э�ڲ���Ԥ�������
uint8_t xdata buff[10]={0};
uint8_t xdata ReiciverbufferSize=0;
//����NOACK
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
//����һ����Ч������
uint8_t GetIICData()
{
	uint8_t TempValue,IsGetValue=0;	
	IsGetValue=QSFP_ACK(&TempValue);     //First Ask the QSFP Modul need SendData 
	if(IsGetValue)                       //�������QSFP��Ӧ,��ѯ���Ƿ���
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
	ReiciverbufferSize=0;				//q���
}
//�������NOACK����Ҫ�е�ʱ����Ҫ��λ������ֵ
void ResetValue()
{
	
}