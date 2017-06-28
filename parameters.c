#include "parameters.h"
#include "c8051_flash.h"
#include "journal.h"
#include "c8051_i2c.h"
//extern uint8_t Flash_data[256];
//
uint8_t xdata ParameterStru[2];
uint8_t GetOffset(uint16_t SetAddress)
{
	switch(SetAddress)
	{
		case TX_ParameterList: 
			return TxParametesNum;
			break;
		case RX_ParameterList :
			return RxParametesNum;
			break;
/*		case UserDefine_ParameterList:
			return UserDefineParametesNum;*/
			break;
		default:
			return 0;
	}
}
uint16_t GetAddress(uint8_t offset)
{
	switch(offset)
	{
		case TxParametesNum:
			return TX_ParameterList;
		break;
		case RxParametesNum:
			return RX_ParameterList;
		break;
	/*	case UserDefine_ParameterList:
			return UserDefine_ParameterList;
		break;*/
		default:
			return 0;
	}
}
uint8_t * GetParameter(uint16_t SetAddress,uint8_t num)
{
	ParameterStru[0]=FLASH_ByteRead(SetAddress+num-1);    //address 
	ParameterStru[1]=FLASH_ByteRead(SetAddress+num);      //value
	return  ParameterStru;
}
uint8_t DelecetParmeter(uint16_t SetAddress,uint8_t num)
{
	uint8_t tempi=num-1;
	uint8_t kindtype=GetOffset(SetAddress);
	uint8_t ParaNum = FLASH_ByteRead(ConfigAddr+kindtype);
	if(ParaNum<num)
	{
		return 0;
	}
	FLASH_ReadData(SetAddress,Flash_data,ParaNum*2);
	FLASH_PageErase(SetAddress);
	for(;tempi<2*ParaNum-2;tempi++)
	{
		Flash_data[tempi]=Flash_data[tempi+2];
	}
	ParaNum--;

	FLASH_WriteData(SetAddress,Flash_data,ParaNum*2);
	updateConfig(kindtype,ParaNum);
	return 1;
}
uint8_t ChangeParmeter(uint16_t SetAddress,uint8_t num,uint8_t address,uint8_t value)
{
	uint8_t kindtype=GetOffset(SetAddress);
	uint8_t ParaNum = FLASH_ByteRead(ConfigAddr+kindtype);
	if(ParaNum<num)
	{
		return 0;
	}
	FLASH_ReadData(SetAddress,Flash_data,ParaNum*2); 
	FLASH_PageErase(SetAddress);
	Flash_data[2*num-2]=address;
	Flash_data[2*num-1]=value;
	FLASH_WriteData(SetAddress,Flash_data,ParaNum*2);
	return 1;
}
uint8_t AddParameter(uint16_t SetAddress,uint8_t address,uint8_t value)
{
	uint8_t kindtype=GetOffset(SetAddress);
	uint8_t ParaNum = FLASH_ByteRead(ConfigAddr+kindtype);
	FLASH_ReadData(SetAddress,Flash_data,ParaNum*2);  
	FLASH_PageErase(SetAddress);
	Flash_data[ParaNum]=address;
	Flash_data[ParaNum+1]=value;
	ParaNum++;
	FLASH_WriteData(SetAddress,Flash_data,ParaNum*2);
	updateConfig(kindtype,ParaNum);
	return 1;
}

/*void Init_TX_Parameters()
{
	uint8_t ParaNum = FLASH_ByteRead(ConfigAddr+TxParametesNum);
	FLASH_ReadData(TX_ParameterList,Flash_data,ParaNum*2);  
	while(--ParaNum>0)
	{
		TX_I2C_SingleByteWrite(HXT6104Address,Flash_data[ParaNum-1],Flash_data[ParaNum]);
	}
}
void Init_Rx_Parameters()
{
	uint8_t ParaNum = FLASH_ByteRead(ConfigAddr+RxParametesNum);
	FLASH_ReadData(RX_ParameterList,Flash_data,ParaNum*2);  
	while(--ParaNum>0)
	{
		RX_read(HXR6104Address,Flash_data[ParaNum-1],Flash_data[ParaNum]);
	}
}
void Init_User_define()
{

}*/