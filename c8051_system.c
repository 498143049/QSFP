#include "c8051_system.h"
#include "c8051_flash.h"
#include "c8051_i2c.h"
/************************************************
*	�������ƣ�SYSCLK_Init
*	����������ϵͳʱ��Ƶ�ʵĳ�ʼ��
*	��    ������
*   �� �� ֵ����
*   ��    ��: 
*************************************************/
void SYSCLK_Init(void)
{
	OSCICN |= 0x03; //�����ڲ����������Ƶ�� ����Ƶ���
	RSTSRC = 0x04;  //ʹ��ʼ�ն�ʧ���Ĵ���
}
void Flash_init()
{
	//��ȡ�Ƿ���0xAB��
/*	if(FLASH_ByteRead(ConfigAddr)!=0xAB)
	{
		//���ʼ��������ϵͳʹ�á�
		FLASH_PageErase(ConfigAddr);
		FLASH_PageErase(JourAddr);
		//��ʼ����������
		FLASH_ByteWrite(ConfigAddr,0xAB);
		FLASH_ByteWrite(ConfigAddr+1,0);
		//��ȡ��һ��Ԫ�ص�ֵ
	
	}
	else
	{
    	 uint8_t temp=FLASH_ByteRead(ParametersAddr);
		//	I2C_SingleByteWrite(0xEE,0,temp);
	}
	*/
}