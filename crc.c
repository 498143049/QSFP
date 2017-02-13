#include "types.h"
#include "crc.h"

/************************************************
*	�������ƣ�crc_feed
*	����������crcУ�������
*	��    ������
*   �� �� ֵ����
*   ��    ��:  
*************************************************/
unsigned char *tempaddr;
uint16_t crc_feed(unsigned char *addr, int num, uint16_t crc) 
{
    int i;  
	tempaddr=addr;
    for (; num > 0; num--)              /* Step through bytes in memory */  
    {  
        crc = crc ^ (*addr++);     /* Fetch byte from memory, XOR into CRC top byte*/  
       	for( i = 8; i != 0; i-- )
		{
			if( ( (crc) & 0x0001 ) != 0 ) 
			{
				crc >>= 1;
				crc ^= 0xA001;
			} 
			else
			{
				crc >>= 1;
			}
		}
    }                               /* Loop until num=0 */ 
	addr= tempaddr;
    return(crc);                    /* Return updated CRC */  
}  