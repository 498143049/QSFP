#include "c8051f330.h"
#include "types.h"
#include "c8051_system.h"
#include "c8051_smbus.h"
#include "c8051_gpio.h"
#include "c8051_usart.h"
/************************************************
*	函数名称：GPIO_Init
*	功能描述：configured the port funciton 
*             configured uartPort Adc p2.5 p1.7 port P1.2 P1.3 simulate I2C port P0.2 P0.7 extern intertput smbus 
*   作    者: dubing
*************************************************/
void GPIO_Init(void)
{
    //P0.1 P0.2 configured as the SDL SCK so output
    P0MDIN = 0xFF;
    P0MDOUT = 0x00;
    P0MDOUT |= 0x10;
    P0 = 0xff;

    P1MDIN = 0xFF;
    P1MDOUT = 0x08;
    P1 = 0xff;
    //P1MDIN &= ~0x10;

    P0SKIP = 0xCC;
    //configure P2.4_ModeseL P2.2_Reset P2.0_LPMode as Input
    //default is digtal Input ,so do not write any code

    //enable other config
    XBR0 = 0x05; //0000 0101   //enable uart and smbus
    XBR1 = 0x40; //0100 0000   // Enable crossbar and weak pull-ups

    //configured the ADC Interrupt
    P1SKIP |= 0x80; // configured P1.7 Skill it
    //P2MDIN &= 0xDF;      // configured P2.5 as an analog input.
    P1MDIN &= 0x7F; // configured P1.7 as an analog input

    //configured the external Interrpution
    P0SKIP |= 0x84; //configured P0.2 and P0.7 skill it 1000 0100
    INT01CF = 0x27; //configured as 0 010 0 111
    TCON |= 0x05;   //config边沿触发
}
/************************************************
*	函数名称：ADC_数据采集 10位 对于的ADC的引脚需要配置 为模拟输入，交叉开关需要跳过。
*   启动ADC的方式为给的启动。 有专门的ADC中断进行处理。
*	功能描述：ADC 数据采集    //数据手册在 C8051F330 ZH 的32页 P2.5 和 P0.6 
*	参    
*   返 回 值：无
*   作    者: 
*************************************************/
void ADC0_Init()
{
    ADC0CN = 0x00; // ADC0 disabled, normal tracking, 向ADC0写1时启动转换

    REF0CN = 0x0A; // Enable on-chip VREF, select Vdd as
                   // voltage reference.
    AMX0P = 0x07;  //先选择VDD     0D               // ADC0 initial positive input = P2.5 0XOD OXO7

    AMX0N = 0x1F; // ADC0 negative input = GND    N端选择地
                  // i.e., single ended mode

    ADC0CF = ((SYSCLK / 3000000) - 1) << 3; // Set SAR clock to 3MHz

    ADC0CF |= 0x00; // Right-justify results 右对齐

    // BIT_CLEAN(EIE1,3);
    // EIE1 &= ^0x08;                       // disable ADC0 EOC interrupt

    AD0EN = 1; // Enable ADC0
}
/************************************************
*	函数名称：gpio_detect
*	功能描述：it aims to monitor the extern pin status and respond while the change the status 
*	参    数：
*   返 回 值：noee 
*   作    者：dubing
*************************************************/
/*
sbit ModPrsl  = P2^4;
sbit IntL     = P2^3;
sbit ReseL    = P2^2;
sbit ModseL   = P2^1;
sbit LPMode   = P2^0;
*/
void gpio_monitor()
{
    if (!ReseL)
        reset_system();
    if (!ModseL)
        EIE1 |= 0;
    else
    {
        //复位
        SMBus_ReSet();
        EIE1 |= 1;
    }
}
/************************************************
*	函数名称：sampleAdcValue
*	功能描述：waiting to get AdcValue  
*	参    数：times sample times 
*   返 回 值：true value 
*   作    者：dubing
*************************************************/
uint16_t sample_adc_value(int8_t times)
{
    iuint16_t adc_value = 0;
    iint8_t times_backup = times;
    //启动ADC
    while (times--)
    {
        ADC_start(); //start_Adc
        while (AD0INT == 0)
            ; //wait ADC to success

        AD0INT = 0;
        adc_value += ADC0;
    }
    return adc_value / times_backup;
}
