#include "c8051f330.h"
#include "types.h"
#include "c8051_system.h"
#include "c8051_smbus.h"
#include "c8051_gpio.h"
#include "c8051_usart.h"
/************************************************
*	�������ƣ�GPIO_Init
*	����������configured the port funciton 
*             configured uartPort Adc p2.5 p1.7 port P1.2 P1.3 simulate I2C port P0.2 P0.7 extern intertput smbus 
*   ��    ��: dubing
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
    TCON |= 0x05;   //config���ش���
}
/************************************************
*	�������ƣ�ADC_���ݲɼ� 10λ ���ڵ�ADC��������Ҫ���� Ϊģ�����룬���濪����Ҫ������
*   ����ADC�ķ�ʽΪ���������� ��ר�ŵ�ADC�жϽ��д���
*	����������ADC ���ݲɼ�    //�����ֲ��� C8051F330 ZH ��32ҳ P2.5 �� P0.6 
*	��    
*   �� �� ֵ����
*   ��    ��: 
*************************************************/
void ADC0_Init()
{
    ADC0CN = 0x00; // ADC0 disabled, normal tracking, ��ADC0д1ʱ����ת��

    REF0CN = 0x0A; // Enable on-chip VREF, select Vdd as
                   // voltage reference.
    AMX0P = 0x07;  //��ѡ��VDD     0D               // ADC0 initial positive input = P2.5 0XOD OXO7

    AMX0N = 0x1F; // ADC0 negative input = GND    N��ѡ���
                  // i.e., single ended mode

    ADC0CF = ((SYSCLK / 3000000) - 1) << 3; // Set SAR clock to 3MHz

    ADC0CF |= 0x00; // Right-justify results �Ҷ���

    // BIT_CLEAN(EIE1,3);
    // EIE1 &= ^0x08;                       // disable ADC0 EOC interrupt

    AD0EN = 1; // Enable ADC0
}
/************************************************
*	�������ƣ�gpio_detect
*	����������it aims to monitor the extern pin status and respond while the change the status 
*	��    ����
*   �� �� ֵ��noee 
*   ��    �ߣ�dubing
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
        //��λ
        SMBus_ReSet();
        EIE1 |= 1;
    }
}
/************************************************
*	�������ƣ�sampleAdcValue
*	����������waiting to get AdcValue  
*	��    ����times sample times 
*   �� �� ֵ��true value 
*   ��    �ߣ�dubing
*************************************************/
uint16_t sample_adc_value(int8_t times)
{
    iuint16_t adc_value = 0;
    iint8_t times_backup = times;
    //����ADC
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
