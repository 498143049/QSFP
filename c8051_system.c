#include "c8051_system.h"
#include "c8051_flash.h"
#include "c8051_i2c.h"
/************************************************
*	函数名称：SYSCLK_Init
*	功能描述：系统时钟频率的初始化
*	参    数：无
*   返 回 值：无
*   作    者: 
*************************************************/
void SYSCLK_Init(void)
{
	OSCICN |= 0x03; //配置内部振荡器的最大频率 不分频输出
	RSTSRC = 0x04;  //使能始终丢失检测寄存器
}
void Flash_init()
{
	//读取是否是0xAB；
/*	if(FLASH_ByteRead(ConfigAddr)!=0xAB)
	{
		//则初始化擦除给系统使用。
		FLASH_PageErase(ConfigAddr);
		FLASH_PageErase(JourAddr);
		//初始化配置区域
		FLASH_ByteWrite(ConfigAddr,0xAB);
		FLASH_ByteWrite(ConfigAddr+1,0);
		//读取第一块元素的值
	
	}
	else
	{
    	 uint8_t temp=FLASH_ByteRead(ParametersAddr);
		//	I2C_SingleByteWrite(0xEE,0,temp);
	}
	*/
}