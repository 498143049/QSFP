#include "cmd.h"
#include "c8051_system.h"
#include "types.h"
#include "c8051_flash.h"
#include "c8051_usart.h"
#include "c8051_i2c.h"
#include "QSFP_Mem.h"
#include "journal.h"
//#include "crc.
//extern uint8_t xdata Flash_data[256];
uint8_t Read_Rx(uint8_t lowAddress)
{
	return RX_read(lowAddress);
}
uint8_t Read_Tx(uint8_t lowAddress)
{
	return TX_read(lowAddress);
}
uint8_t Write_Rx(uint8_t lowAddress,uint8_t Data)
{
   RX_write(lowAddress,Data);
   if(Data==RX_read(lowAddress))
   		return 1;
	else
		return 0;
}
uint8_t Write_Tx(uint8_t lowAddress,uint8_t Data)
{

   TX_write(lowAddress,Data);
   if(Data==TX_read(lowAddress))
   		return 1;
	else
		return 0;
}
void praseTostr(struct ReciverCmd * cmd,uint8_t * buffer,uint8_t bufferLength)
{
	uint8_t xdata num = 0;
	cmd->fun=*(FunCode *)buffer;
	cmd->addr=*(AddressCode *)(buffer+sizeof(cmd->fun));
	cmd->Length=bufferLength-sizeof(cmd->fun)-sizeof(cmd->addr); 
	num=cmd->Length;
	while(num>0)
	{
		num--;
		cmd->dataValue[num]=buffer[sizeof(cmd->fun)+sizeof(cmd->addr)+num];
	}
}

ReadFunKind * GetAccerssReadFun(uint8_t highAddress)
{
  switch(highAddress)
 {
		case Tx_driver:
			return Read_Tx;
			break;
		case Rx_driver:
			return Read_Rx;
			break;
		case QSFP:
			return Get_Data;
		case AddressNum:
			return GetConfigNum;
		default:
			return 0;
			break;
  }
}
WriteFunKind * GetAccerssWriteFun(uint8_t highAddress)
{
	switch(highAddress)
 	{
		case Tx_driver:
			return Write_Tx;
			break;
		case Rx_driver:
			return Write_Rx;
			break;
		case QSFP:
			return Write_SingleData;
		default:
			return 0;
			break;
  }
}
uint8_t Fillsinglebyte(uint8_t GetValue,uint8_t * Sendbuff,struct ReciverCmd * cmd)
{
	uint8_t xdata tempi=0;
	uint8_t xdata * sendHead= (uint8_t *)cmd;
	//赋值数组
	
	for(tempi=1;tempi<(sizeof(cmd->fun)+sizeof(cmd->addr))+1;tempi++)
	{
		Sendbuff[tempi]=*sendHead++;
	}
	Sendbuff[tempi]=GetValue;
	Sendbuff[0]=tempi;
	return tempi;
}
uint8_t Fillsinglebytes(uint8_t * value ,uint8_t fillNum,uint8_t * Sendbuff,struct ReciverCmd * cmd)
{
	uint8_t xdata tempi=0;
	uint8_t xdata * sendHead= (uint8_t *)cmd;
	//赋值数组
	
	for(tempi=1;tempi<(sizeof(cmd->fun)+sizeof(cmd->addr))+1;tempi++)
	{
		Sendbuff[tempi]=*sendHead++;
	}
	while(--fillNum>0)
	{
		Sendbuff[tempi++]=*value++;
	}
	return tempi;
}
/*
void writeToFlash(uint8_t funCode,uint16_t Address,uint8_t Status)
{
	uint8_t num = 0;
	num=FLASH_ByteRead(ConfigAddr+1);
	FLASH_ByteWrite(JourAddr+4*num,funCode);
	FLASH_ByteWrite(JourAddr+4*num+1,BYTE_LOW(Address));
	FLASH_ByteWrite(JourAddr+4*num+2,BYTE_HIGH(Address));
	FLASH_ByteWrite(JourAddr+4*num+3,Status);
	FLASH_PageErase(ConfigAddr);
	FLASH_ByteWrite(ConfigAddr,0xAB);
	FLASH_ByteWrite(ConfigAddr+1,++num);

}
void writeToParametersTable(uint8_t Paaddr,uint8_t Value)
{
	//读取
	FLASH_ReadData(ParametersAddr,Flash_data,255);
	Flash_data[Paaddr]=Value;
	//擦除扇区
	FLASH_PageErase(ParametersAddr);
	FLASH_WriteData(ParametersAddr,Flash_data,255);

}
//返回错误结果
void ReturnMessage(uint8_t fun,uint16_t Address,uint8_t Status,uint8_t ErrorValue,uint8_t dataValue,uint8_t dataLength)
{
	uint16_t CRCVAlue;
	uint8_t i;
	SendBuffer[0]=fun+ErrorValue;
	SendBuffer[1]=BYTE_LOW(Address);
	SendBuffer[2]=BYTE_HIGH(Address);
	if(Status==1)
	{
		SendBuffer[3]=ErrorValue;
		CRCVAlue=crc_feed(SendBuffer,4,0xFFFF);
	}
	else 
	{
		if(dataLength!=0)
		{
			SendBuffer[3]=dataValue;
			CRCVAlue=crc_feed(SendBuffer,4,0xFFFF);
		}
		else
		{
			CRCVAlue=crc_feed(SendBuffer,3,0xFFFF);
		}
	}
	if((Status==1)||(dataLength!=0))
	{
		SendBuffer[4]=BYTE_HIGH(CRCVAlue);
		SendBuffer[5]=BYTE_LOW(CRCVAlue);
		for(i=0;i<=5;i++)
		{
			UASRT0_Transmit(SendBuffer[i]);
		}
	}
	else
	{
		SendBuffer[3]=BYTE_HIGH(CRCVAlue);
		SendBuffer[4]=BYTE_LOW(CRCVAlue);
		for(i=0;i<=4;i++)
		{
			UASRT0_Transmit(SendBuffer[i]);	
		}
	
	}
}
//读取第几条发送的数据
void ReadJour(uint8_t num)
{
	UASRT0_Transmit(FLASH_ByteRead(JourAddr+(num-1)*4));
	UASRT0_Transmit(FLASH_ByteRead(JourAddr+(num-1)*4+1));
	UASRT0_Transmit(FLASH_ByteRead(JourAddr+(num-1)*4+2));
	UASRT0_Transmit(FLASH_ByteRead(JourAddr+(num-1)*4+3));
}
void RetrurnHelp()
{
	UASRT0_SendStr("|=======Test Main menu =================|\n\r");
	UASRT0_SendStr("| 02  ---单字节写，格式: 02 00 00 84 FF 01 |\n\r");
	UASRT0_SendStr("| 06  ---单字节读，格式: 06 00 00 C1 91	   |\n\r");
	UASRT0_SendStr("| 0A  ---日志读取，格式: 0A 00 02 03 D0    |\n\r");
	UASRT0_SendStr("| 0C  ---报警测试，格式: 0C 00 00 C3 B1    |\n\r");
	UASRT0_SendStr("| FE  ---提示菜单，格式: FE 00 00 30 10    |\n\r");
	UASRT0_SendStr("|======================================|\n\r");
}
void PrintAlarm(uint8_t type)
{
	switch(type)
	{
		case 0:
			UASRT0_SendStr("| 电压异常|\n\r");
		break;
		case 1:
			UASRT0_SendStr("| 温度异常|\n\r");
		break;
		case 2:
			UASRT0_SendStr("| 电流功率异常|\n\r");
		break;
	}
}*/