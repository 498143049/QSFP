#include "journal.h"
#include "QSFP_Mem.h"
#include "c8051_flash.h"

extern xdata uint8_t Flash_data[256];
uint8_t xdata JourBuffer[10];
void EraseAlljour()
{
	FLASH_PageErase(JourAddr);
	FLASH_PageErase(JourAddr+512);
	FLASH_PageErase(JourAddr+1024);
	FLASH_PageErase(JourAddr+1536);
}
void updateConfig(uint8_t Kind,uint8_t Value)
{
	FLASH_ReadData(ConfigAddr,Flash_data,5);  //�ȶ�ȡ����
	FLASH_PageErase(ConfigAddr);
	Flash_data[Kind]=Value;
	FLASH_WriteData(ConfigAddr,Flash_data,5);  
}
uint8_t  GetConfigNum(uint8_t kind)
{
	FLASH_ReadData(ConfigAddr,Flash_data,5);  //�ȶ�ȡ����
	return Flash_data[kind];
}
void journal_MemInit()
{
	if(FLASH_ByteRead(ConfigAddr)!=securityCodeValue)
	{
		//������������
		FLASH_PageErase(ConfigAddr);
		Flash_data[securityCodeAdd]=securityCodeValue;
		Flash_data[JourNum]=0;
		Flash_data[TxParametesNum]=0;
		Flash_data[RxParametesNum]=0;
		Flash_data[UserDefineParametesNum]=0;
		FLASH_WriteData(ConfigAddr,Flash_data,5);
		//������־��
		EraseAlljour();
		//�������ʼ�� 

		//QSFP�ڴ�ֵ��ʼ��
		QSFP_Flash_Init();
	
	}
}
void AddOneJournal(uint8_t  * buffer,uint8_t  num)
{

	uint8_t  FlashNum = FLASH_ByteRead(ConfigAddr+JourNum);
	if(FlashNum==255)
	{
		EraseAlljour();
		FlashNum=0;
	}
	FLASH_WriteData(JourAddr+8*FlashNum,buffer,num);
	updateConfig(JourNum,++FlashNum); 
}
uint8_t * ReadOneJour(uint8_t num)
{
	FLASH_ReadData(JourAddr+8*(num-1),JourBuffer,8);
	return JourBuffer;
}
