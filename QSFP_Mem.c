#include "QSFP_Mem.h"
#include "types.h"
#include "c8051_i2c.h"
#include "c8051_flash.h"
#include "journal.h"
#include "c8051_system.h"
#include "c8051_gpio.h"
#include <absacc.h>

//This file funcitons operate the QSFP Memory
xuint8_t _data_Zeroth_Low[256];

extern iuint32_t intl_status;
extern iuint32_t intl_status_old;
extern iuint16_t adc_value[10];
extern xuint8_t Flash_data[256];

//QSFP_Mem In
void QSFP_initValue()
{
    iuint8_t i = 0;
    for (i = 0; i < 255; i++)
    {
	_data_Zeroth_Low[i] = 0;
    }
    _data_Zeroth_Low[PageSelect] = 0;
}
void QSFP_Flash_Init()
{
    //擦除第一块内存
    iuint8_t i = 0;

    //擦除第一块
    FLASH_PageErase(data_Zeroth_High_Address);
    for (i = 0; i < 128; i++)
    {
	Flash_data[i] = 3;
    }
    FLASH_WriteData(data_Zeroth_High_Address, Flash_data, 128);

    //擦除第二块
    FLASH_PageErase(data_First_High_Address);
    for (i = 0; i < 128; i++)
    {
	Flash_data[i] = 4;
    }
    FLASH_WriteData(data_First_High_Address, Flash_data, 128);

    //擦除第三块
    FLASH_PageErase(data_Second_High_Address);
    for (i = 0; i < 128; i++)
    {
	Flash_data[i] = 5;
    }
    FLASH_WriteData(data_Second_High_Address, Flash_data, 128);
}

//TX执行的中断 检测
uint8_t TX_LOS_FUN(uint8_t value)
{
    if (comparebit(value, 6, 0) && comparebit(value, 2, 1))
	    return 1;
    else
	    return 0;
}
uint8_t TX_FAULT_FUN(uint8_t value)
{
    if ((comparebit(value, 3, 0)&&comparebit(value, 1, 1)) || comparebit(value, 2, 0)&&comparebit(value, 0, 1))
	    return 1;
    else
	    return 0;
}
//为什么需要加1 需要在看下手册
void _MonitorValue(uint8_t value, uint8_t (*fun)(uint8_t), uint8_t value_position, uint8_t bit_position, uint8_t mask_value_position, uint8_t mask_bit_position)
{
    bit ismask = 1;
    bit bit_value, bit_trigger;
    bit_value = BIT_GET(_data_Zeroth_Low[value_position], bit_position);
    bit_trigger = fun(value);
    if((!bit_value)&&bit_trigger){
	    BIT_SET(_data_Zeroth_Low[value_position], bit_position); //set bit
        ismask = BIT_GET(_data_Zeroth_Low[mask_value_position],(mask_bit_position + 1));
	    if (!ismask)
	        intl_status = 1;
    }
    //clearn 
  /*  if(bit_trigger)
    {
        TX_LOS_CLEAR();
    }*/
}
#define TX_LOS_CLEAR(clear_value, address) clear_value = BIT_CLEAN(clear_value, 2);  TX_write(address, clear_value);
#define TX_FALUT_CLEAR(clear_value, address) clear_value = BIT_CLEAN(clear_value, 1);  clear_value = BIT_CLEAN(clear_value, 0);  TX_write(clear_value, value);
#define RX_LOS_CLEAR(clear_value, address) clear_value = BIT_CLEAN(clear_value, 2);  RX_write(address, clear_value);
void interrupt_tx()
{
    xuint8_t tempArray[4] = {0};
    //首先检测TX的LOS中断
    TX_I2C_BytesRead(HXT6104Address, 0x50, tempArray, 4); //读取TX中中断寄存器寄存器  判断 LOS错误和fault错误
    MonitorValue(tempArray[0], TX_LOS1);
    MonitorValue(tempArray[1], TX_LOS2);
    MonitorValue(tempArray[2], TX_LOS3);
    MonitorValue(tempArray[3], TX_LOS4);

    //检测TX的Fault中断
    TX_I2C_BytesRead(HXT6104Address, 0x70, tempArray, 4);
    MonitorValue(tempArray[0], TX_FAULT1);
    MonitorValue(tempArray[1], TX_FAULT2);
    MonitorValue(tempArray[2], TX_FAULT3);
    MonitorValue(tempArray[3], TX_FAULT4);
}
//RX_LOS
void interrupt_rx()
{
    xuint8_t tempArray[4] = {0};
    //检测RX的LOS中断
    RX_reads(0x50, tempArray, 4);
    MonitorValue(tempArray[0], RX_LOS1);
    MonitorValue(tempArray[1], RX_LOS2);
    MonitorValue(tempArray[2], RX_LOS3);
    MonitorValue(tempArray[3], RX_LOS4);
}
//读操作清除中断
void Read_Operate(uint8_t address)
{
    //读完寄存器中断后需要清除中断
    //如果是四通道收发器在对四通道收发器做处理对无措LOS 有4通道
    if (address >= 3 && address <= 21)
    {
	_data_Zeroth_Low[address] = 0; //读取完后清除中断位
	//reset status;
	switch (address)
	{
	case 3:
	    intl_status &= 0xFFFFFF00;
	    intl_status_old &= 0xFFFFFF00;
	    break;
	case 4:
	    intl_status &= 0xFFFFF0FF;
	    intl_status_old &= 0xFFFFF0FF;
	    break;
	case 6:
	    intl_status &= 0xFFFF3FFF;
	    intl_status_old &= 0xFFFFF3FF;
	    break;
	case 7:
	    intl_status &= 0xFFFFCFFF;
	    intl_status_old &= 0xFFFFFCFF;
	    break;
	case 9:
	    intl_status &= 0xFFF0FFFF;
	    intl_status_old &= 0xFFFFFCFF;
	    break;
	case 10:
	    intl_status &= 0xFF0FFFFF;
	    intl_status_old &= 0xFF0FFFFF;
	    break;
	case 11:
	    intl_status &= 0xF0FFFFFF;
	    intl_status_old &= 0xF0FFFFFF;
	    break;
	case 12:
	    intl_status &= 0x0FFFFFFF;
	    intl_status_old &= 0x0FFFFFFF;
	    break;
	default:
	    break;
	}
    }
}
//改变多个 而且address 不保证重用性质

//其他数据写完调用 控制字节
void WriteSet(uint8_t address)
{
    //写入字节控制 函数   only write and record doing the in main
    iuint8_t i = 0;
    iuint8_t value = _data_Zeroth_Low[address];
    iuint8_t value_old = 0;
    if ((address == PageSelect) && (value >= 4))
	_data_Zeroth_Low[PageSelect] = 0;
    /*	else if(address==Tx_disable)
		BIT_SET(control_flag_change, 0);
	else if(address==240)
		BIT_SET(control_flag_change, 1);
	else if(address==241)
		BIT_SET(control_flag_change, 2);*/
    else if (address == Tx_disable)
    {
	//TX 屏蔽通道使能  （字节写入）
	//for循环四次

	for (i = 0; i < 4; i++)
	{
	    value_old = TX_read(0X40 + i);
	    if (value & 0x01)
		    TX_write(0x40 + i, BIT_SET(value_old, 3));
	    else
		    TX_write(0x40 + i, BIT_CLEAN(value_old, 3));

	    value = value >> 1;
	}
    }
    else if (address == 240)
    {
	for (i = 0; i < 4; i++)
	{
	    value_old = RX_read(0X40 + i);
	    if (value & 0x01)
		    RX_write(0x40 + i, BIT_SET(value_old, 1));
	    else
		    RX_write(0x40 + i, BIT_CLEAN(value_old, 1));

	    value = value >> 1;
	}

	//tx4_SQ
	for (i = 0; i < 4; i++)
	{
	    value_old = TX_read(0X40 + i);
	    if (value & 0x01)
		    TX_write(0x40 + i, BIT_SET(value_old, 1));
	    else
		    TX_write(0x40 + i, BIT_CLEAN(value_old, 1));

	    value = value >> 1;
	}

	//for 循环四次
    }
    else if (address == 241)
    {
	//Rx_output
	for (i = 0; i < 4; i++)
	{
	    value_old = RX_read(0X40 + i);
	    if (value & 0x01)
		    RX_write(0x40 + i, BIT_SET(value_old, 3));
	    else
		    RX_write(0x40 + i, BIT_CLEAN(value_old, 3));

	    value = value >> 1;
	}
    }
}

//return Privilege
uint8_t GetPrivilege(uint8_t address)
{
    if (address >= 0 && address < 86)
    {
	return ReadOnly;
    }
    else if (address >= 86 && address < 128)
    {
	return ReadAndWrite;
    }
    else if ((_data_Zeroth_Low[PageSelect] == 0 || _data_Zeroth_Low[PageSelect] == 1))
    {
	return ReadOnly;
    }
    else if (_data_Zeroth_Low[PageSelect] == 2)
    {
	return ReadAndWrite;
    }
    else if (_data_Zeroth_Low[PageSelect] == 3)
    {
	if (address <= 225)
	{
	    return ReadOnly;
	}
	else
	{
	    return ReadAndWrite;
	}
    }
    else
    {
	return NoPrivilege;
    }
}

uint8_t Get_Data(uint8_t address)
{

    if (address < 128)
    {
	return _data_Zeroth_Low[address];
    }
    else if (_data_Zeroth_Low[PageSelect] == 0)
    {
	return FLASH_ByteRead(data_Zeroth_High_Address + address - 128);
    }
    else if (_data_Zeroth_Low[PageSelect] == 1)
    {
	return FLASH_ByteRead(data_First_High_Address + address - 128);
    }
    else if (_data_Zeroth_Low[PageSelect] == 2)
    {
	return FLASH_ByteRead(data_Second_High_Address + address - 128);
    }
    else if (_data_Zeroth_Low[PageSelect] == 3)
    {
	return _data_Zeroth_Low[address];
    }
    else
    {
	return 0xFF;
    }
}
void Write_SingleData(uint8_t address, uint8_t DataValue)
{
    if (address < 128)
    {
	_data_Zeroth_Low[address] = DataValue;
    }
    else if (_data_Zeroth_Low[PageSelect] == 0)
    {
	FlashWriteOnebyte(data_Zeroth_High_Address, address - 128, DataValue, 128);
	//flash Write
    }
    else if (_data_Zeroth_Low[PageSelect] == 1)
    {
	FlashWriteOnebyte(data_First_High_Address, address - 128, DataValue, 128);
	//flash Write
    }
    else if (_data_Zeroth_Low[PageSelect] == 2)
    {
	FlashWriteOnebyte(data_Second_High_Address, address - 128, DataValue, 128);
	//flash Write
    }
    else if (_data_Zeroth_Low[PageSelect] == 3)
    {
	_data_Zeroth_Low[address] = DataValue;
    }
}

void Write_datas(uint8_t address, uint8_t *ValueArray, uint8_t num)
{
    uint8_t xdata tempi = 0;
    if ((address) < 128)
    {
	for (tempi = 0; tempi < num; tempi++)
	{
	    _data_Zeroth_Low[(address + tempi)] = *ValueArray++;
	}
    }
    else if (_data_Zeroth_Low[127] == 0)
    {
	FlashWriteOnebytes(data_Zeroth_High_Address, address - 128, ValueArray, num, 128);
	//flash Write
    }
    else if (_data_Zeroth_Low[127] == 1)
    {
	FlashWriteOnebytes(data_First_High_Address, address - 128, ValueArray, num, 128);
	//flash Write
    }
    else if (_data_Zeroth_Low[127] == 2)
    {
	FlashWriteOnebytes(data_Second_High_Address, address - 128, ValueArray, num, 128);
	//flash Write
    }
    else if (_data_Zeroth_Low[127] == 3)
    {

	for (tempi = 0; tempi < num; tempi++)
	{
	    _data_Zeroth_Low[(address + tempi)] = *ValueArray++;
	}
    }
}
/*******/
uint8_t QSFP_GetData(uint8_t address)
{
    iuint8_t Privilege;
    Privilege = GetPrivilege(address);
    if ((Privilege == ReadOnly) || (Privilege == ReadAndWrite))
    {
		Read_Operate(address);
		return Get_Data(address); //if found error record in journal
    }
    else
	return 0xFF;
}

uint8_t QSFP_writeSingledata(uint8_t address, uint8_t Value)
{
    iuint8_t Privilege;
    Privilege = GetPrivilege(address);
    if ((Privilege == ReadAndWrite) || (Privilege == WriteOnly))
    {
		Write_SingleData(address, Value);
		WriteSet(address);
		return Succeed;
    }
    else
    {
		return Privilegefail;
    }
}
uint8_t QSFP_writedatas(uint8_t address, uint8_t *ValueArray, uint8_t num)
{
    iuint8_t tempi = num;
    iuint8_t addresshead = address;
    iuint8_t Privilege;
    if ((address <= 127) && (address + num) > 128)
    {
	//寻找不能跨越中间数字
	return crossAreafail;
    }
    while (tempi > 0)
    {
	Privilege = GetPrivilege(address++); //检测所有地址都合格则继续
	if (!((Privilege == ReadAndWrite) || (Privilege == WriteOnly)))
	    return Privilegefail;
	tempi--;
    }
    Write_datas(addresshead, ValueArray, num); //进行数据写入处理
    write_sets(addresshead, num);
    return Succeed;
}
//
//get status  数组的顺序为 高提醒 低提醒 高警告

#define fun_detect(s) \
 		bit ismask,  value_bit;   \
		value_bit = BIT_GET(_data_Zeroth_Low[bytep], bitp+s); \
		if(!value_bit)												\
		{											\					
			BIT_SET(_data_Zeroth_Low[bytep], bitp + s);			\
			ismask = BIT_GET(_data_Zeroth_Low[Mbytep], Mbitp + s);	\
			if (!ismask)				\
	    		intl_status = 1;       	\
		} 

#define HIGH_ALARM 3 
#define MIN_ALARM  2
#define HIGH_WARNING 1
#define MIN_WARNING  0

void _GetValueStatus(uint16_t value, uint16_t max_AlarmValue, uint16_t min_AlarmValue, uint16_t max_WanningValue, uint16_t min_WarningValue, uint8_t valuePosition, uint8_t bytep, uint8_t bitp, uint8_t Mbytep, uint8_t Mbitp )
{
    //给定位置赋值
	//set recordValue
    int *p = &_data_Zeroth_Low[valuePosition];
    *p = value;
    //wheter if it
    if (value > max_AlarmValue)
    {
		fun_detect(HIGH_ALARM);
    }
    else if (value < min_AlarmValue)
    {
		fun_detect(MIN_ALARM);
    }
    else if (value > max_WanningValue)
    {
		fun_detect(HIGH_WARNING);
    }
    else if (value < min_WarningValue)
    {
		fun_detect(MIN_WARNING);
    }
	//else it is common 
}

/************************************************
*	函数名称：loop_detect
*	功能描述：loop detect the current and voltage   max must limit 50ms ? ,100ms detect onece 
*	参    数：none  
*   返 回 值：none
*   作    者：dubing
*************************************************/
void loop_detect()
{
    //loop_detect the adc and the value
    six_one();
    adc_value[0] = sample_adc_value(5);

    one_two();
	adc_value[1] = sample_adc_value(5);

	two_three();
	adc_value[2] = sample_adc_value(5);

	three_four();
	adc_value[3] = sample_adc_value(5);

	four_five();
	adc_value[4] = sample_adc_value(5);

	five_six();
	adc_value[5] = sample_adc_value(5);
    //GetValueStatus(adc_value[4],Temp);
    //GetValueStatus(adc_value[5],Lvcc);

    /*GetValueStatus(adc_value[0],LRX1);
	GetValueStatus(adc_value[1],LRX2);
	GetValueStatus(adc_value[2],LRX3);
	GetValueStatus(adc_value[3],LRX4);

	//current watch 
	ADC_Value[0]=TX_read(0X10);
	ADC_Value[1]=TX_read(0X13);
	ADC_Value[2]=TX_read(0X16);
	ADC_Value[3]=TX_read(0X19);

	GetValueStatus(ADC_Value[3],LTX4);
	GetValueStatus(ADC_Value[0],LTX1);
	GetValueStatus(ADC_Value[1],LTX2);
	GetValueStatus(ADC_Value[2],LTX3); */
}
