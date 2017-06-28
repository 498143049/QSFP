#include "c8051f330.h"
#include "c8051_it.h"
#include "c8051_system.h"
#include "c8051_usart.h"
#include "FIFOQUEUE.h"
#include "cmd.h"
#include "c8051_smbus.h"
#include "c8051_i2c.h"
#include "QSFP_PROTOCOL.h"
#include "debug.h"
#include "QSFP_Mem.h"
#include "c8051_gpio.h"
#include "stdio.h"
#include "assert.h"

iuint8_t control_flag_change1 = 0;

//SM_BUS need source
iuint8_t smbus_reciver_buff[10];
iuint8_t reciver_buff_size = 0;
iuint8_t smbus_status = 0;
iuint8_t _currentAddress = 0; //recode the Inter_Address

extern iuint8_t uart_send_flag;
extern iuint8_t start_adc_flag;
extern iuint32_t intl_status;
extern iuint32_t intl_status_old;
extern iuint32_t times;

//-----------------------------------------------------------------------------
// Timer3_Init()
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Timer3 configured for use by the SMBus low timeout detect feature as
// follows:
// - Timer3 in 16-bit auto-reload mode
// - SYSCLK/12 as Timer3 clock source
// - Timer3 reload registers loaded for a 25ms overflow period
// - Timer3 pre-loaded to overflow after 25ms
// - Timer3 enabled
void Timer3_Init(void)
{
    TMR3CN = 0x00; // Timer3 configured for 16-bit auto-
		   // reload, low-byte interrupt disabled

    CKCON &= ~0x40; // Timer3 uses SYSCLK/12

    TMR3RL = -(SYSCLK / 12 / 40); // Timer3 configured to overflow after
    TMR3 = TMR3RL;		  // ~25ms (for SMBus low timeout detect):
				  // 1/.025 = 40
}
/************************************************
*	�������ƣ�Timer2_Init
*	����������Timer2�ĳ�ʼ��
*	��    ������
*   �� �� ֵ����
*   ��    ��: 
*************************************************/
void Timer2_Init()
{
    TMR2CN = 0x00;  //����Ϊϵͳʱ�ӵ�1/12
    CKCON &= ~0x20; //���ø�2λΪ��.

    TMR2 = - - SYSCLK / 12 / 40; //��ʱ25ms����
    TMR2RL = -SYSCLK / 12 / 40;

    TR2 = 0;
}
void Timer3_ISR(void) interrupt 14
{
    SMB0CF &= ~0x80; // Disable SMBus
    SMB0CF |= 0x80;  // Re-enable SMBus

    TMR3CN &= ~0x80; // Clear Timer3 interrupt-pending flag
}
/************************************************
*	�������ƣ�USART0_ISR
*	����������USART0_ISR�жϷ������
*	��    ������
*   �� �� ֵ����
*   ��    ��: 
*************************************************/
/*void USART0_ISR(void) interrupt 4
{

    //	unsigned char RX_Data;
    ES0 = 0; //����UART0�ж�
    if (RI0)
    {
	RI0 = 0;
    }
    else
	TI0 = 0;
    ES0 = 1; //����UART0�ж�
}*/

/************************************************
*	�������ƣ�Timer2_ISR
*	����������Timer2_ISR�жϷ������
*	��    ������
*   �� �� ֵ����
*   ��    ��: �ű� ���� ADC�жϣ�ADC2��������ADC������
*************************************************/
void Timer2_ISR(void) interrupt 5
{
    //�������ִ���жϺ���

    TMR2CN &= ~0x80; // Clear Timer2 interrupt-pending flag
    //gpio_monitor();  //����ⲿ���ű仯
    //Interrupt status change
    /*	IntL = 1;
	if(intl_status!=intl_status_old)
	{
		intl_status_old = intl_status;
		IntL = 0;
	}*/
    //25ms
    times++;
    //100ms ִ��һ��adc
    if ((times % 4) == 0 && start_adc_flag == 0)
		start_adc_flag = 1;

    //1sִ��һ��uart_send
    if ((times % 40) == 0 && uart_send_flag == 0)
		uart_send_flag = 1;
}

//���ö�ʱ�� 0
void Timer0_Init(void)
{

// Make sure the Timer can produce the appropriate frequency in 8-bit mode
// Supported SMBus Frequencies range from 10kHz to 100kHz.  The CKCON register
// settings may need to change for frequencies outside this range.
#if ((SYSCLK / SMB_FREQUENCY / 3) < 255)
#define SCALE 1
    CKCON |= 0x04; // Timer0 clock source = SYSCLK
#elif ((SYSCLK / SMB_FREQUENCY / 4 / 3) < 255)
#define SCALE 4
    CKCON |= 0x01;
    CKCON &= ~0x06; // Timer0 clock source = SYSCLK / 4
#endif

    TMOD |= 0x02; // Timer0 n 8-bit auto-reload mode

    // Timer1 configured to overflow at 1/3 the rate defined by SMB_FREQUENCY
    TH0 = -(SYSCLK / SMB_FREQUENCY / SCALE / 3);

    TL0 = TH0; // Init Timer0

    TR0 = 1; // Timer0 enabled
}

void SMBus_ISR(void) interrupt 7
{
    if (ARBLOST == 0)
    {
	switch (SMB0CN & 0xF0)
	{
	case SMB_SRADD: //IninFun  //strat
	    STA = 0;    // Clear STA bit
	    switch (SMB0DAT & 0xFE)
	    {

	    case QSFP_SLAVE_ADDRESS:
		ACK = 1;
		if (smbus_status == pending)
		{
		    if (BIT_GET(SMB0DAT, 0))
		    {
			smbus_status = qsfp_mode | reading; //read
			SMB0DAT = QSFP_GetData(_currentAddress);
			_currentAddress++;
		    }
		    else
			smbus_status = qsfp_mode | writeing; //write
		}
		else
		{
		    if ((reciver_buff_size == 1) && BIT_GET(SMB0DAT, 0) && (smbus_status & qsfp_mode)) //the seacand operate is read.
		    {
				smbus_status |= reading;
				SMB0DAT = QSFP_GetData(smbus_reciver_buff[0]);
				smbus_reciver_buff[0]++;
		    }
		    //Э�����治���ܷ���  //����prinft
		    //assert(1!=1);
		}
		break;
	    case DEBUG_SLAVE_ADDRESS:
		ACK = 1;
		if (smbus_status == pending)
		{
		    if (BIT_GET(SMB0DAT, 0))
		    {
			smbus_status = debug_mode | reading; //read
			SMB0DAT = DEBUG_GetData(_currentAddress++);
		    }
		    else
			smbus_status = qsfp_mode | writeing; //write
		}
		else
		{
		    if (BIT_GET(SMB0DAT, 0) && (smbus_status & debug_mode)) //the seacand operate is read.
		    {
				smbus_status |= reading;
				SMB0DAT = DEBUG_GetData(smbus_reciver_buff[0]);
				smbus_reciver_buff[0]++;
		    }
		    else //Э�����治���ܷ���  //����prinft
			 //assert(1!=1);

			break;
		default:
		    smbus_status = pending;
		    ACK = 0;
		    break;
		}
		break;
	    case SMB_SRDB:					   //�յ�����
			smbus_reciver_buff[reciver_buff_size++] = SMB0DAT; //recode value
			ACK = 1;					   //��Ӧ�յ�
			break;
	    case SMB_STDB:    //���ACK��ʾΪ1��
		if (ACK == 1) //�������������
		{
		    if (smbus_status == (qsfp_mode | writeing | reading)) //���ŷ�����,����״̬
				SMB0DAT = QSFP_GetData(smbus_reciver_buff[0]++);
		    else
				SMB0DAT = QSFP_GetData(_currentAddress++);
		}
		else
		{
		    if (smbus_status == (qsfp_mode | writeing | reading)) //���ŷ�����,����״̬
				SMB0DAT = DEBUG_GetData(smbus_reciver_buff[0] - 1);
		    else
				SMB0DAT = DEBUG_GetData(_currentAddress - 1);
		}
		break;
	    case SMB_STSTO: //��ʧ��ֱ�Ӹ�λ
	    case SMB_SRSTO:
		STO = 0; // STO must be cleared by software when a STOP is detected as a slave //�������ʱ�䴦��
		//����Ƿ�debugģʽ����Ҫ
		if (!(smbus_status & reading))
		{
		    //�ر� SMBUS�ж�
		    smbus_status |= dealwrite;
		    BIT_CLEAN(EIE1, 0);
		}
		else
		{
		    reciver_buff_size = 0;
		    smbus_status = pending;
		}
		break;
	    }
	}
    }
    else
    {
	SMBus_ReSet();
    }
    SI = 0;
}

//-----------------------------------------------------------------------------
// ADC0_ISR
//-----------------------------------------------------------------------------
//
// This ISR averages <INT_DEC> samples for each analog MUX input then prints
// the results to the terminal.  The ISR is called after each ADC conversion,
// which is triggered by Timer2.
//
//-----------------------------------------------------------------------------
//�л�״̬����

/*void ADC0_ISR() interrupt 10
{
	static   int  accumulator = 0;
	static   char measurements = measureTimes;     

	AD0INT  = 0;  //���Flag 
	

	accumulator+=ADC0;

	if(measurements<=1)
	{
		ADC_Value[status] = accumulator/(measureTimes);
		measurements = measureTimes;
		accumulator=0;
		switch(status)
		{
			case 0: 
				statusonetotwo();
				status++;
				ADC_start();  //��ʼ��һ��ת��
			break;
			case 1:
				statustwotothree();
				status++;
				ADC_start();  
			break;
			case 2:
				statusthreetofour();
				status++;
				ADC_start(); 
				break;
			case 3:
				statusfourtofive();
				status++;
				ADC_start(); 
				break;
			case 4:
				statusfivetoone();
				status = 0;
				AdcComplete = 1;
				break;
			default :
				status = 0;
				AdcComplete = 1;
			break;
		}
	}
	else
	{
		measurements--;   //�����Դ�
		ADC_start();   // hold five times 

	}
   //����5�����ݣ�ÿ�β�5��
}*/
// The interrupt pending flag is automatically cleared by vectoring to the ISR
void INT0_ISR() interrupt 0
{
    //RX �ж�
    //	interrupt_rx();
    BIT_SET(control_flag_change1, 1);
}
// The interrupt pending flag is automatically cleared by vectoring to the ISR
void INT1_ISR() interrupt 2
{
    //TX �ж�
    //
    BIT_SET(control_flag_change1, 2);
}
