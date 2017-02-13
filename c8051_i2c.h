#include "c8051f330.h"

#ifndef __C8051_I2C_H
#define __C8051_I2C_H


//����״̬�����Ķ���

sbit TX_I2C_SDA = P1^2;  //I2C���ߵ������߶˿�
sbit TX_I2C_SCL = P1^3;  //I2C���ߵ�ʱ���߶˿�

sbit RX_I2C_SDA = P1^4;  //I2C���ߵ������߶˿�
sbit RX_I2C_SCL = P1^5;  //I2C���ߵ�ʱ���߶˿�


#define HXR6104Address 0x98
#define HXT6104Address 0xD6

unsigned char RX_I2C_SingleByteRead(unsigned char Daddr, unsigned char Waddr);
void TX_I2C_Init(void); //I2C�ĳ�ʼ��
void RX_I2C_Init(void);

void TX_I2C_SingleByteWrite(unsigned char Daddr, unsigned char Waddr, unsigned char Data);
void RX_I2C_SingleByteWrite(unsigned char Daddr, unsigned char Waddr, unsigned char Data);
unsigned char TX_I2C_SingleByteRead(unsigned char Daddr, unsigned char Waddr);

void delay_ms(unsigned int tms);
unsigned char * TX_I2C_BytesRead(unsigned char Daddr, unsigned char Waddr,unsigned char * arry,unsigned char num);
unsigned char *  RX_I2C_BytesRead(unsigned char Daddr, unsigned char Waddr,unsigned char * arry,unsigned char num);
/*
void RX_I2C_BytesWrite(unsigned char Daddr, unsigned char Waddr, unsigned char *DataArray,unsigned char num); //RX���ֽ�д��಻�ܳ���3��
void TX_I2C_BytesWrite(unsigned char Daddr, unsigned char Waddr, unsigned char *DataArray,unsigned char num); //RX���ֽ�д��಻�ܳ���3��
*/
#endif /* __C8051_I2C_H */