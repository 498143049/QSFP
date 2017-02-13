#ifndef __JOURNAL_H__
#define __JOURNAL_H__

#include "types.h"

#define data_Zeroth_High_Address      0x3A00
#define data_First_High_Address     0x3800
#define data_Second_High_Address      0x3600 
#define TX_ParameterList             0x3400 
#define RX_ParameterList             0x3200
#define UserDefine_ParameterList     0x3000 
#define JourAddr                     0x2A00 
#define ConfigAddr                   0x2800 



#define securityCodeValue             242  
#define securityCodeAdd               0x00 
#define JourNum                       0x01
#define TxParametesNum                0x02 
#define RxParametesNum                0x03 
#define UserDefineParametesNum        0x04 
#define AllNum                        0xFF 



void journal_MemInit();
void AddOneJournal(uint8_t  * buffer,uint8_t  num);
void updateConfig(uint8_t Kind,uint8_t Value);
uint8_t  GetConfigNum(uint8_t kind);
uint8_t * ReadOneJour(uint8_t num);
#endif 