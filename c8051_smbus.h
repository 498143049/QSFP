#include "c8051f330.h"

#ifndef __C8051_SMBUS_H
#define __C8051_SMBUS_H

// #define SMB_FREQUENCY   50000  //目标sci时钟频率 SMBUS从机不需要。

// Status vector - top 4 bits only
#define SMB_SRADD 0x20      // (SR) slave address received
#define SMB_SRSTO 0x10      // (SR) STOP detected while SR or ST,
#define SMB_SRDB 0x00       // (SR) data byte received,
#define SMB_STDB 0x40       // (ST) data byte transmitted
#define SMB_STSTO 0x50      // (ST) STOP detected during a
                            //    transaction; bus error
#define SMB_FREQUENCY 10000 // Target SMBus frequency
//Slave Address

#define QSFP_SLAVE_ADDRESS 0xA0
#define DEBUG_SLAVE_ADDRESS 0xAA

void SMBus_Init();

void SMBus_ReSet();
#endif /* __C8051_SMBUS_H */