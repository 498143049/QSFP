#include "c8051f330.h"

#ifndef __C8051_I2C_H
#define __C8051_I2C_H

//结束状态向量的定义

sbit TX_I2C_SDA = P1 ^ 2; //I2C总线的数据线端口
sbit TX_I2C_SCL = P1 ^ 3; //I2C总线的时钟线端口

sbit RX_I2C_SDA = P1 ^ 2; //I2C总线的数据线端口
sbit RX_I2C_SCL = P1 ^ 3; //I2C总线的时钟线端口

#define HXT6104Address 0xD6
#define HXR6104Address 0x98

#define TX_read(Waddr) TX_I2C_SingleByteRead(HXT6104Address, Waddr)
#define RX_read(Waddr) TX_I2C_SingleByteRead(HXR6104Address, Waddr)

#define TX_write(Waddr, Data) TX_I2C_SingleByteWrite(HXT6104Address, Waddr, Data)
#define RX_write(Waddr, Data) TX_I2C_SingleByteWrite(HXR6104Address, Waddr, Data)

#define TX_reads(Waddr, arry, num) TX_I2C_BytesRead(HXT6104Address, Waddr, arry, num)
#define RX_reads(Waddr, arry, num) TX_I2C_BytesRead(HXR6104Address, Waddr, arry, num)

void TX_I2C_Init(void); //I2C的初始化

unsigned char TX_I2C_SingleByteRead(unsigned char Daddr, unsigned char Waddr);

void TX_I2C_SingleByteWrite(unsigned char Daddr, unsigned char Waddr, unsigned char Data);

unsigned char *TX_I2C_BytesRead(unsigned char Daddr, unsigned char Waddr, unsigned char *arry, unsigned char num);

void delay_ms(unsigned int tms);

/*
void RX_I2C_BytesWrite(unsigned char Daddr, unsigned char Waddr, unsigned char *DataArray,unsigned char num); //RX多字节写最多不能超过3个
void TX_I2C_BytesWrite(unsigned char Daddr, unsigned char Waddr, unsigned char *DataArray,unsigned char num); //RX多字节写最多不能超过3个
*/
#endif /* __C8051_I2C_H */