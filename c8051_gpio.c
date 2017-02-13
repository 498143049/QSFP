#include "c8051f330.h"

/************************************************
*	�������ƣ�GPIO_Init
*	����������GPIO_Init�ĳ�ʼ��
*	��    ������
*   �� �� ֵ����
*   ��    ��: 
*************************************************/
void GPIO_Init(void)
{
   P0MDIN  = 0xFF;                           
   P0MDOUT = 0x00;                           
   P0MDOUT|= 0x10;                           
   P0      = 0xff;                               


   P1MDIN  = 0xFF;                           
   P1MDOUT = 0x08;                           
   P1      = 0xff;  
   //P1MDIN &= ~0x10;

   P0SKIP= 0xCC;
   XBR0    = 0x05;  //0000 0101
   XBR1    = 0x40;  //0100 0000 
}