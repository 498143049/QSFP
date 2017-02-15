#include "QSFP_Mem.h"
#include "types.h"
#include "c8051_i2c.h"
#include "c8051_flash.h"
#include "journal.h"
//This file funcitons operate the QSFP Memory
uint8_t xdata _data_Zeroth_Low[128];
uint8_t xdata _data_Third_High[128];
uint8_t xdata Flash_data[256];  //构造 Flash_data

//QSFP_Mem Init 
void QSFP_initValue()
{
	uint8_t xdata i=0;
	for(i=0;i<128;i++)
	{
		_data_Zeroth_Low[i]=50+i;
	}
	for(i=0;i<128;i++)
	{
		_data_Third_High[i]=50+i;
	}
	_data_Zeroth_Low[PageSelect]=0;
	
}
void QSFP_Flash_Init()
{
	//擦除第一块内存 
	uint8_t xdata i=0;

	//擦除第一块
	FLASH_PageErase(data_Zeroth_High_Address);
	for(i=0;i<128;i++)
	{
		Flash_data[i]=3;
	}
	FLASH_WriteData(data_Zeroth_High_Address,Flash_data,128);

	//擦除第二块
	FLASH_PageErase(data_First_High_Address);
	for(i=0;i<128;i++)
	{
		Flash_data[i]=4;
	}
	FLASH_WriteData(data_First_High_Address,Flash_data,128);

	//擦除第三块
	FLASH_PageErase(data_Second_High_Address);
	for(i=0;i<128;i++)
	{
		Flash_data[i]=5;
	}
	FLASH_WriteData(data_Second_High_Address,Flash_data,128);


}
//TX执行的中断 
#define comparebit(value,Fbit,Target) (BIT_GET(value,Fbit)==Target)  //比较其中的bit位
//Tx_Falut Tx_ROS 
void interruptforTx()
{
	uint8_t xdata tempArray[4]={0};
	int8_t xdata tempValue=0;
	//首先检测TX的LOS中断
	TX_I2C_BytesRead(HXT6104Address,0x50,tempArray,4);    //读取TX中中断寄存器寄存器  判断 LOS错误和fault错误
	tempValue=3;
	while(tempValue>=0)
	{
		if(comparebit(tempArray[tempValue],6,0)&&comparebit(tempArray[tempValue],2,1))
		{
			//  6(LOS)位=0,2位(LLOS)位=1 做&判断  则发生TX的LOS错误
     		BIT_SET(_data_Zeroth_Low[TX_RX_LOS_1_4],L_Tx1_LOS+tempValue);   //给对应的值赋不同的值
		}
		//TX L-vcc  
	/*	if(comparebit(tempArray[tempValue],5,1)&&comparebit(tempArray[tempValue],1,1))
		{
		  	BIT_SET(_data_First_Low[TX_VCC_Alarm_Warning],L_Tx1_LOS+tempValue);   //给对应的值赋不同的值
		}
		if(((BIT_GET(tempArray[tempValue],4))&&(BIT_GET(tempArray[tempValue],0)))
		{
		  
		}*/
		tempValue--;
	}

	//检测TX的Fault中断 
	TX_I2C_BytesRead(HXT6104Address,0x70,tempArray,4);
	tempValue=3;
	while(tempValue>0)
	{
		//同时为1则正常 ，不同时为1则为异常触发中断 
		if(comparebit(tempArray[tempValue],3,0)||comparebit(tempArray[tempValue],2,0))	 
		{
			//同时为1触发LOS
			BIT_SET(_data_Zeroth_Low[TX_falut_1_4],L_Tx1_Fault+tempValue);     
		}
		tempValue--;
	}
	//引发错误 

}
//RX_LOS
void interruptforRx()
{
	uint8_t  tempArray[4]={0};
	int8_t  tempValue=0;
  //检测RX的LOS中断
	RX_I2C_BytesRead(HXR6104Address,0x50,tempArray,4);
	tempValue=3;

	while(tempValue>0)
	{
		//同时为1触发LOS
		if(comparebit(tempArray[tempValue],6,0)&&comparebit(tempArray[tempValue],2,1))	 
		{		
    		BIT_SET(_data_Zeroth_Low[TX_RX_LOS_1_4],L_Rx1_LOS+tempValue);
    	}
		//同时为
		if(comparebit(tempArray[tempValue],7,0)&&comparebit(tempArray[tempValue],2,1))	 
		{		
    		BIT_SET(_data_Zeroth_Low[TX_RX_LOS_1_4],L_Rx1_LOS+tempValue);
    	}


		tempValue--;
	}
}
//返回Temp16的地址
#define  L_Temp_A   1000 
#define  L_Temp_B   1000 
#define  L_Vcc_A    1000
#define  L_Vcc_B    1000 

//全局变量 

uint8_t I1;
uint8_t I2[4]={0};

uint8_t * Get_I_Temp()
{
	return &I1;
}
//循环对模块进行检测
void ModelInterruputer()
{
    //L-Temp
	Get_I_Temp(); //获取到值
	SetFlagValue(I1,L_Temp);
	//L-vcc 
	SetFlagValue(V1,L_VCC);
} 
void Channelinterruputer()
{
   //L-Rx Power 1234
   	SetFlagValue(I1,L_RX_1);
	SetFlagValue(I1,L_RX_2);
	SetFlagValue(I1,L_RX_3);
	SetFlagValue(I1,L_RX_4);
	//Tx_1234 
	SetFlagValue(I2[0],L_TX_1);
	SetFlagValue(I2[1],L_TX_2);
	SetFlagValue(I2[3],L_TX_3);
	SetFlagValue(I2[4],L_TX_4);
}

/*void ModelValueSet()
{
	//Temperature  byte23 判断大端/还是小端
	int *point = &_data_Zeroth_Low[22];
	*point=TempValue*I1;
	//Supply Voltage Value
	point=&_data_Zeroth_Low[26];
	*point=vcc;

}
void ChannelValueSet()
{
    //Rx 输入功率 
	//Tx 的basis Value 
}
*/
void Read_Operate(uint8_t address)
{
	//读完寄存器中断后需要清除中断
//	if(address>=3&&address<=21)
//	{
//		_data_Zeroth_Low[address]=0;  //读取完后清除中断位
//	}
	//如果是四通道收发器在对四通道收发器做处理
	/*switch(address)
	{
		case  TX_RX_LOS_1_4:

			break;
		default:
			break;
	}*/
}
//其他数据写完调用 控制字节 
void WriteSet(uint8_t address,uint8_t Value)
{
	//写入字节控制 函数 
	if((address==PageSelect)&&(Value>=4))
	{
		//限制页数不能其他值,超过给0
		_data_Zeroth_Low[PageSelect]=0;
	}
	else if(address==Tx_disable)
	{
	  //TX 屏蔽通道使能  （字节写入）

	}
	else if(address==100)
	{
		//LOS
	}
	else if(address==101)
	{
		//FAULT
	}
	else if(address==103)
	{
		//TEMP
	}
	else if(address==104)
	{
		//VCC
	}
	else if(Value==SelectThirdPage&&address==240)
	{
		//RX_sq tx_SQ
	}
	else if(Value==SelectThirdPage&&address==241)
	{
		//Rx_output
	}
	else if(Value==SelectThirdPage&&(address==242||address==242||address==242) //243 244 245 都需要
	{
		//Power Power Bais Tx 
	}
	else
	{
		
	}
	//通道中断的屏蔽 (可字节可序列)

}
void FlashWriteOnebyte(uint16_t Base_address,uint8_t OffsetAddress,uint8_t DataValue,uint8_t ArrayLength)
{
	FLASH_ReadData(Base_address,Flash_data,ArrayLength);  //先读取数据
	Flash_data[OffsetAddress]=DataValue;
	FLASH_PageErase(Base_address);
	FLASH_WriteData(Base_address,Flash_data,ArrayLength);
}
void FlashWriteOnebytes(uint16_t Base_address,uint8_t OffsetAddress,uint8_t *DataValue,uint8_t num,uint8_t ArrayLength)
{
	uint8_t xdata tempi=0;
	FLASH_ReadData(Base_address,Flash_data,ArrayLength);  //先读取数据
	for(tempi=0;tempi<num;tempi++)
	{
		Flash_data[(Base_address+OffsetAddress+tempi)]=*DataValue++;
	}
	FLASH_PageErase(Base_address);
	FLASH_WriteData(Base_address,Flash_data,ArrayLength);
}
//获取可读写设置
uint8_t GetPrivilege(uint8_t address)
{
	if(address>=0&&address<86)
	{
		return ReadOnly;
	}
	else if(address>=86&&address<128)
	{
		return ReadAndWrite;
	}
	else if((_data_Zeroth_Low[PageSelect]==0||_data_Zeroth_Low[PageSelect]==1))
	{	
		return ReadOnly;
	}
	else if(_data_Zeroth_Low[PageSelect]==2)
	{
		return ReadAndWrite;
	}
	else if(_data_Zeroth_Low[PageSelect]==3)
	{
		if(address<=225)
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
	
	if(address<128)
	{
		return _data_Zeroth_Low[address];
	}
	else if(_data_Zeroth_Low[PageSelect]==0)
	{
		return FLASH_ByteRead(data_Zeroth_High_Address+address-128);
	}
	else if(_data_Zeroth_Low[PageSelect]==1)
	{
		return FLASH_ByteRead(data_First_High_Address+address-128);
	}
	else if(_data_Zeroth_Low[PageSelect]==2)
	{
		return FLASH_ByteRead(data_Second_High_Address+address-128);
	}
	else if(_data_Zeroth_Low[PageSelect]==3)
	{
		return _data_Third_High[address-128];
	}
	else 
	{
		return 0xFF;
	}
}
void Write_SingleData(uint8_t address,uint8_t DataValue)
{
	if(address<128)
	{
		 _data_Zeroth_Low[address]=DataValue;
	}
	else if(_data_Zeroth_Low[PageSelect]==0)
	{
		FlashWriteOnebyte(data_Zeroth_High_Address,address-128,DataValue,128);
		//flash Write 
	}
	else if(_data_Zeroth_Low[PageSelect]==1)
	{
		FlashWriteOnebyte(data_First_High_Address,address-128,DataValue,128);
		//flash Write 
	}
	else if(_data_Zeroth_Low[PageSelect]==2)
	{
		FlashWriteOnebyte(data_Second_High_Address,address-128,DataValue,128);
		//flash Write 
	}
	else if(_data_Zeroth_Low[PageSelect]==3)
	{
		 _data_Third_High[address-128]=DataValue;
	}
}

void Write_datas(uint8_t address,uint8_t *ValueArray,uint8_t num)
{
    uint8_t xdata tempi=0;
	if((address)<128)
	{
		for(tempi=0;tempi<num;tempi++)
		{
			_data_Zeroth_Low[(address+tempi)]=*ValueArray++;
		}
	
	}
	else if(_data_Zeroth_Low[127]==0)
	{
		FlashWriteOnebytes(data_Zeroth_High_Address,address-128,ValueArray,num,128);
		//flash Write 
	}
	else if(_data_Zeroth_Low[127]==1)
	{
		FlashWriteOnebytes(data_First_High_Address,address-128,ValueArray,num,128);
		//flash Write 
	}
	else if(_data_Zeroth_Low[127]==2)
	{
		FlashWriteOnebytes(data_Second_High_Address,address-128,ValueArray,num,128);
		//flash Write 
	}
	else if(_data_Zeroth_Low[127]==3)
	{

		for(tempi=0;tempi<num;tempi++)
		{
			_data_Third_High[(address+tempi)]=*ValueArray++;
		}
	}

}
uint8_t QSFP_GetData(uint8_t address,uint8_t *DataValue)
{
	uint8_t xdata Privilege;
	Privilege=GetPrivilege(address);  
	if((Privilege==ReadOnly)||(Privilege==ReadAndWrite))
	{
		*DataValue=Get_Data(address);
		Read_Operate(address);   //读取的一些特殊操作。
		return Succeed;
	}
	else
	{
		return Privilegefail;
	}
}
uint8_t QSFP_writeSingledata(uint8_t address,uint8_t Value)
{
	uint8_t xdata Privilege;
	Privilege=GetPrivilege(address);  
	if((Privilege==ReadAndWrite)||(Privilege==WriteOnly))
	{
		Write_SingleData(address,Value);
		return Succeed;
	}
	else
	{
		return Privilegefail;
	}
}
uint8_t QSFP_writedatas(uint8_t address,uint8_t * ValueArray,uint8_t num)
{
	uint8_t xdata  tempi=num;
	uint8_t xdata addresshead=address;
	uint8_t xdata Privilege;
	if((address<=127)&&(address+num)>128)
	{
		//寻找不能跨越中间数字
		return crossAreafail;
	}
	while(tempi>0)
	{
		Privilege=GetPrivilege(address++);                    //检测所有地址都合格则继续
		if(!((Privilege==ReadAndWrite)||(Privilege==WriteOnly)))
			return Privilegefail;
		tempi--;
	}
	Write_datas(addresshead,ValueArray,num);                  //进行数据写入处理
	return Succeed;
		
}
//
//get status  数组的顺序为 高提醒 低提醒 高警告
uint8_t GetValueStatus(uint8_t a,uint8_t b,uint16_t* array)
{
   uint16_t targetValue = BaisValue*a+b;
   if(targetValue>array[0]){
   		
     	return HighAlarm;
   }
   else if(targetValue>array[2])
   {
   		return HighWarning;	
   }
   else if(targetValue<array[3])
   {
   		return LowAlarm;
   }
   else if(targetValue<array[1])
   {
   	   return LowWarning;	
   }
   else
   {
   		return common;
   }
}
#define SetFlagValue(BaisValue,name)\
{\
		 uint16_t targetValue = BaisValue*#name##_A+#name##_B;\
		 //这里可以写入检测值 只需要知道他的byte位数的宏就可以
         //中断标志响应这里也需要完成 
		 if(targetValue>#name##_HighAlarm)\
		 { \
		   HighAlarm;\
		   //name_byte 设置中断标志位
		 }\
		else if(targetValue>#name##_HighWarning)
   		{\
			HighWarning;\	
   		}\
   		else if(targetValue<#name##_LowAlarm) \
   		{\
   			LowAlarm;\
   		}\
   		else if(targetValue<#name##_LowAlarm)\
  		{\
   	   		LowWarning;\	
   		}\
   		else\
   		{\	
   			common;\ //不进行任何处理 
   		}\
}
