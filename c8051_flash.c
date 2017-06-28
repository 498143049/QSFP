//-----------------------------------------------------------------------------
// FLASH_ByteWrite
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR addr - address of the byte to write to
//                    valid range is 0x0000 to 0xFBFF for 64K Flash devices
//                    valid range is 0x0000 to 0x7FFF for 32K Flash devices
//                    valid range is 0x0000 to 0x2FFF for 8K Flash devices
//   2) U8 byte - byte to write to Flash.
//
// This routine writes <byte> to the linear FLASH address <addr>.
//-----------------------------------------------------------------------------
#include "c8051_flash.h"
#include "c8051f330.h"
#include "stdarg.h"
#include <absacc.h>
xuint8_t Flash_data[256]; //构造 Flash_data

void FLASH_ByteWrite(FLADDR addr, uint8_t byte)
{
  bit EA_SAVE = EA;           // Preserve EA
  uint8_t xdata *data pwrite; // FLASH write pointer

  // uint8_t SFRPAGE_save = SFRPAGE;   //杩欎釜涓嶇煡閬撴槸浠�涔堝瘎瀛樺櫒
  // SFRPAGE = ACTIVE_PAGE;

  EA = 0; // Disable interrupts
  //VDD澶嶄綅
  VDM0CN = 0xA0; // Enable VDD monitor and high threshold
  RSTSRC = 0x02; // Enable VDD monitor as a reset source

  pwrite = (uint8_t xdata *)addr; //璁″叆鍚稿叆鍦板潃

  FLKEY = 0xA5;  // Key Sequence 1
  FLKEY = 0xF1;  // Key Sequence 2
  PSCTL |= 0x01; // PSWE = 1 which enables writes

  VDM0CN = 0xA0; // Enable VDD monitor and high threshold
  RSTSRC = 0x02; // Enable VDD monitor as a reset source

  *pwrite = byte; // Write the byte

  PSCTL &= ~0x01; // PSWE = 0 which disable writes

  EA = EA_SAVE; // Restore interrupts

  //  SFRPAGE = SFRPAGE_save;
}

//-----------------------------------------------------------------------------
// FLASH_ByteRead
//-----------------------------------------------------------------------------
//
// Return Value :
//      U8 - byte read from Flash
// Parameters   :
//   1) FLADDR addr - address of the byte to read to
//                    valid range is 0x0000 to 0xFBFF for 64K Flash devices
//                    valid range is 0x0000 to 0x7FFF for 32K Flash devices
//
// This routine reads a <byte> from the linear FLASH address <addr>.
//-----------------------------------------------------------------------------

//应该读数据不需要这么麻烦  测试一下就知道了
//#define FLASH_ByteRead(addr) CWORD[addr]
uint8_t FLASH_ByteRead (FLADDR addr)
{
   bit EA_SAVE = EA;                   // Preserve EA
   uint8_t code * data pread;               // FLASH read pointer
   uint8_t  byte;

   EA = 0;                             // Disable interrupts
   pread = (uint8_t code *) addr;
   byte = *pread;                      // Read the byte
   EA = EA_SAVE;                       // Restore interrupts
   return byte;
}

//-----------------------------------------------------------------------------
// FLASH_PageErase
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) FLADDR addr - address of any byte in the page to erase
//                    valid range is 0x0000 to 0xF9FF for 64K Flash devices
//                    valid range is 0x0000 to 0x7DFF for 32K Flash devices
//
// This routine erases the FLASH page containing the linear FLASH address
// <addr>.  Note that the page of Flash containing the Lock Byte cannot be
// erased from application code.
//-----------------------------------------------------------------------------

/************擦除当前分区******************/
void FLASH_PageErase(FLADDR addr)
{
  bit EA_SAVE = EA;           // Preserve EA
  uint8_t xdata *data pwrite; // FLASH write pointer

  EA = 0; // Disable interrupts

  VDM0CN = 0xA0; // Enable VDD monitor and high threshold

  RSTSRC = 0x02; // Enable VDD monitor as a reset source

  pwrite = (uint8_t xdata *)addr;

  FLKEY = 0xA5;  // Key Sequence 1
  FLKEY = 0xF1;  // Key Sequence 2
  PSCTL |= 0x03; // PSWE = 1; PSEE = 1

  VDM0CN = 0xA0; // Enable VDD monitor and high threshold

  RSTSRC = 0x02; // Enable VDD monitor as a reset source

  *pwrite = 0; // Initiate page erase

  PSCTL &= ~0x03; // PSWE = 0; PSEE = 0
  EA = EA_SAVE;   // Restore interrupts
}
/************************************************
*	函数名称：FLASH_ReadData
*	功能描述：read bytes 
*           
*   作    者: dubing
*************************************************/
void FLASH_ReadData(FLADDR addr, uint8_t *buffer, uint8_t length)
{
  uint8_t i;
  for (i = 0; i < length; i++)
  {
    buffer[i] = FLASH_ByteRead(addr + i);
  }
}
/************************************************
*	函数名称：FLASH_ReadData
*	功能描述：read bytes 
*   Input   : the head address        
*   作    者: dubing
*************************************************/
void FLASH_WriteData(FLADDR addr, uint8_t *buffer, uint8_t length)
{
  uint8_t i;
  for (i = 0; i < length; i++)
    FLASH_ByteWrite(addr + i, buffer[i]);
}
/************************************************
*	函数名称：FlashWriteOnebyte
*	功能描述：Write single byte 
*   Input   : the head address        
*   作    者: dubing
*************************************************/
void FlashWriteOnebyte(uint16_t Base_address, uint8_t OffsetAddress, uint8_t DataValue, uint8_t ArrayLength)
{
  FLASH_ReadData(Base_address, Flash_data, ArrayLength); //先读取数据
  Flash_data[OffsetAddress] = DataValue;
  FLASH_PageErase(Base_address);
  FLASH_WriteData(Base_address, Flash_data, ArrayLength);
}
/************************************************
*	函数名称：FlashWriteOnebyte
*	功能描述：Write bytes  
*   Input   : the head address 
				ArrayLength the section length ,in order to accumulate the read operate speed        
*   作    者: dubing
*************************************************/
void FlashWriteOnebytes(uint16_t Base_address, uint8_t OffsetAddress, uint8_t *DataValue, uint8_t num, uint8_t ArrayLength)
{
  iuint8_t tempi = 0;
  FLASH_ReadData(Base_address, Flash_data, ArrayLength); //先读取数据
  for (tempi = 0; tempi < num; tempi++)
    Flash_data[OffsetAddress + tempi] = *DataValue++;

  FLASH_PageErase(Base_address);
  FLASH_WriteData(Base_address, Flash_data, ArrayLength);
}
/************************************************
*	函数名称：flash_write_bytes
*	功能描述：Write bytes  though variable parameter
*   Input   : the head address  offset value the parametes num   
*   作    者: dubing
*************************************************/
void flash_write_bytes(uint16_t Base_address, uint8_t OffsetAddress, uint8_t ArrayLength, uint8_t num, ...)
{
  iuint8_t tempi = 0;
  va_list arg_ptr;
  va_start(arg_ptr, num);
  FLASH_ReadData(Base_address, Flash_data, ArrayLength); //先读取数据
  for (tempi = 0; tempi < num; tempi++)
    Flash_data[OffsetAddress + tempi] = va_arg(arg_ptr, uint8_t);
  FLASH_PageErase(Base_address);
  FLASH_WriteData(Base_address, Flash_data, ArrayLength);
}
