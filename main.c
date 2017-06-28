/**************************************************************
*
*	����������Silicon IDE 4.5�� Keil C51 4
*	ʹ��оƬ��C8051F311
*	�����ܣ�
*
***************************************************************/
#include "c8051f330.h"
#include "c8051_i2c.h"
#include "c8051_system.h"
#include "crc.h"
#include "c8051_usart.h"
#include "c8051_gpio.h"
#include "c8051_it.h"
#include "types.h"
#include "cmd.h"
#include "c8051_smbus.h"
#include "QSFP_Mem.h"
#include "journal.h"
#include "c8051_it.h"
#include "stdio.h"

iuint16_t adc_value[10] = {0}; //save the sysytem adc_value
iuint32_t times = 0;
iuint32_t intl_status = 0;
iuint32_t intl_status_old = 0;
iuint8_t uart_send_flag = 0;
iuint8_t start_adc_flag = 0;
iuint32_t frame_count = 0;
xuint8_t buff[50];
extern iuint8_t control_flag_change1;
extern iuint8_t smbus_status;
extern iuint8_t smbus_reciver_buff[10];
extern iuint8_t reciver_buff_size ;

/********Ŀǰ�����ö��еĽṹȥ��********/
void main()
{

    PCA0MD &= ~0x40; //��ֹ���Ź�

    /***************************ϵͳ��ʼ��************************/
    SYSCLK_Init(); //��ʼ��ʱ��Ƶ��
    GPIO_Init();   //��ʼ���˿�I/O
    UASRT0_Init(); //��ʼ������UART0

    TX_I2C_Init();
    SMBus_Init();
    ADC0_Init();

    if (journal_MemInit()) //write to the flash
		UASRT0_SendStr("Flash_init 0k!\n");
    else
		UASRT0_SendStr("Flash_init Fail!\n");

    if (RX_read(0xE0) == HXRTYPE)
		UASRT0_SendStr("HXR_init 0k!\n");
    else
		UASRT0_SendStr("HXR_init Fail!\n");

    if (TX_read(0xE0) == HXTTYPE)
		UASRT0_SendStr("HXT_init 0k!\n");
    else
		UASRT0_SendStr("HXT_init Fail!\n");

    /********************ֵ�ó�ʼ��********************************/
    QSFP_initValue(); //read_from the flash

    Timer2_Init(); //����ѭ�����Ʋ���
    Timer3_Init(); //����SMBUS �ĸ�λ  it include Time 3 started and enable .
    Timer0_Init(); //����SMBUS��������

    /********�����ж����ȼ�*************************************/
    //Interput prior configure
    //the external  Intertupt 1 external Interputur 2
    //time0 for produce the appropriate frequency
    //time1 for serial time2
    //time2 for prodece sysytime time
    //time3 configured for use by the SMBus low timeout detect feature as
    //smbus is the most important,so configured set bit . others clearn.
    IP = 0x00;
    EIP1 = 0x01; //configure the smbus is the first
		 /***************************�жϹ���************************/
    EIE1 |= 1;   //����SMBUS�ж�
    ES0 = 1;     //����UART0�ж�

    //	EX0 = 1;     // Enable /INT0 interrupts  this interrput need to test
    //  EX1 = 1;     // Enable /INT1 interrupts

    EIE1 |= 0x80;   // Timer3 interrupt enable
    TMR3CN |= 0x04; // Start Timer3

    ET2 = 1; //����ʱ��2�ж�
    TR2 = 1; //TR2��ʼ����

    EA = 1; //��ȫ�����ж�
	//start()
    TX_write(0x10, 0x20);
	TX_write(0x13, 0x20);
	TX_write(0x16, 0x20);
	TX_write(0x19, 0x20);
    TX_write(0x11, 0x20);
    /***************************����ִ��************************/
    while (1)
    {
		frame_count++;
	
	//doing all the time  //update time
	//������һ��ADC��Ҫ
	if (start_adc_flag)
	{
	    loop_detect();
	    start_adc_flag = 0;
	}

/*	switch (control_flag_change1)
	{
	case 1:
	    interrupt_rx();
	    break;
	case 2:
	    interrupt_tx();
	    break;
	default:
	    break;
	}*/
/*	if (smbus_status & dealwrite)
	{
	    if (smbus_status & qsfp_mode)
	    {
			qsfp_write_data((*smbus_reciver_buff), (smbus_reciver_buff + 1), reciver_buff_size - 1);
	    }
	    else
	    {
			qsfp_write_data((*smbus_reciver_buff), (smbus_reciver_buff + 1), reciver_buff_size - 1);
	    }

		smbus_status = pending;
		reciver_buff_size = 0;
		EIE1 |= 1;   //����SMBUS�ж�
	    //������Դ
	}*/
	//detect the interput of the;
	//detect the
	//it output debug infomation
	if (uart_send_flag)
	{
		uart_send_flag = 0;
		sprintf(buff,"frame_count: %ld, ad_0: %d, ad_1: %d, ad_2: %d, ad_3: %d, ad_4: %d , ad_5: %d  \n", frame_count, adc_value[0], adc_value[1], adc_value[2], adc_value[3], adc_value[4], adc_value[5]);
		UASRT0_SendStr(buff);
	//	printf("hellow world!");
	//	UASRT0_SendStr("ss");
	    frame_count = 0;
	}
    }
}
