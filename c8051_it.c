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
//
void Timer3_Init (void)
{
   TMR3CN = 0x00;                      // Timer3 configured for 16-bit auto-
                                       // reload, low-byte interrupt disabled

   CKCON &= ~0x40;                     // Timer3 uses SYSCLK/12

   TMR3RL = -(SYSCLK/12/40);           // Timer3 configured to overflow after
   TMR3 = TMR3RL;                      // ~25ms (for SMBus low timeout detect):
                                       // 1/.025 = 40

   EIE1 |= 0x80;                       // Timer3 interrupt enable
   TMR3CN |= 0x04;                     // Start Timer3
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
    TMR2CN = 0x00;   		//����Ϊϵͳʱ�ӵ�1/12    
 	CKCON  &= ~0x20; 		//���ø�2λΪ��.

	//TMR2 = -(SYSCLK/12)*BITNUMS/2/BAUDRATE; //���㶨ʱʱ��                  
    //TMR2RL = -(SYSCLK/12)*BITNUMS/2/BAUDRATE;  
	TMR2=- -SYSCLK/12/50;    //��ʱ1s����
	TMR2RL=-SYSCLK/12/50;       
	TR2=0; 
}
void Timer3_ISR (void) interrupt 14
{
   SMB0CF &= ~0x80;                    // Disable SMBus
   SMB0CF |= 0x80;                     // Re-enable SMBus
   TMR3CN &= ~0x80;                    // Clear Timer3 interrupt-pending flag
}
/************************************************
*	�������ƣ�USART0_ISR
*	����������USART0_ISR�жϷ������
*	��    ������
*   �� �� ֵ����
*   ��    ��: 
*************************************************/
void USART0_ISR (void) interrupt 4
{

 //	unsigned char RX_Data;  
	ES0 = 0;  //����UART0�ж�
	if(RI0)
	{

		RI0 = 0; 
	}
	else
		TI0=0;
	ES0 = 1;  //����UART0�ж�
}
sbit Tx_int=P1^1;
sbit Rx_int=P1^6;  
/************************************************
*	�������ƣ�Timer2_ISR
*	����������Timer2_ISR�жϷ������
*	��    ������
*   �� �� ֵ����
*   ��    ��: 
*************************************************/
void Timer2_ISR (void) interrupt 5
{  
	//�������ִ���жϺ��� 
	if(Tx_int==1)
	{
		interruptforTx();
	}
	if(Tx_int==1)
	{
		interruptforRx();
	}
	TF2H=0;
}
void Timer1_Init (void)
{

// Make sure the Timer can produce the appropriate frequency in 8-bit mode
// Supported SMBus Frequencies range from 10kHz to 100kHz.  The CKCON register
// settings may need to change for frequencies outside this range.
#if ((SYSCLK/SMB_FREQUENCY/3) < 255)
   #define SCALE 1
      CKCON |= 0x08;                   // Timer1 clock source = SYSCLK
#elif ((SYSCLK/SMB_FREQUENCY/4/3) < 255)
   #define SCALE 4
      CKCON |= 0x01;
      CKCON &= ~0x0A;                  // Timer1 clock source = SYSCLK / 4
#endif

   TMOD = 0x20;                        // Timer1 in 8-bit auto-reload mode

   // Timer1 configured to overflow at 1/3 the rate defined by SMB_FREQUENCY
   TH1 = -(SYSCLK/SMB_FREQUENCY/SCALE/3);

   TL1 = TH1;                          // Init Timer1

   TR1 = 1;                            // Timer1 enabled
}

//�����һ����ΪB0
uint8_t xdata IsSend=0;
uint8_t xdata SendSingleValue=0; 
uint8_t xdata InterBeginTime=0; 
uint8_t xdata SMBUSFIFOReciver[10]={0};
uint8_t xdata  ReiciverbufferSize=0;
uint8_t xdata CurrentSlave=0;
void SMBus_ISR (void) interrupt 7
{
    if (ARBLOST == 0)
    {
        switch (SMB0CN & 0xF0)           
        {
        	case  SMB_SRADD:        //IninFun
				STA = 0;                   // Clear STA bit
				InterBeginTime++;
				switch(SMB0DAT&0xFE)
				{

					case QSFP_SLAVE_ADDRESS:
						CurrentSlave=QSFP_SLAVE_ADDRESS;
					    ACK=1;
						if(InterBeginTime==1)
							QSFP_InitFunOne(SMB0DAT,&IsSend,&SendSingleValue);
						else if(InterBeginTime==2)
							QSFP_InitFunTwo(SMB0DAT,&IsSend,&SendSingleValue);
						break;
					case DEBUG_SLAVE_ADDRESS:
						ACK=1;
						Debug_InitFun(&IsSend,&SendSingleValue);
						CurrentSlave=DEBUG_SLAVE_ADDRESS;
						break; 
					default:
						CurrentSlave=0;
						ACK=0;
						IsSend=0;
						break;
				}
				if(IsSend)
				{

					SMB0DAT=SendSingleValue;
					IsSend=0;
				}
            	break;
			case  SMB_SRDB:
					SMBUSFIFOReciver[ReiciverbufferSize++] = SMB0DAT;     //recode value 
					if(CurrentSlave==DEBUG_SLAVE_ADDRESS)
					{
						Debug_ReciverValue(&IsSend,&SendSingleValue);
					}
					if(IsSend)
					{
						SMB0DAT=SendSingleValue;
						IsSend=0;
					}
					ACK=1;     //��Ӧ�յ�
				break;
			case  SMB_STDB:                //���ACK��ʾΪ1��
				if(ACK == 1)               //������������� 
				{	
					switch(CurrentSlave)
					{
						case QSFP_SLAVE_ADDRESS:
							SMB0DAT=QSFP_ACK();
							break;
						case DEBUG_SLAVE_ADDRESS:
							SMB0DAT=Debug_ACK();
							break;
					}
				}
				break;
			case  SMB_STSTO:  //��ʧ��ֱ�Ӹ�λ
			case  SMB_SRSTO:
				//����Ƿ�debugģʽ����Ҫ
				switch(CurrentSlave)
				{
					case QSFP_SLAVE_ADDRESS:
						QSFPStop();
				}
				ReiciverbufferSize=0;
				InterBeginTime=0;
				CurrentSlave=0;
				STO = 0;                   // STO must be cleared by software when a STOP is detected as a slave //�������ʱ�䴦��
           		break;
        }	

    }
    else
    {
		SMBus_ReSet();
    }
   SI=0;                                     
}