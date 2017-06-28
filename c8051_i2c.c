#include "c8051_i2c.h"
/************************************************
*	�������ƣ�delay_1ms
*	����������1ms��ʱ
*	��    ������
*   �� �� ֵ����
*   ��    ��:
*************************************************/
/*void delay_1ms()
{
	unsigned int i;
	for(i=3060; i>0; i--)
	{
	}
}*/

/************************************************
*	�������ƣ�delay_ms
*	��������������(ms)��������ʱ
*	��    ����unsigned int tms,��ʱt����
*   �� �� ֵ����
*   ��    ��:
*************************************************/
/*void delay_ms(unsigned int tms)
{
	for(; tms>0; tms--)
	{
		delay_1ms();
	}
}*/
/************************************************
*	�������ƣ�Delay_I2C_Temp()
*	����������I2C�е���ʱ
*	��    ����unsigned int t����Ҫ��ʱ�Ļ�������
*   �� �� ֵ����
*   ��    ��:
*************************************************/
void Delay_I2C_Temp(unsigned int t)
{
    while (t)
    {
	t--;
    }
}

/************************************************
*	�������ƣ�I2C_Delay
*	����������I2C�е���ʱ
*	��    ������
*   �� �� ֵ����
*   ��    ��:
*************************************************/
void I2C_Delay(void)
{
    Delay_I2C_Temp(5);
}

/************************************************
*	�������ƣ�I2C_Init
*	������������ʼ��I2C����
*	��    ������
*   �� �� ֵ����
*	��    �ߣ�
*************************************************/
void TX_I2C_Init(void)
{
    TX_I2C_SDA = 1; //�ͷ�I2C���ߵ�������
    TX_I2C_SCL = 1; //�ͷ�I2C���ߵ�ʱ����
}
/************************************************
*	�������ƣ�void I2C_Start(void)
*	��������������I2C����ʼ�źţ�SCLΪ�ߣ�SDA�½���
*	��    ������
*   �� �� ֵ����
*   ��    �ߣ�
*************************************************/
void TX_I2C_Start()
{
    TX_I2C_SDA = 1; //
    TX_I2C_SCL = 1; //
    I2C_Delay();    //�ӳ�5����������
    TX_I2C_SDA = 0; //SCL�ߣ�SDA�½��أ�������ʼ�ź�
    I2C_Delay();    //
    TX_I2C_SCL = 0; //
}

/************************************************
*	�������ƣ�I2C_Stop
*	��������������I2C��ֹͣ�źţ�SCLΪ�ߣ�SDA������
*	��    ������
*   �� �� ֵ����
*   ��    �ߣ�
*************************************************/
void TX_I2C_Stop()
{
    TX_I2C_SDA = 0;
    I2C_Delay();
    TX_I2C_SCL = 1;
    I2C_Delay();    //��ʱ5����������
    TX_I2C_SDA = 1; //SCLΪ�ߣ�SDA�����أ�����ֹͣ�ź�
    I2C_Delay();
}

/************************************************
*	�������ƣ�I2C_Ack
*	����������I2C�е�Ӧ���źţ�ÿ�η���8λ�ֺ󣬷��ͷ���SDA�ͷ�һ��bit��ʱ�䣬
*             �ڼ���շ���SDA���ͱ�ʾӦ��
*	��    ������
*   �� �� ֵ��bit,����ֵΪ0��ʾӦ��Ϊ1��ʾ��Ӧ��
*   ��    �ߣ�
*************************************************/
bit TX_I2C_Ack()
{
    bit ack; //�洢Ӧ���ź�
    TX_I2C_SDA = 1;
    I2C_Delay();
    TX_I2C_SCL = 1;
    I2C_Delay();
    ack = TX_I2C_SDA; //��ȡȷ���źţ�
    I2C_Delay();
    TX_I2C_SCL = 0;
    I2C_Delay();

    return ack; //0��ʾӦ��1��ʾ��Ӧ��
}

/************************************************
*	�������ƣ�I2C_Write8Bit
*	����������I2C�����е�һ���ֽ�д����������8bit����
*	��    ����unsigned char DData��д�������
*   �� �� ֵ����
*   ��    �ߣ�
*************************************************/
void TX_I2C_Write8Bit(unsigned char DData)
{
    unsigned char i;
    unsigned char temp;
    temp = DData;
    for (i = 0; i < 8; i++)
    {
	TX_I2C_SDA = temp & 0x80; //����I2CЭ������λ��ʼ����
	I2C_Delay();
	TX_I2C_SCL = 1;
	I2C_Delay();
	TX_I2C_SCL = 0;
	I2C_Delay();
	temp = temp << 1; //�����������ƣ�׼��������һλ
    }
}

/************************************************
*	�������ƣ�I2C_Read8Bit
*	����������I2C�����ж�ȡһ���ֽڣ�8bit������
*	��    ������
*   �� �� ֵ��ReadData-I2C������8bit����
*   ��    �ߣ�
*************************************************/
unsigned char TX_I2C_Read8Bit()
{
    unsigned char i;
    unsigned char ReadData;
    TX_I2C_SDA = 1;
    for (i = 0; i < 8; i++)
    {
	TX_I2C_SCL = 1;
	I2C_Delay();
	ReadData = ReadData << 1;
	I2C_Delay();
	if (TX_I2C_SDA)
	{
	    ReadData = ReadData | 0x01;
	}
	TX_I2C_SCL = 0;
	I2C_Delay();
    }
    return ReadData;
}

/***************************************************************************************
*						QSFP_MSAЭ���е�д������3�֣�
*	1���ֽ�д��д����˳��Start -> QSFP ADDR(д) -> ACK(����) -> MEMORY ADDR -> ACK(�ӻ�)
						   -> DATA -> ACK(�ӻ�) -> Stop
*	2��˳��д������˳��Start -> QSFP ADDR(д) -> ACK(����) -> MEMORY ADDR -> ACK(�ӻ�)
*						 -> DATA 1 -> ACK(����) -> DATA 2 -> ACK(����) -> DATA n -> ACK(����)
						 -> Stop
	3��ȷ�ϻ��Ƶ���ѯ��ʽд
*****************************************************************************************/
/***************************************************************************************
*						QSFP_MSAЭ���еĶ�������4�֣�
	1����ǰ��ַ�Ķ�����������ʽ��Start -> QSFP ADDR������ -> ACK(�ӻ�) -> NACK ->Stop
	2�����������������ʽ��Start -> QSFP ADDR(д) -> ACK(�ӻ�) -> MEMORY ADDR ->
						   ACK(�ӻ�) -> QSFP ADDR������ -> ACK(�ӻ�) -> DATA -> NACK 
						   -> STOP
	3���ӵ�ǰ��ַ��ʼ����������������ʽ��Start -> QSFP ADDR������ -> ACK(�ӻ�) -> DATA
							-> ACK(����) -> DATA+1 -> ACK(����) -> DATA+x ->
							NACK -> Stop
	4��������ĵ�ַ��ʼ����������������ʽ��Start -> QSFP ADDR(д) -> ACK(�ӻ�) -> 
							MEMORY ADDR -> ACK(����) -> Start -> QSFP ADDR(��) ->
							ACK(�ӻ�) -> DATA -> ACK(����) -> DATA+1 -> ACK(����)
							-> DATA+n -> NACK -> Stop
*****************************************************************************************/
/************************************************
*	�������ƣ�I2C_SingleByteWrite
*	������������ӻ�����һ���ֽڵ�����
*	��    ����Daddr-�ӻ���ַ�� Waddr-���ݵ�ַ��Data-��������
*   �� �� ֵ����
*   ��    �ߣ�
*************************************************/
void TX_I2C_SingleByteWrite(unsigned char Daddr, unsigned char Waddr, unsigned char Data)
{
    TX_I2C_Start();
    TX_I2C_Write8Bit(Daddr); //д��ӻ��豸��ַ
    TX_I2C_Ack();
    TX_I2C_Write8Bit(Waddr); //д�����ݵ�ַ
    TX_I2C_Ack();
    TX_I2C_Write8Bit(Data); //д������
    TX_I2C_Ack();
    TX_I2C_Stop(); //ֹͣ��������
}

/*void TX_I2C_BytesWrite(unsigned char Daddr, unsigned char Waddr, unsigned char *DataArray,unsigned char num)
{
	unsigned char Temp=num;
	TX_I2C_Start();
	TX_I2C_Write8Bit(Daddr); //д��ӻ��豸��ַ
	TX_I2C_Ack();
	TX_I2C_Write8Bit(Waddr); //д�����ݵ�ַ
	TX_I2C_Ack();

	while(Temp>0)
	{
		TX_I2C_Write8Bit(*DataArray++);   //д������
		TX_I2C_Ack();
		Temp--;
	}
	TX_I2C_Stop();   //ֹͣ��������
}*/
void TX_SendAck(void)
{
    TX_I2C_SDA = 0;
    I2C_Delay(); //�ӳ�5����������
    TX_I2C_SCL = 1;
    I2C_Delay(); //�ӳ�5����������
    TX_I2C_SCL = 0;
}

/************************************************
*	�������ƣ�I2C_SingleByteRead
*	����������I2C�����ж�ȡ�ӻ��е�һ���ֽ�
*	��    ����Daddr-�ӻ���ַ��Waddr-���ݵ�ַ
*   �� �� ֵ��ReadData-�����е�һ���ֽ�����
*   ��    �ߣ�
*************************************************/

unsigned char TX_I2C_SingleByteRead(unsigned char Daddr, unsigned char Waddr)
{
    unsigned char ReadData;
    TX_I2C_Start();
    TX_I2C_Write8Bit(Daddr); //д���豸��ַ
    TX_I2C_Ack();
    TX_I2C_Write8Bit(Waddr); //д�����ݲ�����ַ
    TX_I2C_Ack();
    TX_I2C_Stop();

    TX_I2C_Start();
    TX_I2C_Write8Bit(Daddr + 1); //д���豸��ַ
    TX_I2C_Ack();
    ReadData = TX_I2C_Read8Bit(); //����I2C�е�����

    TX_I2C_Stop();

    return ReadData;
}

unsigned char *TX_I2C_BytesRead(unsigned char Daddr, unsigned char Waddr, unsigned char *arry, unsigned char num)
{
    unsigned char Temp = 0;

    TX_I2C_Start();
    TX_I2C_Write8Bit(Daddr); //д���豸��ַ
    TX_I2C_Ack();
    TX_I2C_Write8Bit(Waddr); //д�����ݲ�����ַ
    TX_I2C_Ack();
    TX_I2C_Stop();

    TX_I2C_Start();
    TX_I2C_Write8Bit(Daddr + 1); //д���豸��ַ
    TX_I2C_Ack();
    *arry++ = TX_I2C_Read8Bit(); //����I2C�е�����
    for (Temp = 1; Temp < num; Temp++)
    {
	TX_SendAck();
	*arry++ = TX_I2C_Read8Bit();
    }
    TX_I2C_Stop();

    return arry;
}
