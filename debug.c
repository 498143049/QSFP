#include "debug.h"
#include "cmd.h"
#include "c8051_system.h"
#include "parameters.h"
#include "journal.h"
#include "c8051_flash.h"
uint8_t xdata Sendbuff[10]; //recode Return the buff
uint8_t xdata _buffercount; //recode the num of buff
extern iuint8_t SMBUSFIFOReciver[10];
extern xdata uint8_t ReiciverbufferSize;
uint8_t xdata Getvalue; //recode the value

uint8_t xdata *xdata _DebugsendPoint; //inorder to recode the send point;
ReadFunKind *adc;

/*void DEBUG_StopFun()
{
	
}*/
void Debug_InitFun(uint8_t *IsSend, uint8_t *Value)
{
    if (ReiciverbufferSize != 0) //里面记录了秘钥
    {
	_buffercount = (SMBUSFIFOReciver[0] & 0x07);
	if ((SMBUSFIFOReciver[0] >> 3) == DebugKey)
	{
	    ReiciverbufferSize = 0;
	}
    }
    *IsSend = 0;
    *Value = 0; //不返回数据
}
void Debug_ReciverValue(uint8_t *Send, uint8_t *Value)
{
    if (_buffercount == ReiciverbufferSize) //如果确定调试模式而秘钥验证成功
    {
	//这里进行数据解析
	struct ReciverCmd dealNow;
	uint16_t address;
	uint8_t *point;
	praseTostr(&dealNow, SMBUSFIFOReciver, ReiciverbufferSize); //命令解析为一个结构体
								    //传入命令执行器
	switch (dealNow.fun)
	{
	//读写寄存器控制
	case ReadIICSingle:
	    adc = GetAccerssReadFun(BYTE_HIGH(dealNow.addr));
	    Getvalue = (*adc)(BYTE_LOW(dealNow.addr));
	    //获取到值下一步打包数据数据格式 /
	    *Value = Fillsinglebyte(Getvalue, Sendbuff, &dealNow); //返回数据格式长度
	    _DebugsendPoint = Sendbuff;
	    break;
	//写命令
	case WriteIICSingle:
	    Getvalue = (*GetAccerssWriteFun(BYTE_HIGH(dealNow.addr)))(BYTE_LOW(dealNow.addr), dealNow.dataValue[0]);
	    *Value = Fillsinglebyte(Getvalue, Sendbuff, &dealNow); //返回数据格式长度
	    _DebugsendPoint = Sendbuff;
	    break;
	case ReadParamete:
	    address = GetAddress(BYTE_HIGH(dealNow.addr));
	    if (!address)
	    {
		point = GetParameter(address, BYTE_LOW(dealNow.addr));
		*Value = Fillsinglebytes(point, 2, Sendbuff, &dealNow);
		_DebugsendPoint = Sendbuff;
	    }
	    break;
	case AddParamete:
	    address = GetAddress(BYTE_HIGH(dealNow.addr));
	    if (!address)
	    {
		AddParameter(address, dealNow.dataValue[0], dealNow.dataValue[1]);
		*Value = Fillsinglebyte(50, Sendbuff, &dealNow); //返回数据格式长度
		_DebugsendPoint = Sendbuff;
	    }
	    break;
	case DeleteParamete:
	    address = GetAddress(BYTE_HIGH(dealNow.addr));
	    if (!address)
	    {
		DelecetParmeter(address, dealNow.dataValue[0]);
		*Value = Fillsinglebyte(50, Sendbuff, &dealNow); //返回数据格式长度
		_DebugsendPoint = Sendbuff;
	    }
	    break;
	case ChangeParamete:
	    address = GetAddress(BYTE_HIGH(dealNow.addr));
	    if (!address)
	    {
		ChangeParmeter(address, BYTE_LOW(dealNow.addr), dealNow.dataValue[0], dealNow.dataValue[1]);
		*Value = Fillsinglebyte(50, Sendbuff, &dealNow); //返回数据格式长度
		_DebugsendPoint = Sendbuff;
	    }
	    break;
	/*	case ReadJouranal:
				 point=ReadOneJour(BYTE_LOW(dealNow.addr));
				*Value=Fillsinglebytes(point,point[0],Sendbuff,&dealNow);
				 _DebugsendPoint=Sendbuff;
				break;
			case ReadJournalNearly:
				ReadOneJour(FLASH_ByteRead(ConfigAddr+JourNum));
				*Value=Fillsinglebytes(point,point[0],Sendbuff,&dealNow);
				 _DebugsendPoint=Sendbuff;
				//读取条数然后在获取
				break;
				*/
	default:
	    break;
	}
	*Send = 1;
    }
}

uint8_t Debug_ACK()
{
    return *++_DebugsendPoint;
}
