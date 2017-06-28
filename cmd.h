#ifndef __CMD_H__
#define __CMD_H__
#include "types.h"
/****鍙戦�佸懡浠ょ殑缁撴瀯浣撴暟缁?******/
#define FunCode uint8_t
#define AddressCode uint16_t
#define Data uint8_t
#define MaxLength 4
struct ReciverCmd
{
    FunCode fun;
    AddressCode addr;
    Data dataValue[MaxLength];
    // uint16_t    CRCvalue;  //里面不含有CRC校验
    uint8_t Length; //鍑哄幓CRC鐨勯暱搴?
};

typedef uint8_t ReadFunKind(uint8_t lowAddress);
typedef uint8_t WriteFunKind(uint8_t lowAddress, uint8_t DataValue);
/******操作指令***********/

#define WriteIICSingle 0x02
#define ReadIICSingle 0x06
#define ReadParamete 0x0A
#define AddParamete 0x0C
#define DeleteParamete 0x0E
#define ChangeParamete 0x10
#define ReadJouranal 0x12
#define ReadJournalNearly 0x14

#define Help 0xFC

#define systemFail 0xFF //系统接收错误
#define crcFail 0x01    //CRC校验错误
#define CMDNone 0x02    //找不到命令

#define Successful 0x00
#define Fail 0x01

/*******高字节地址分配*********/
/*******每个组128个字节操作******/
/*#define QSFP_1_L 0x00
#define QSFP_1_H 0x01
#define QSFP_2_H 0x02
#define QSFP_3_H 0x03
#define QSFP_4_H 0x04*/
#define QSFP 0x00
#define AddressNum 0x01
/*********每一个组256个字节操作*****/
/*********由高地址制定*************/
#define Tx_driver 0x05
#define Rx_driver 0x06
/*******对日志进行单独的操作*******/

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