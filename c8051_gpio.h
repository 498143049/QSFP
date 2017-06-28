#include "c8051f330.h"

#ifndef __C8051_GOPIO_H
#define __C8051_GOPIO_H

void GPIO_Init(void); //C8051引脚的初始化
void ADC0_Init();
void gpio_monitor();
uint16_t sample_adc_value(int8_t times); //sample

#define ADC_start() AD0BUSY = 1

#define measureTimes 5

#define one_two()  \
    RX_write(0x40, 0x28); \
    RX_write(0x41, 0x2c); \
    RX_write(0x42, 0x28); \
    RX_write(0x43, 0x28); \
    RX_write(0x01, 0x04);
#define two_three() \
    RX_write(0x40, 0x28);  \
    RX_write(0x41, 0x28);  \
    RX_write(0x42, 0x2c);  \
    RX_write(0x43, 0x28);  \
    RX_write(0x01, 0x04);

#define three_four() \
    RX_write(0x40, 0x28);   \
    RX_write(0x41, 0x28);   \
    RX_write(0x42, 0x28);   \
    RX_write(0x43, 0x2c);   \
    RX_write(0x01, 0x04);

#define four_five() \
    RX_write(0x40, 0x28);  \
    RX_write(0x41, 0x28);  \
    RX_write(0x42, 0x28);  \
    RX_write(0x43, 0x28);  \
    RX_write(0x01, 0x05);

#define five_six() \
	 AMX0P = 0x0D; 		

#define six_one() \
	AMX0P = 0x07;         \
	RX_write(0x40, 0x2c); \
    RX_write(0x41, 0x28); \
    RX_write(0x42, 0x28); \
    RX_write(0x43, 0x28); \
    RX_write(0x01, 0x04);


	
sbit ModPrsl = P2 ^ 4;
sbit IntL = P2 ^ 3;
sbit ReseL = P2 ^ 2;
sbit ModseL = P2 ^ 1;
sbit LPMode = P2 ^ 0;

#endif /* __C8051_GOPIO_H */