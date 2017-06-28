#include "c8051f330.h"
#include "c8051_system.h"
#include "c8051_usart.h"

/************************************************
*	函数名称：UASRT0_Init
*	功能描述：UASRT0_Init的初始化
*	参    数：使用Time1当成定时器
*   返 回 值：无
*   作    者: 
*************************************************/
void UASRT0_Init(void)
{
	SCON0 = 0x10;   //串口工作方式允许接收，8位数据传送
	if(SYSCLK/BAUDRATE/2/256 < 1)
	{
		TH1 = -(SYSCLK/BAUDRATE/2); //=-什么意思
		CKCON &= ~0x0B;            //
		CKCON |= 0x08;              //
	}
	else if(SYSCLK/BAUDRATE/2/256 < 4)
	{
		TH1 = -(SYSCLK/BAUDRATE/2); //=-什么意思
		CKCON &= ~0x0B;            //
		CKCON |= 0x09;              //
	}
	else if(SYSCLK/BAUDRATE/2/256 < 12)
	{
		TH1 = -(SYSCLK/BAUDRATE/2/12); //=-什么意思
		CKCON &= ~0x0B;            //
	}
	else
	{
		TH1 = -(SYSCLK/BAUDRATE/48); //=-什么意思
		CKCON &= ~0x0B;            //
		CKCON |= 0x02;              //
	}

	TL1 = TH1;
	TMOD &= 0xF0;
	TMOD |= 0x20; //定时器采用1，工作方式为2
	TR1 = 1;

	TI0=1;
}

/************************************************
*	函数名称：UASRT0_Transmit
*	功能描述：UASRT0发送单字节
*	参    数：unsigned char *Data
*   返 回 值：无
*   作    者: 
*************************************************/
void UASRT0_Transmit(unsigned char Data)
{
	ES0 = 0;         //关闭UASRT0中断
	SBUF0 = Data;    //将发送的字节放入缓冲区
	while(!TI0); //等待发送
	TI0=0;
	ES0 = 1;         //打开UASRT0中断
}

/************************************************
*	函数名称：UASRT0_SendStr
*	功能描述：UASRT0发生字符串
*	参    数：unsigned char *Data，将要发送的字符串
*   返 回 值：无
*   作    者: 
*************************************************/
void UASRT0_SendStr(unsigned char *Data)
{
	while(*Data)
	{
		UASRT0_Transmit(*Data++);
	}
}