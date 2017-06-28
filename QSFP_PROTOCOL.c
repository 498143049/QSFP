#include "QSFP_PROTOCOL.h"
#include "QSFP_Mem.h"
#include "journal.h"
#define Current 0
#define ReadNext 1

void QSFP_Write()
{
    //字节写的命令
    int8_t tempi = 0;
    if (_QSFP_Status == WRITE)
    {
	//目前只是考虑单字节写
	if (ReiciverbufferSize == 2)
	{
	    //单字节写入。
	    OperateStatus = QSFP_writeSingledata(SMBUSFIFOReciver[0], SMBUSFIFOReciver[1]);
	    //插入插入失败 记录到日志中
	}
	else
	{
	    //则进行多字节写入（暂未测试）
	    QSFP_writedatas(SMBUSFIFOReciver[0], &SMBUSFIFOReciver[1], ReiciverbufferSize - 1);
	}
	//日志记录
	JourBuffer[0] = ReiciverbufferSize + 3;
	JourBuffer[1] = WRITE + OperateStatus << 1; //0 表示写入
	JourBuffer[2] = Get_Data(0X7F);		    //记录标记地址
						    //将ReiciverbufferSize 里面的数据全部放到里面去
	tempi = ReiciverbufferSize - 1;
	while (--tempi >= 0)
	{
	    JourBuffer[3 + tempi] = SMBUSFIFOReciver[tempi];
	}
	//	AddOneJournal(JourBuffer,ReiciverbufferSize);
    }
}
/*void ReadOperateRecordInJour()
{
	if(	_QSFP_Status==READ)
	{
			//第一位记录有几个数，第二位记录操作是否成功和操作类别（读或者写），第三位记录读取的地址 第四位记录读取的个数
		JourBuffer[0]=4;
		JourBuffer[1]=1+OperateStatus<<1;
		JourBuffer[2]=Get_Data(0X7F);
		if(_READ_TYPE==ReadNext)
			JourBuffer[3]=SMBUSFIFOReciver[0];
		else
			JourBuffer[3]=_currentAddress-GetValueNum;
		JourBuffer[4]=GetValueNum;

		//插入Jour
	//	AddOneJournal(JourBuffer,4);
	}
}*/
void QSFPStop()
{
    GetValueNum = 1; //复位值
    //parse the buff and enter the buff to deal queue
    //reeset the value
    ReadOperateRecordInJour();
    QSFP_Write();
}

//when the begin interrupt the funciton is called
void QSFP_InitFunOne(uint8_t address, uint8_t *IsSend, uint8_t *Value)
{
    if ((address & 0x01) == READ)
    {

	OperateStatus = QSFP_GetData(_currentAddress, Value);
	if (OperateStatus != Succeed)
	{
	    *Value = 0xFF;
	    return;
	}
	_READ_TYPE = Current;
	_currentAddress++;
	*IsSend = 1;
    }
    else
    {
	_QSFP_Status = WRITE;
    }
}
void QSFP_InitFunTwo(uint8_t address, uint8_t *IsSend, uint8_t *Value)
{
    if ((address & 0x01) == READ)
    {
	ReciverAddress = SMBUSFIFOReciver[0];
	OperateStatus = QSFP_GetData(ReciverAddress, Value);
	if (OperateStatus != Succeed)
	{
	    //由于验证不通过。
	    *Value = 0xFF;
	    return;
	}
	ReciverAddress++; //地址的值+1；
	_READ_TYPE = ReadNext;
	*IsSend = 1;
    }
    else
    {
	_QSFP_Status = WRITE;
    }
}
//QSFP返回数据
uint8_t QSFP_ACK()
{
    uint8_t xdata GetTheValue;
    GetValueNum++;
    if (_READ_TYPE == Current)
    {
	OperateStatus = QSFP_GetData(_currentAddress, &GetTheValue);
	if (OperateStatus != Succeed)
	{
	    //由于验证不通过。
	    return 0xFF;
	}
	else
	{
	    _currentAddress++;
	    return GetTheValue;
	}
    }
    else
    {
	OperateStatus = QSFP_GetData(ReciverAddress, &GetTheValue);
	if (OperateStatus != Succeed)
	{
	    //由于验证不通过。
	    return 0xFF;
	}
	else
	{
	    ReciverAddress++;
	    return GetTheValue;
	}
    }
}
