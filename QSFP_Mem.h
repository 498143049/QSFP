#ifndef __QSFP_Mem_H
#define __QSFP_Mem_H
#include "types.h"



/*********************返回操作关键字******************************/
#define Succeed        		   0x00
#define Privilegefail		   0x01
#define crossAreafail          0x02           


/************************************操作的宏函数**********************************/
#define BIT_CLEAN(var,n)        (var) &= ~(1<<(n))   //变量var 的n位（即第n+1位）清0
#define BIT_SET(var,n)          (var) |=  (1<<(n))   //变量var 的n位（即第n+1位）置1
#define BIT_GET(var,n)          (((var)>>(n))&0x01)  //读取变量var 的n位（即第n+1位）


/*****************end*********************************/

#define SelectZerotPage   0x00 
#define SelectFirstPage   0x01
#define SelectSecondPage  0x02
#define SelectThirdPage   0x03

/*****************权限设置***************************/
#define NoPrivilege    0x00 
#define ReadOnly       0x01
#define WriteOnly      0x02
#define ReadAndWrite   0x03

/******************指示******************************/
#define StatusIndicators             0x01 

       /******每一个位定义*********/
#define Data_Not_Ready               0x00



/*********************中断对应的寄存器****************/
#define TX_RX_LOS_1_4                 0x03
#define TX_falut_1_4                  0x04
#define TX_Temp_Alarm_Warning         0x06
#define TX_VCC_Alarm_Warning          0x07
#define RX_Power_Alarm_Warning_1_2    0x09
#define RX_Power_Alarm_Warning_3_4    0x0A
#define TX_Bais_Alarm_Warning_1_2     0x0B
#define TX_Bais_Alarm_Warning_3_4     0x0C
       /******每一个位定义*********/
#define L_Tx4_LOS                    0x07
#define L_Tx3_LOS                    0x06
#define L_Tx2_LOS                    0x05
#define L_Tx1_LOS                    0x04
#define L_Rx4_LOS                    0x03
#define L_Rx3_LOS                    0x02
#define L_Rx2_LOS                    0x01
#define L_Rx1_LOS                    0x00

#define L_Tx4_Fault                   0x03
#define L_Tx3_Fault                   0x02
#define L_Tx2_Fault                   0x01
#define L_Tx1_Fault                   0x00

#define L_Temp_High_Alarm             0x07
#define L_Temp_Low_Alarm              0x06
#define L_Temp_High_Warning           0x05
#define L_Temp_Low_Warning            0x04

#define L_Vcc_High_Alarm             0x07
#define L_Vcc_Low_Alarm              0x06
#define L_Vcc_High_Warning           0x05
#define L_Vcc_Low_Warning            0x04

#define L_Rx1_Power_High_Alarm             0x07
#define L_Rx1_Power_Low_Alarm              0x06
#define L_Rx1_Power_High_Warning           0x05
#define L_Rx1_Power_Low_Warning            0x04

#define L_Rx2_Power_High_Alarm             0x03
#define L_Rx2_Power_Low_Alarm              0x02
#define L_Rx2_Power_High_Warning           0x01
#define L_Rx2_Power_Low_Warning            0x00

#define L_Rx3_Power_High_Alarm             0x07
#define L_Rx3_Power_Low_Alarm              0x06
#define L_Rx3_Power_High_Warning           0x05
#define L_Rx3_Power_Low_Warning            0x04

#define L_Rx4_Power_High_Alarm             0x03
#define L_Rx4_Power_Low_Alarm              0x02
#define L_Rx4_Power_High_Warning           0x01
#define L_Rx4_Power_Low_Warning            0x00

#define L_Tx1_Bais_High_Alarm             0x07
#define L_Tx1_Bais_Low_Alarm              0x06
#define L_Tx1_Bais_High_Warning           0x05
#define L_Tx1_Bais_Low_Warning            0x04

#define L_Tx2_Bais_High_Alarm             0x03
#define L_Tx2_Bais_Low_Alarm              0x02
#define L_Tx2_Bais_High_Warning           0x01
#define L_Tx2_Bais_Low_Warning            0x00

#define L_Tx3_Bais_High_Alarm             0x07
#define L_Tx3_Bais_Low_Alarm              0x06
#define L_Tx3_Bais_High_Warning           0x05
#define L_Tx3_Bais_Low_Warning            0x04

#define L_Tx4_Bais_High_Alarm             0x03
#define L_Tx4_Bais_Low_Alarm              0x02
#define L_Tx4_Bais_High_Warning           0x01
#define L_Tx4_Bais_Low_Warning            0x00

/*****************************************************/

/*******************控制字节**************************/

#define Tx_disable                        0x56       //10进制 86 

#define TX1_disable                       0x00
#define TX2_disable                       0x00
#define TX3_disable                       0x00
#define TX4_disable                       0x00

#define PageSelect                        0x7F
/*****************定义每一个不同的ab******************/
#define  L_Temp_A    0
#define  L_Temo_B    1
/****************定义值******************************/
#define L_TEMP_HIGH_Head            (uint16 *)0

/******************/
/*********定义状态字节*******************************/
#define HighAlarm        1
#define HighWarning      2
#define LowAlarm         3
#define LowWarning       4
#define common           0
/*******************End*****************************/



uint8_t QSFP_GetData(uint8_t address,uint8_t *DataValue);
void QSFP_initValue();
void QSFP_Flash_Init();
uint8_t QSFP_writeSingledata(uint8_t address,uint8_t Value);
uint8_t Get_Data(uint8_t address);
uint8_t QSFP_writedatas(uint8_t address,uint8_t * ValueArray,uint8_t num);
uint8_t Get_Data(uint8_t address);
void Write_SingleData(uint8_t address,uint8_t DataValue);
void interruptforTx();
void interruptforRx();
uint8 GetValueStatus(uint8_t a,uint8_t b,uint16_t* array);
#endif 
