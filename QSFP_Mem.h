#ifndef __QSFP_Mem_H
#define __QSFP_Mem_H
#include "types.h"

/*********************返回操作关键字******************************/
#define Succeed 0x00
#define Privilegefail 0x01
#define crossAreafail 0x02

/*****************end*********************************/

#define SelectZerotPage 0x00
#define SelectFirstPage 0x01
#define SelectSecondPage 0x02
#define SelectThirdPage 0x03

/*****************权限设置***************************/
#define NoPrivilege 0x00
#define ReadOnly 0x01
#define WriteOnly 0x02
#define ReadAndWrite 0x03

/******************指示******************************/
#define StatusIndicators 0x01

/******每一个位定义*********/
#define Data_Not_Ready 0x00

/*********************中断对应的寄存器****************/
#define TX_RX_LOS_1_4 0x03
#define TX_falut_1_4 0x04
#define TX_Temp_Alarm_Warning 0x06
#define TX_VCC_Alarm_Warning 0x07
#define RX_Power_Alarm_Warning_1_2 0x09
#define RX_Power_Alarm_Warning_3_4 0x0A
#define TX_Bais_Alarm_Warning_1_2 0x0B
#define TX_Bais_Alarm_Warning_3_4 0x0C

/******每一个位定义*********/
#define L_Tx4_LOS 0x07
#define L_Tx3_LOS 0x06
#define L_Tx2_LOS 0x05
#define L_Tx1_LOS 0x04
#define L_Rx4_LOS 0x03
#define L_Rx3_LOS 0x02
#define L_Rx2_LOS 0x01
#define L_Rx1_LOS 0x00

#define L_Tx4_Fault 0x03
#define L_Tx3_Fault 0x02
#define L_Tx2_Fault 0x01
#define L_Tx1_Fault 0x00

#define L_Temp_High_Alarm 0x07
#define L_Temp_Low_Alarm 0x06
#define L_Temp_High_Warning 0x05
#define L_Temp_Low_Warning 0x04

#define L_Vcc_High_Alarm 0x07
#define L_Vcc_Low_Alarm 0x06
#define L_Vcc_High_Warning 0x05
#define L_Vcc_Low_Warning 0x04

#define L_Rx1_Power_High_Alarm 0x07
#define L_Rx1_Power_Low_Alarm 0x06
#define L_Rx1_Power_High_Warning 0x05
#define L_Rx1_Power_Low_Warning 0x04

#define L_Rx2_Power_High_Alarm 0x03
#define L_Rx2_Power_Low_Alarm 0x02
#define L_Rx2_Power_High_Warning 0x01
#define L_Rx2_Power_Low_Warning 0x00

#define L_Rx3_Power_High_Alarm 0x07
#define L_Rx3_Power_Low_Alarm 0x06
#define L_Rx3_Power_High_Warning 0x05
#define L_Rx3_Power_Low_Warning 0x04

#define L_Rx4_Power_High_Alarm 0x03
#define L_Rx4_Power_Low_Alarm 0x02
#define L_Rx4_Power_High_Warning 0x01
#define L_Rx4_Power_Low_Warning 0x00

#define L_Tx1_Bais_High_Alarm 0x07
#define L_Tx1_Bais_Low_Alarm 0x06
#define L_Tx1_Bais_High_Warning 0x05
#define L_Tx1_Bais_Low_Warning 0x04

#define L_Tx2_Bais_High_Alarm 0x03
#define L_Tx2_Bais_Low_Alarm 0x02
#define L_Tx2_Bais_High_Warning 0x01
#define L_Tx2_Bais_Low_Warning 0x00

#define L_Tx3_Bais_High_Alarm 0x07
#define L_Tx3_Bais_Low_Alarm 0x06
#define L_Tx3_Bais_High_Warning 0x05
#define L_Tx3_Bais_Low_Warning 0x04

#define L_Tx4_Bais_High_Alarm 0x03
#define L_Tx4_Bais_Low_Alarm 0x02
#define L_Tx4_Bais_High_Warning 0x01
#define L_Tx4_Bais_Low_Warning 0x00

/*****************************************************/

/*******************控制字节**************************/

#define Tx_disable 0x56 //10进制 86

#define TX1_disable 0x00
#define TX2_disable 0x00
#define TX3_disable 0x00
#define TX4_disable 0x00

#define PageSelect 0x7F

/*********定义状态字节*******************************/
#define HighAlarm 1
#define HighWarning 2
#define LowAlarm 3
#define LowWarning 4
#define common 0
/*******************End*****************************/

//定义点一个码为B0
#define pending 0x00
#define writeing 0x01
#define reading 0x02
#define qsfp_mode 0x04
#define debug_mode 0x08
#define dealwrite 0x10

//parameters 参数
#define Temp_A 3
#define Temp_B 5
#define Temp_max_AlarmValue 1500
#define Temp_min_AlarmValue 100
#define Temp_max_WarnningValue 1000
#define Temp_min_WarnningValue 300
#define Temp_valueP 22
#define Temp_bytep 6
#define Temp_bitp 4
#define Temp_Mbytep 103
#define Temp_Mbitp 4

#define Lvcc_A 5
#define Lvcc_B 5
#define Lvcc_max_AlarmValue 1500
#define Lvcc_min_AlarmValue 100
#define Lvcc_max_WarnningValue 1000
#define Lvcc_min_WarnningValue 300
#define Lvcc_valueP 26
#define Lvcc_bytep 7
#define Lvcc_bitp 4
#define Lvcc_Mbytep 104
#define Lvcc_Mbitp 4

#define LRX1_A 1
#define LRX1_B 0
#define LRX1_max_AlarmValue 1500
#define LRX1_min_AlarmValue 150
#define LRX1_max_WarnningValue 1000
#define LRX1_min_WarnningValue 500
#define LRX1_valueP 34
#define LRX1_bytep 9
#define LRX1_bitp 4
#define LRX1_Mbytep 242
#define LRX1_Mbitp 4

#define LRX2_A 5
#define LRX2_B 5
#define LRX2_max_AlarmValue 1500
#define LRX2_min_AlarmValue 100
#define LRX2_max_WarnningValue 1000
#define LRX2_min_WarnningValue 300
#define LRX2_valueP 36
#define LRX2_bytep 9
#define LRX2_bitp 0
#define LRX2_Mbytep 242
#define LRX2_Mbitp 0

#define LRX3_A 5
#define LRX3_B 5
#define LRX3_max_AlarmValue 1500
#define LRX3_min_AlarmValue 100
#define LRX3_max_WarnningValue 1000
#define LRX3_min_WarnningValue 300
#define LRX3_valueP 38
#define LRX3_bytep 10
#define LRX3_bitp 4
#define LRX3_Mbytep 243
#define LRX3_Mbitp 4

#define LRX4_A 5
#define LRX4_B 5
#define LRX4_max_AlarmValue 1500
#define LRX4_min_AlarmValue 100
#define LRX4_max_WarnningValue 1000
#define LRX4_min_WarnningValue 300
#define LRX4_valueP 40
#define LRX4_bytep 10
#define LRX4_bitp 0
#define LRX4_Mbytep 243
#define LRX4_Mbitp 0

#define LTX1_A 5
#define LTX1_B 5
#define LTX1_max_AlarmValue 1500
#define LTX1_min_AlarmValue 100
#define LTX1_max_WarnningValue 1000
#define LTX1_min_WarnningValue 300
#define LTX1_valueP 42
#define LTX1_bytep 11
#define LTX1_bitp 4
#define LTX1_Mbytep 244
#define LTX1_Mbitp 4

#define LTX2_A 5
#define LTX2_B 5
#define LTX2_max_AlarmValue 1500
#define LTX2_min_AlarmValue 100
#define LTX2_max_WarnningValue 1000
#define LTX2_min_WarnningValue 300
#define LTX2_valueP 44
#define LTX2_bytep 11
#define LTX2_bitp 0
#define LTX2_Mbytep 244
#define LTX2_Mbitp 0

#define LTX3_A 5
#define LTX3_B 5
#define LTX3_max_AlarmValue 1500
#define LTX3_min_AlarmValue 100
#define LTX3_max_WarnningValue 1000
#define LTX3_min_WarnningValue 300
#define LTX3_valueP 46
#define LTX3_bytep 12
#define LTX3_bitp 4
#define LTX3_Mbytep 245
#define LTX3_Mbitp 4

#define LTX4_A 5
#define LTX4_B 5
#define LTX4_max_AlarmValue 1500
#define LTX4_min_AlarmValue 100
#define LTX4_max_WarnningValue 1000
#define LTX4_min_WarnningValue 300
#define LTX4_valueP 48
#define LTX4_bytep 12
#define LTX4_bitp 0
#define LTX4_Mbytep 245
#define LTX4_Mbitp 0

#define RX_LOS1_FUN TX_LOS_FUN
#define RX_LOS2_FUN TX_LOS_FUN
#define RX_LOS3_FUN TX_LOS_FUN
#define RX_LOS4_FUN TX_LOS_FUN

#define TX_LOS1_FUN TX_LOS_FUN
#define TX_LOS2_FUN TX_LOS_FUN
#define TX_LOS3_FUN TX_LOS_FUN
#define TX_LOS4_FUN TX_LOS_FUN

#define TX_FAULT1_FUN TX_FAULT_FUN
#define TX_FAULT2_FUN TX_FAULT_FUN
#define TX_FAULT3_FUN TX_FAULT_FUN
#define TX_FAULT4_FUN TX_FAULT_FUN
#define TX_LOS1_bytep 3
#define TX_LOS1_bitp 0
#define TX_LOS1_Mbytep 100
#define TX_LOS1_Mbitp 0

#define TX_LOS2_bytep 3
#define TX_LOS2_bitp 1
#define TX_LOS2_Mbytep 100
#define TX_LOS2_Mbitp 1

#define TX_LOS3_bytep 3
#define TX_LOS3_bitp 2
#define TX_LOS3_Mbytep 100
#define TX_LOS3_Mbitp 2

#define TX_LOS4_bytep 3
#define TX_LOS4_bitp 3
#define TX_LOS4_Mbytep 100
#define TX_LOS4_Mbitp 3

#define RX_LOS1_bytep 3
#define RX_LOS1_bitp 4
#define RX_LOS1_Mbytep 100
#define RX_LOS1_Mbitp 4

#define RX_LOS2_bytep 3
#define RX_LOS2_bitp 5
#define RX_LOS2_Mbytep 100
#define RX_LOS2_Mbitp 5

#define RX_LOS3_bytep 3
#define RX_LOS3_bitp 6
#define RX_LOS3_Mbytep 100
#define RX_LOS3_Mbitp 6

#define RX_LOS4_bytep 3
#define RX_LOS4_bitp 7
#define RX_LOS4_Mbytep 100
#define RX_LOS4_Mbitp 7

#define TX_FAULT1_bytep 4
#define TX_FAULT1_bitp 0
#define TX_FAULT1_Mbytep 101
#define TX_FAULT1_Mbitp 0

#define TX_FAULT2_bytep 4
#define TX_FAULT2_bitp 1
#define TX_FAULT2_Mbytep 101
#define TX_FAULT2_Mbitp 1

#define TX_FAULT3_bytep 4
#define TX_FAULT3_bitp 2
#define TX_FAULT3_Mbytep 101
#define TX_FAULT3_Mbitp 2

#define TX_FAULT4_bytep 4
#define TX_FAULT4_bitp 3
#define TX_FAULT4_Mbytep 101
#define TX_FAULT4_Mbitp 3

//define the bit of intl_status
#define TX_LOS1_INTL 0
#define TX_LOS2_INTL 1
#define TX_LOS3_INTL 2
#define TX_LOS4_INTL 3
#define RX_LOS1_INTL 4
#define RX_LOS2_INTL 5
#define RX_LOS3_INTL 6
#define RX_LOS4_INTL 7
#define TX_FAULT1_INTL 8
#define TX_FAULT2_INTL 9
#define TX_FAULT3_INTL 10
#define TX_FAULT4_INTL 11

#define LTEMP_HIGH_INTL 12
#define LTEMP_LOW_INTL 13

#define LVCC_HIGH_INTL 14
#define LVCC_LOW_INTL 15

#define LRX1_LOW_INTL 16
#define LRX1_HIGH_INTL 17
#define LRX2_LOW_INTL 18
#define LRX2_HIGH_INTL 19
#define LRX3_LOW_INTL 20
#define LRX3_HIGH_INTL 21
#define LRX4_LOW_INTL 22
#define LRX4_HIGH_INTL 23

#define LTX1_LOW_INTL 24
#define LTX1_HIGH_INTL 15
#define LTX2_LOW_INTL 26
#define LTX2_HIGH_INTL 27
#define LTX3_LOW_INTL 28
#define LTX3_HIGH_INTL 29
#define LTX4_LOW_INTL 30
#define LTX4_HIGH_INTL 31

#define comparebit(value, Fbit, Target) (BIT_GET(value, Fbit) == Target) //比较其中的bit位
#define GetValueStatus(value, name) _GetValueStatus(name##_A *value + name##_B, name##_max_AlarmValue, name##_min_AlarmValue, name##_max_WarnningValue, name##_min_WarnningValue, name##_valueP, name##_bytep, name##_bitp, name##_Mbytep, name##_Mbitp )
#define MonitorValue(value, name) _MonitorValue(value, name##_FUN, name##_bytep, name##_bitp, name##_Mbytep, name##_Mbitp);
#define DEBUG_GetData(address) Get_Data(address)

#define qsfp_write_data(data_address, data_head, data_num ) \  
					if ((data_num) == 1) \	
						QSFP_writeSingledata((data_address), *(data_head)); \
    else QSFP_writedatas((data_address), (data_head), (data_num))

#define write_sets(address, num) \
    while (num--)                \
        WriteSet(address++)

uint8_t TX_LOS_FUN(uint8_t value);
uint8_t TX_FAULT_FUN(uint8_t value);
void  _GetValueStatus(uint16_t value, uint16_t max_AlarmValue, uint16_t min_AlarmValue, uint16_t max_WanningValue, uint16_t min_WarningValue, uint8_t valuePosition, uint8_t bytep, uint8_t bitp, uint8_t Mbytep, uint8_t Mbitp );
void _MonitorValue(uint8_t value, uint8_t (*fun)(uint8_t), uint8_t value_position, uint8_t bit_position, uint8_t mask_value_position, uint8_t mask_bit_position);
uint8_t QSFP_GetData(uint8_t address);
void QSFP_initValue();
void QSFP_Flash_Init();
uint8_t QSFP_writeSingledata(uint8_t address, uint8_t Value);
uint8_t Get_Data(uint8_t address);
uint8_t QSFP_writedatas(uint8_t address, uint8_t *ValueArray, uint8_t num);
void Write_SingleData(uint8_t address, uint8_t DataValue);
void interrupt_tx();
void interrupt_rx();
void loop_detect();

#endif
