#ifndef __PARAMETERS_H__
#define __PARAMETERS_H__

//#include <stdint.h>
#include "types.h"

uint8_t * GetParameter(uint16_t SetAddress,uint8_t num);
uint8_t DelecetParmeter(uint16_t SetAddress,uint8_t num);
uint8_t ChangeParmeter(uint16_t SetAddress,uint8_t num,uint8_t address,uint8_t value);
uint8_t AddParameter(uint16_t SetAddress,uint8_t address,uint8_t value);
void Init_TX_Parameters();
void Init_Rx_Parameters();
void Init_User_define();
uint16_t GetAddress(uint8_t offset);
#endif
