#ifndef __CMD_H__
#define __CMD_H__
#include "types.h"
/****发送命令的结构体数�?******/
#define FunCode uint8_t
#define AddressCode uint16_t
#define Data uint8_t
#define MaxLength 4
struct ReciverCmd
{
    FunCode fun;
    AddressCode addr;
    Data dataValue[MaxLength];
    // uint16_t    CRCvalue;  //���治����CRCУ��
    uint8_t Length; //出去CRC的长�?
};

typedef uint8_t ReadFunKind(uint8_t lowAddress);
typedef uint8_t WriteFunKind(uint8_t lowAddress, uint8_t DataValue);
/******����ָ��***********/

#define WriteIICSingle 0x02
#define ReadIICSingle 0x06
#define ReadParamete 0x0A
#define AddParamete 0x0C
#define DeleteParamete 0x0E
#define ChangeParamete 0x10
#define ReadJouranal 0x12
#define ReadJournalNearly 0x14

#define Help 0xFC

#define systemFail 0xFF //ϵͳ���մ���
#define crcFail 0x01    //CRCУ�����
#define CMDNone 0x02    //�Ҳ�������

#define Successful 0x00
#define Fail 0x01

/*******���ֽڵ�ַ����*********/
/*******ÿ����128���ֽڲ���******/
/*#define QSFP_1_L 0x00
#define QSFP_1_H 0x01
#define QSFP_2_H 0x02
#define QSFP_3_H 0x03
#define QSFP_4_H 0x04*/
#define QSFP 0x00
#define AddressNum 0x01
/*********ÿһ����256���ֽڲ���*****/
/*********�ɸߵ�ַ�ƶ�*************/
#define Tx_driver 0x05
#define Rx_driver 0x06
/*******����־���е����Ĳ���*******/

void praseTostr(struct ReciverCmd *cmd, uint8_t *buffer, uint8_t bufferLength);
void writeToFlash(uint8_t funCode, uint16_t Address, uint8_t Status);
void writeToParametersTable(uint8_t Paaddr, uint8_t Value);
void RetrurnHelp();
void ReadJour(uint8_t num);
void ReturnMessage(uint8_t fun, uint16_t Address, uint8_t Status, uint8_t ErrorValue, uint8_t dataValue, uint8_t dataLength);
void PrintAlarm(uint8_t type);
ReadFunKind *GetAccerssReadFun(uint8_t highAddress);
WriteFunKind *GetAccerssWriteFun(uint8_t highAddress);
uint8_t Fillsinglebyte(uint8_t GetValue, uint8_t *Sendbuff, struct ReciverCmd *cmd);
uint8_t Fillsinglebytes(uint8_t *value, uint8_t fillNum, uint8_t *Sendbuff, struct ReciverCmd *cmd);

/******/
#endif