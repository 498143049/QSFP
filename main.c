/**************************************************************
*
*	����������Silicon IDE 4.5�� Keil C51 4
*	ʹ��оƬ��C8051F330
*	�����ܣ�
*
***************************************************************/
#include "c8051f330.h"
#include "c8051_i2c.h"
#include "c8051_system.h"
#include "crc.h"
#include "c8051_usart.h"
#include "c8051_gpio.h"
#include "c8051_it.h"
#include "types.h"
//#include "FIFOQUEUE.h"
#include "cmd.h"
#include "c8051_smbus.h"
#include "QSFP_Mem.h"
#include "journal.h"
#include "journal.h"
#include "c8051_it.h"
//uint8_t temp=0;
uint8_t xdata tempArray[4]={0};
/********************************Ŀǰ�����ö��еĽṹȥ��********/
void main()
{

	PCA0MD &= ~0x40; //��ֹ���Ź�

/***************************ϵͳ��ʼ��************************/
	SYSCLK_Init();   //��ʼ��ʱ��Ƶ��
	GPIO_Init();     //��ʼ���˿�I/O
	UASRT0_Init();   //��ʼ������UART0
	TX_I2C_Init();
	RX_I2C_Init();
	SMBus_Init();
	journal_MemInit();
/********************ֵ�ó�ʼ��********************************/
	QSFP_initValue();  //QSFP��ֵ��ʼ��
	Timer3_Init();
	Timer1_Init();
	Timer2_Init();
/***************************�жϹ���************************/
	EIE1|=1;   //����SMBUS�ж�
	ES0 = 1;  //����UART0�ж�
	EA  = 1;  //��ȫ�����ж�
	ET2	= 1;   //����ʱ��2�ж�
	TR2=1;    //TR2��ʼ����
/***************************����ִ��************************/
	while(1)
	{	
		delay_ms(100);
	}
}
