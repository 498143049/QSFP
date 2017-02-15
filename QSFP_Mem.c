#include "QSFP_Mem.h"
#include "types.h"
#include "c8051_i2c.h"
#include "c8051_flash.h"
#include "journal.h"
//This file funcitons operate the QSFP Memory
uint8_t xdata _data_Zeroth_Low[128];
uint8_t xdata _data_Third_High[128];
uint8_t xdata Flash_data[256];  //���� Flash_data

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
	//������һ���ڴ� 
	uint8_t xdata i=0;

	//������һ��
	FLASH_PageErase(data_Zeroth_High_Address);
	for(i=0;i<128;i++)
	{
		Flash_data[i]=3;
	}
	FLASH_WriteData(data_Zeroth_High_Address,Flash_data,128);

	//�����ڶ���
	FLASH_PageErase(data_First_High_Address);
	for(i=0;i<128;i++)
	{
		Flash_data[i]=4;
	}
	FLASH_WriteData(data_First_High_Address,Flash_data,128);

	//����������
	FLASH_PageErase(data_Second_High_Address);
	for(i=0;i<128;i++)
	{
		Flash_data[i]=5;
	}
	FLASH_WriteData(data_Second_High_Address,Flash_data,128);


}
//TXִ�е��ж� 
#define comparebit(value,Fbit,Target) (BIT_GET(value,Fbit)==Target)  //�Ƚ����е�bitλ
//Tx_Falut Tx_ROS 
void interruptforTx()
{
	uint8_t xdata tempArray[4]={0};
	int8_t xdata tempValue=0;
	//���ȼ��TX��LOS�ж�
	TX_I2C_BytesRead(HXT6104Address,0x50,tempArray,4);    //��ȡTX���жϼĴ����Ĵ���  �ж� LOS�����fault����
	tempValue=3;
	while(tempValue>=0)
	{
		if(comparebit(tempArray[tempValue],6,0)&&comparebit(tempArray[tempValue],2,1))
		{
			//  6(LOS)λ=0,2λ(LLOS)λ=1 ��&�ж�  ����TX��LOS����
     		BIT_SET(_data_Zeroth_Low[TX_RX_LOS_1_4],L_Tx1_LOS+tempValue);   //����Ӧ��ֵ����ͬ��ֵ
		}
		//TX L-vcc  
	/*	if(comparebit(tempArray[tempValue],5,1)&&comparebit(tempArray[tempValue],1,1))
		{
		  	BIT_SET(_data_First_Low[TX_VCC_Alarm_Warning],L_Tx1_LOS+tempValue);   //����Ӧ��ֵ����ͬ��ֵ
		}
		if(((BIT_GET(tempArray[tempValue],4))&&(BIT_GET(tempArray[tempValue],0)))
		{
		  
		}*/
		tempValue--;
	}

	//���TX��Fault�ж� 
	TX_I2C_BytesRead(HXT6104Address,0x70,tempArray,4);
	tempValue=3;
	while(tempValue>0)
	{
		//ͬʱΪ1������ ����ͬʱΪ1��Ϊ�쳣�����ж� 
		if(comparebit(tempArray[tempValue],3,0)||comparebit(tempArray[tempValue],2,0))	 
		{
			//ͬʱΪ1����LOS
			BIT_SET(_data_Zeroth_Low[TX_falut_1_4],L_Tx1_Fault+tempValue);     
		}
		tempValue--;
	}
	//�������� 

}
//RX_LOS
void interruptforRx()
{
	uint8_t  tempArray[4]={0};
	int8_t  tempValue=0;
  //���RX��LOS�ж�
	RX_I2C_BytesRead(HXR6104Address,0x50,tempArray,4);
	tempValue=3;

	while(tempValue>0)
	{
		//ͬʱΪ1����LOS
		if(comparebit(tempArray[tempValue],6,0)&&comparebit(tempArray[tempValue],2,1))	 
		{		
    		BIT_SET(_data_Zeroth_Low[TX_RX_LOS_1_4],L_Rx1_LOS+tempValue);
    	}
		//ͬʱΪ
		if(comparebit(tempArray[tempValue],7,0)&&comparebit(tempArray[tempValue],2,1))	 
		{		
    		BIT_SET(_data_Zeroth_Low[TX_RX_LOS_1_4],L_Rx1_LOS+tempValue);
    	}


		tempValue--;
	}
}
//����Temp16�ĵ�ַ
#define  L_Temp_A   1000 
#define  L_Temp_B   1000 
#define  L_Vcc_A    1000
#define  L_Vcc_B    1000 

//ȫ�ֱ��� 

uint8_t I1;
uint8_t I2[4]={0};

uint8_t * Get_I_Temp()
{
	return &I1;
}
//ѭ����ģ����м��
void ModelInterruputer()
{
    //L-Temp
	Get_I_Temp(); //��ȡ��ֵ
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
	//Temperature  byte23 �жϴ��/����С��
	int *point = &_data_Zeroth_Low[22];
	*point=TempValue*I1;
	//Supply Voltage Value
	point=&_data_Zeroth_Low[26];
	*point=vcc;

}
void ChannelValueSet()
{
    //Rx ���빦�� 
	//Tx ��basis Value 
}
*/
void Read_Operate(uint8_t address)
{
	//����Ĵ����жϺ���Ҫ����ж�
//	if(address>=3&&address<=21)
//	{
//		_data_Zeroth_Low[address]=0;  //��ȡ�������ж�λ
//	}
	//�������ͨ���շ����ڶ���ͨ���շ���������
	/*switch(address)
	{
		case  TX_RX_LOS_1_4:

			break;
		default:
			break;
	}*/
}
//��������д����� �����ֽ� 
void WriteSet(uint8_t address,uint8_t Value)
{
	//д���ֽڿ��� ���� 
	if((address==PageSelect)&&(Value>=4))
	{
		//����ҳ����������ֵ,������0
		_data_Zeroth_Low[PageSelect]=0;
	}
	else if(address==Tx_disable)
	{
	  //TX ����ͨ��ʹ��  ���ֽ�д�룩

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
	else if(Value==SelectThirdPage&&(address==242||address==242||address==242) //243 244 245 ����Ҫ
	{
		//Power Power Bais Tx 
	}
	else
	{
		
	}
	//ͨ���жϵ����� (���ֽڿ�����)

}
void FlashWriteOnebyte(uint16_t Base_address,uint8_t OffsetAddress,uint8_t DataValue,uint8_t ArrayLength)
{
	FLASH_ReadData(Base_address,Flash_data,ArrayLength);  //�ȶ�ȡ����
	Flash_data[OffsetAddress]=DataValue;
	FLASH_PageErase(Base_address);
	FLASH_WriteData(Base_address,Flash_data,ArrayLength);
}
void FlashWriteOnebytes(uint16_t Base_address,uint8_t OffsetAddress,uint8_t *DataValue,uint8_t num,uint8_t ArrayLength)
{
	uint8_t xdata tempi=0;
	FLASH_ReadData(Base_address,Flash_data,ArrayLength);  //�ȶ�ȡ����
	for(tempi=0;tempi<num;tempi++)
	{
		Flash_data[(Base_address+OffsetAddress+tempi)]=*DataValue++;
	}
	FLASH_PageErase(Base_address);
	FLASH_WriteData(Base_address,Flash_data,ArrayLength);
}
//��ȡ�ɶ�д����
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
		Read_Operate(address);   //��ȡ��һЩ���������
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
		//Ѱ�Ҳ��ܿ�Խ�м�����
		return crossAreafail;
	}
	while(tempi>0)
	{
		Privilege=GetPrivilege(address++);                    //������е�ַ���ϸ������
		if(!((Privilege==ReadAndWrite)||(Privilege==WriteOnly)))
			return Privilegefail;
		tempi--;
	}
	Write_datas(addresshead,ValueArray,num);                  //��������д�봦��
	return Succeed;
		
}
//
//get status  �����˳��Ϊ ������ ������ �߾���
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
		 //�������д����ֵ ֻ��Ҫ֪������byteλ���ĺ�Ϳ���
         //�жϱ�־��Ӧ����Ҳ��Ҫ��� 
		 if(targetValue>#name##_HighAlarm)\
		 { \
		   HighAlarm;\
		   //name_byte �����жϱ�־λ
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
   			common;\ //�������κδ��� 
   		}\
}
