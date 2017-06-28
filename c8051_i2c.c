#include "c8051_i2c.h"
/************************************************
*	函数名称：delay_1ms
*	功能描述：1ms延时
*	参    数：无
*   返 回 值：无
*   作    者:
*************************************************/
/*void delay_1ms()
{
	unsigned int i;
	for(i=3060; i>0; i--)
	{
	}
}*/

/************************************************
*	函数名称：delay_ms
*	功能描述：毫秒(ms)级级别延时
*	参    数：unsigned int tms,延时t毫秒
*   返 回 值：无
*   作    者:
*************************************************/
/*void delay_ms(unsigned int tms)
{
	for(; tms>0; tms--)
	{
		delay_1ms();
	}
}*/
/************************************************
*	函数名称：Delay_I2C_Temp()
*	功能描述：I2C中的延时
*	参    数：unsigned int t，需要延时的机器周期
*   返 回 值：无
*   作    者:
*************************************************/
void Delay_I2C_Temp(unsigned int t)
{
    while (t)
    {
	t--;
    }
}

/************************************************
*	函数名称：I2C_Delay
*	功能描述：I2C中的延时
*	参    数：无
*   返 回 值：无
*   作    者:
*************************************************/
void I2C_Delay(void)
{
    Delay_I2C_Temp(5);
}

/************************************************
*	函数名称：I2C_Init
*	功能描述：初始化I2C总线
*	参    数：无
*   返 回 值：无
*	作    者：
*************************************************/
void TX_I2C_Init(void)
{
    TX_I2C_SDA = 1; //释放I2C总线的数据线
    TX_I2C_SCL = 1; //释放I2C总线的时钟线
}
/************************************************
*	函数名称：void I2C_Start(void)
*	功能描述：生成I2C的起始信号，SCL为高，SDA下降沿
*	参    数：无
*   返 回 值：无
*   作    者：
*************************************************/
void TX_I2C_Start()
{
    TX_I2C_SDA = 1; //
    TX_I2C_SCL = 1; //
    I2C_Delay();    //延迟5个机器周期
    TX_I2C_SDA = 0; //SCL高，SDA下降沿，产生起始信号
    I2C_Delay();    //
    TX_I2C_SCL = 0; //
}

/************************************************
*	函数名称：I2C_Stop
*	功能描述：生成I2C的停止信号，SCL为高，SDA上升沿
*	参    数：无
*   返 回 值：无
*   作    者：
*************************************************/
void TX_I2C_Stop()
{
    TX_I2C_SDA = 0;
    I2C_Delay();
    TX_I2C_SCL = 1;
    I2C_Delay();    //延时5个机器周期
    TX_I2C_SDA = 1; //SCL为高，SDA上升沿，产生停止信号
    I2C_Delay();
}

/************************************************
*	函数名称：I2C_Ack
*	功能描述：I2C中的应答信号，每次发送8位字后，发送方将SDA释放一个bit的时间，
*             期间接收方将SDA拉低表示应答
*	参    数：无
*   返 回 值：bit,返回值为0表示应答，为1表示非应答
*   作    者：
*************************************************/
bit TX_I2C_Ack()
{
    bit ack; //存储应答信号
    TX_I2C_SDA = 1;
    I2C_Delay();
    TX_I2C_SCL = 1;
    I2C_Delay();
    ack = TX_I2C_SDA; //读取确认信号，
    I2C_Delay();
    TX_I2C_SCL = 0;
    I2C_Delay();

    return ack; //0表示应答，1表示非应答
}

/************************************************
*	函数名称：I2C_Write8Bit
*	功能描述：I2C总线中的一个字节写操作，发送8bit数据
*	参    数：unsigned char DData，写入的数据
*   返 回 值：无
*   作    者：
*************************************************/
void TX_I2C_Write8Bit(unsigned char DData)
{
    unsigned char i;
    unsigned char temp;
    temp = DData;
    for (i = 0; i < 8; i++)
    {
	TX_I2C_SDA = temp & 0x80; //根据I2C协议从最高位开始发送
	I2C_Delay();
	TX_I2C_SCL = 1;
	I2C_Delay();
	TX_I2C_SCL = 0;
	I2C_Delay();
	temp = temp << 1; //待发数据左移，准备发送下一位
    }
}

/************************************************
*	函数名称：I2C_Read8Bit
*	功能描述：I2C总线中读取一个字节，8bit的数据
*	参    数：无
*   返 回 值：ReadData-I2C总线中8bit数据
*   作    者：
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
*						QSFP_MSA协议中的写操作有3种：
*	1）字节写，写操作顺序：Start -> QSFP ADDR(写) -> ACK(主机) -> MEMORY ADDR -> ACK(从机)
						   -> DATA -> ACK(从机) -> Stop
*	2）顺序写，操作顺序：Start -> QSFP ADDR(写) -> ACK(主机) -> MEMORY ADDR -> ACK(从机)
*						 -> DATA 1 -> ACK(主机) -> DATA 2 -> ACK(主机) -> DATA n -> ACK(主机)
						 -> Stop
	3）确认机制的轮询方式写
*****************************************************************************************/
/***************************************************************************************
*						QSFP_MSA协议中的读操作有4种：
	1）当前地址的读，读操作格式：Start -> QSFP ADDR（读） -> ACK(从机) -> NACK ->Stop
	2）随机读，读操作格式：Start -> QSFP ADDR(写) -> ACK(从机) -> MEMORY ADDR ->
						   ACK(从机) -> QSFP ADDR（读） -> ACK(从机) -> DATA -> NACK 
						   -> STOP
	3）从当前地址开始连续读，读操作格式：Start -> QSFP ADDR（读） -> ACK(从机) -> DATA
							-> ACK(主机) -> DATA+1 -> ACK(主机) -> DATA+x ->
							NACK -> Stop
	4）从随机的地址开始连续读，读操作格式：Start -> QSFP ADDR(写) -> ACK(从机) -> 
							MEMORY ADDR -> ACK(主机) -> Start -> QSFP ADDR(读) ->
							ACK(从机) -> DATA -> ACK(主机) -> DATA+1 -> ACK(主机)
							-> DATA+n -> NACK -> Stop
*****************************************************************************************/
/************************************************
*	函数名称：I2C_SingleByteWrite
*	功能描述：向从机发送一个字节的数据
*	参    数：Daddr-从机地址， Waddr-数据地址，Data-传送数据
*   返 回 值：无
*   作    者：
*************************************************/
void TX_I2C_SingleByteWrite(unsigned char Daddr, unsigned char Waddr, unsigned char Data)
{
    TX_I2C_Start();
    TX_I2C_Write8Bit(Daddr); //写入从机设备地址
    TX_I2C_Ack();
    TX_I2C_Write8Bit(Waddr); //写入数据地址
    TX_I2C_Ack();
    TX_I2C_Write8Bit(Data); //写入数据
    TX_I2C_Ack();
    TX_I2C_Stop(); //停止发送数据
}

/*void TX_I2C_BytesWrite(unsigned char Daddr, unsigned char Waddr, unsigned char *DataArray,unsigned char num)
{
	unsigned char Temp=num;
	TX_I2C_Start();
	TX_I2C_Write8Bit(Daddr); //写入从机设备地址
	TX_I2C_Ack();
	TX_I2C_Write8Bit(Waddr); //写入数据地址
	TX_I2C_Ack();

	while(Temp>0)
	{
		TX_I2C_Write8Bit(*DataArray++);   //写入数据
		TX_I2C_Ack();
		Temp--;
	}
	TX_I2C_Stop();   //停止发送数据
}*/
void TX_SendAck(void)
{
    TX_I2C_SDA = 0;
    I2C_Delay(); //延迟5个机器周期
    TX_I2C_SCL = 1;
    I2C_Delay(); //延迟5个机器周期
    TX_I2C_SCL = 0;
}

/************************************************
*	函数名称：I2C_SingleByteRead
*	功能描述：I2C总线中读取从机中的一个字节
*	参    数：Daddr-从机地址，Waddr-数据地址
*   返 回 值：ReadData-总线中的一个字节数据
*   作    者：
*************************************************/

unsigned char TX_I2C_SingleByteRead(unsigned char Daddr, unsigned char Waddr)
{
    unsigned char ReadData;
    TX_I2C_Start();
    TX_I2C_Write8Bit(Daddr); //写入设备地址
    TX_I2C_Ack();
    TX_I2C_Write8Bit(Waddr); //写入数据操作地址
    TX_I2C_Ack();
    TX_I2C_Stop();

    TX_I2C_Start();
    TX_I2C_Write8Bit(Daddr + 1); //写入设备地址
    TX_I2C_Ack();
    ReadData = TX_I2C_Read8Bit(); //读出I2C中的数据

    TX_I2C_Stop();

    return ReadData;
}

unsigned char *TX_I2C_BytesRead(unsigned char Daddr, unsigned char Waddr, unsigned char *arry, unsigned char num)
{
    unsigned char Temp = 0;

    TX_I2C_Start();
    TX_I2C_Write8Bit(Daddr); //写入设备地址
    TX_I2C_Ack();
    TX_I2C_Write8Bit(Waddr); //写入数据操作地址
    TX_I2C_Ack();
    TX_I2C_Stop();

    TX_I2C_Start();
    TX_I2C_Write8Bit(Daddr + 1); //写入设备地址
    TX_I2C_Ack();
    *arry++ = TX_I2C_Read8Bit(); //读出I2C中的数据
    for (Temp = 1; Temp < num; Temp++)
    {
	TX_SendAck();
	*arry++ = TX_I2C_Read8Bit();
    }
    TX_I2C_Stop();

    return arry;
}
