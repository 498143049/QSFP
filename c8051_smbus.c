#include "c8051f330.h"
#include "c8051_smbus.h"
#include "c8051_usart.h"

/************************************************
*	函数名称：SMBus_Init
*	功能描述：SMBus的初始化
*	参    数：无
*   返 回 值：无
*   作    者: 
*************************************************/
/**********初始化为从机模式*******************/
void SMBus_Init(void)
{
    SMB0CF |= 0x98; //使能SMBus总线，从机模式，SDA数据使能,Timer 0作为时钟源 //使能检测
    SMB0CF |= 0x80; //使能SMBus总线
}
void SMBus_ReSet(void)
{
    STA = 0;
    STO = 0;
    ACK = 0;
}
