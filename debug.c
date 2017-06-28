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
    if (ReiciverbufferSize != 0) //�����¼����Կ
    {
	_buffercount = (SMBUSFIFOReciver[0] & 0x07);
	if ((SMBUSFIFOReciver[0] >> 3) == DebugKey)
	{
	    ReiciverbufferSize = 0;
	}
    }
    *IsSend = 0;
    *Value = 0; //����������
}
void Debug_ReciverValue(uint8_t *Send, uint8_t *Value)
{
    if (_buffercount == ReiciverbufferSize) //���ȷ������ģʽ����Կ��֤�ɹ�
    {
	//����������ݽ���
	struct ReciverCmd dealNow;
	uint16_t address;
	uint8_t *point;
	praseTostr(&dealNow, SMBUSFIFOReciver, ReiciverbufferSize); //�������Ϊһ���ṹ��
								    //��������ִ����
	switch (dealNow.fun)
	{
	//��д�Ĵ�������
	case ReadIICSingle:
	    adc = GetAccerssReadFun(BYTE_HIGH(dealNow.addr));
	    Getvalue = (*adc)(BYTE_LOW(dealNow.addr));
	    //��ȡ��ֵ��һ������������ݸ�ʽ /
	    *Value = Fillsinglebyte(Getvalue, Sendbuff, &dealNow); //�������ݸ�ʽ����
	    _DebugsendPoint = Sendbuff;
	    break;
	//д����
	case WriteIICSingle:
	    Getvalue = (*GetAccerssWriteFun(BYTE_HIGH(dealNow.addr)))(BYTE_LOW(dealNow.addr), dealNow.dataValue[0]);
	    *Value = Fillsinglebyte(Getvalue, Sendbuff, &dealNow); //�������ݸ�ʽ����
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
		*Value = Fillsinglebyte(50, Sendbuff, &dealNow); //�������ݸ�ʽ����
		_DebugsendPoint = Sendbuff;
	    }
	    break;
	case DeleteParamete:
	    address = GetAddress(BYTE_HIGH(dealNow.addr));
	    if (!address)
	    {
		DelecetParmeter(address, dealNow.dataValue[0]);
		*Value = Fillsinglebyte(50, Sendbuff, &dealNow); //�������ݸ�ʽ����
		_DebugsendPoint = Sendbuff;
	    }
	    break;
	case ChangeParamete:
	    address = GetAddress(BYTE_HIGH(dealNow.addr));
	    if (!address)
	    {
		ChangeParmeter(address, BYTE_LOW(dealNow.addr), dealNow.dataValue[0], dealNow.dataValue[1]);
		*Value = Fillsinglebyte(50, Sendbuff, &dealNow); //�������ݸ�ʽ����
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
				//��ȡ����Ȼ���ڻ�ȡ
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
