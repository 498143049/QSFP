/**************************************************************
*
*	开发环境：Silicon IDE 4.5， Keil C51 4
*	使用芯片：C8051F330
*	程序功能：
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
/********************************目前不采用队列的结构去做********/
void main()
{

	PCA0MD &= ~0x40; //禁止看门狗

/***************************系统初始化************************/
	SYSCLK_Init();   //初始化时钟频率
	GPIO_Init();     //初始化端口I/O
	UASRT0_Init();   //初始化串口UART0
	TX_I2C_Init();
	RX_I2C_Init();
	SMBus_Init();
	journal_MemInit();
/********************值得初始化********************************/
	QSFP_initValue();  //QSFP的值初始化
	Timer3_Init();
	Timer1_Init();
	Timer2_Init();
/***************************中断管理************************/
	EIE1|=1;   //开启SMBUS中断
	ES0 = 1;  //开启UART0中断
	EA  = 1;  //开全局总中断
	ET2	= 1;   //允许定时器2中断
	TR2=1;    //TR2开始计算
/***************************程序执行************************/
	while(1)
	{	
		delay_ms(100);
	}
}
