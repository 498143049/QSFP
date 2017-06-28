#include "c8051f330.h"
#include "c8051_system.h"
#include "c8051_usart.h"

/************************************************
*	�������ƣ�UASRT0_Init
*	����������UASRT0_Init�ĳ�ʼ��
*	��    ����ʹ��Time1���ɶ�ʱ��
*   �� �� ֵ����
*   ��    ��: 
*************************************************/
void UASRT0_Init(void)
{
	SCON0 = 0x10;   //���ڹ�����ʽ������գ�8λ���ݴ���
	if(SYSCLK/BAUDRATE/2/256 < 1)
	{
		TH1 = -(SYSCLK/BAUDRATE/2); //=-ʲô��˼
		CKCON &= ~0x0B;            //
		CKCON |= 0x08;              //
	}
	else if(SYSCLK/BAUDRATE/2/256 < 4)
	{
		TH1 = -(SYSCLK/BAUDRATE/2); //=-ʲô��˼
		CKCON &= ~0x0B;            //
		CKCON |= 0x09;              //
	}
	else if(SYSCLK/BAUDRATE/2/256 < 12)
	{
		TH1 = -(SYSCLK/BAUDRATE/2/12); //=-ʲô��˼
		CKCON &= ~0x0B;            //
	}
	else
	{
		TH1 = -(SYSCLK/BAUDRATE/48); //=-ʲô��˼
		CKCON &= ~0x0B;            //
		CKCON |= 0x02;              //
	}

	TL1 = TH1;
	TMOD &= 0xF0;
	TMOD |= 0x20; //��ʱ������1��������ʽΪ2
	TR1 = 1;

	TI0=1;
}

/************************************************
*	�������ƣ�UASRT0_Transmit
*	����������UASRT0���͵��ֽ�
*	��    ����unsigned char *Data
*   �� �� ֵ����
*   ��    ��: 
*************************************************/
void UASRT0_Transmit(unsigned char Data)
{
	ES0 = 0;         //�ر�UASRT0�ж�
	SBUF0 = Data;    //�����͵��ֽڷ��뻺����
	while(!TI0); //�ȴ�����
	TI0=0;
	ES0 = 1;         //��UASRT0�ж�
}

/************************************************
*	�������ƣ�UASRT0_SendStr
*	����������UASRT0�����ַ���
*	��    ����unsigned char *Data����Ҫ���͵��ַ���
*   �� �� ֵ����
*   ��    ��: 
*************************************************/
void UASRT0_SendStr(unsigned char *Data)
{
	while(*Data)
	{
		UASRT0_Transmit(*Data++);
	}
}