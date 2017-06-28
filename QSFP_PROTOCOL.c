#include "QSFP_PROTOCOL.h"
#include "QSFP_Mem.h"
#include "journal.h"
#define Current 0
#define ReadNext 1

void QSFP_Write()
{
    //�ֽ�д������
    int8_t tempi = 0;
    if (_QSFP_Status == WRITE)
    {
	//Ŀǰֻ�ǿ��ǵ��ֽ�д
	if (ReiciverbufferSize == 2)
	{
	    //���ֽ�д�롣
	    OperateStatus = QSFP_writeSingledata(SMBUSFIFOReciver[0], SMBUSFIFOReciver[1]);
	    //�������ʧ�� ��¼����־��
	}
	else
	{
	    //����ж��ֽ�д�루��δ���ԣ�
	    QSFP_writedatas(SMBUSFIFOReciver[0], &SMBUSFIFOReciver[1], ReiciverbufferSize - 1);
	}
	//��־��¼
	JourBuffer[0] = ReiciverbufferSize + 3;
	JourBuffer[1] = WRITE + OperateStatus << 1; //0 ��ʾд��
	JourBuffer[2] = Get_Data(0X7F);		    //��¼��ǵ�ַ
						    //��ReiciverbufferSize ���������ȫ���ŵ�����ȥ
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
			//��һλ��¼�м��������ڶ�λ��¼�����Ƿ�ɹ��Ͳ�����𣨶�����д��������λ��¼��ȡ�ĵ�ַ ����λ��¼��ȡ�ĸ���
		JourBuffer[0]=4;
		JourBuffer[1]=1+OperateStatus<<1;
		JourBuffer[2]=Get_Data(0X7F);
		if(_READ_TYPE==ReadNext)
			JourBuffer[3]=SMBUSFIFOReciver[0];
		else
			JourBuffer[3]=_currentAddress-GetValueNum;
		JourBuffer[4]=GetValueNum;

		//����Jour
	//	AddOneJournal(JourBuffer,4);
	}
}*/
void QSFPStop()
{
    GetValueNum = 1; //��λֵ
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
	    //������֤��ͨ����
	    *Value = 0xFF;
	    return;
	}
	ReciverAddress++; //��ַ��ֵ+1��
	_READ_TYPE = ReadNext;
	*IsSend = 1;
    }
    else
    {
	_QSFP_Status = WRITE;
    }
}
//QSFP��������
uint8_t QSFP_ACK()
{
    uint8_t xdata GetTheValue;
    GetValueNum++;
    if (_READ_TYPE == Current)
    {
	OperateStatus = QSFP_GetData(_currentAddress, &GetTheValue);
	if (OperateStatus != Succeed)
	{
	    //������֤��ͨ����
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
	    //������֤��ͨ����
	    return 0xFF;
	}
	else
	{
	    ReciverAddress++;
	    return GetTheValue;
	}
    }
}
