#ifndef __QSFP_PROTOCOL_H
#define __QSFP_PROTOCOL_H

#include "types.h"

#define SLAVE_ADDRESS 0xA0  
#define WRITE 0x00 			// SMBus WRITE command
#define READ 0x01 			// SMBus READ command

#define ReadAndWrite   0x03   //readable and writeable
#define Readable       0x02   //readable and writeable
#define writeable      0x01 
#define NoReadandWrite 0x00   //No




#define PrivilegeError  0x40;  //
/********Define The Begin IRQ********************************/
void QSFP_InitFunOne(uint8_t address,uint8_t * IsSend,uint8_t *Value); 
void QSFP_InitFunTwo(uint8_t address,uint8_t * IsSend,uint8_t *Value);
/********Define The ACK****************************/
uint8_t  QSFP_ACK();

void QSFPStop();
//void QSFP_Write();
#endif