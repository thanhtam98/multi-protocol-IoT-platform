/**********************  nvm.h  *******************************
 *           #######
 *           ##  ##
 *           #  ##    ####   #####    #####  ##  ##   #####
 *             ##    ##  ##  ##  ##  ##      ##  ##  ##
 *            ##  #  ######  ##  ##   ####   ##  ##   ####
 *           ##  ##  ##      ##  ##      ##   #####      ##
 *          #######   ####   ##  ##  #####       ##  #####
 *                                           #####
 *          Z-Wave, the wireless language.
 *
 *              Copyright (c) 2010
 *              Zensys A/S
 *              Denmark
 *
 *              All Rights Reserved
 *
 *    This source file is subject to the terms and conditions of the
 *    Zensys Software License Agreement which restricts the manner
 *    in which it may be used.
 *
 *---------------------------------------------------------------------------
 *
 * Description: Source code of the Z-Wave Module External NVM interface.
 *              Reading / writing of the Z-Wave Module External NVM via
 *               the software SPI was implemented.
 *              Support of several types of External NVM are implemented.
 *              Type of the External NVM is detected at initialization step
 *               and corresponded External NVM driver is selected automatically
 *               based on manufacturer ID and device ID, readed from the
 *               External NVM chip.
 *
 * Author:      Valeriy Vyshnyak
 *
 * Last Changed By:  $Author: vvi $
 * Revision:         $Revision: 0000 $
 * Last Changed:     $Date: 11 лют. 2010 14:57:43 $
 *
 ****************************************************************************/


#ifndef NVM_H_
#define NVM_H_

#include "ZWaveFlash.h"

/// <summary>
/// External Non-Volatile Memory (NVM) chip types enumeration.
/// </summary>
enum NVMTypes
{
  /// <summary>
  /// Bus and protocol type mask.
  /// </summary>
  NVMTypeMask = 0xFF000000,
  /// <summary>
  /// Bus and protocol type bits position.
  /// </summary>
  NVMTypePos = 24,
  /// <summary>
  /// Chip Manufacturer ID mask. 0x00 - unknown manufacturer id.
  /// </summary>
  NVMManufacturerMask = 0x00FF0000,
  /// <summary>
  /// Chip Manufacturer ID bits position.
  /// </summary>
  NVMManufacturerPos = 16,
  /// <summary>
  /// Chip Device ID mask. 0x00 - unknown device id.
  /// </summary>
  NVMDeviceMask = 0x0000FF00,
  /// <summary>
  /// Chip Device ID bits position.
  /// </summary>
  NVMDevicePos = 8,
  /// <summary>
  /// Chip Memory size mask. Memory size = num * NVMSizeUnit KBytes. 0x00 - unknown size.
  /// </summary>
  NVMSizeMask = 0x000000FF,
  /// <summary>
  /// Chip Memory size bits position.
  /// </summary>
  NVMSizePos = 0,
  /// <summary>
  /// Chip Memory size unit in kilo bytes (KB).
  /// </summary>
  NVMSizeUnit = 8,

  /// <summary>
  /// Invalid, cant detect, error.
  /// </summary>
  NVMInvalid = 0x00000000,
  /// <summary>
  /// No errors, but chip type is unknown at this stage.
  /// </summary>
  NVMUnknown = 0x00010000,
  /// <summary>
  /// Serial EEPROM chip with SPI bus protocol like in Atmel AT25128 serial EEPROM and compatible (ST95128, etc.).
  /// </summary>
  NVMSerialEEPROM = 0x01000000,
  /// <summary>
  /// Serial Flash chip on SPI bus.
  /// </summary>
  NVMSerialFlash = 0x02000000,
  /// <summary>
  /// Numonyx M25PE10
  /// </summary>
  NVMNumonyxM25PE10 = 0x02208010,
  /// <summary>
  /// Numonyx M25PE20
  /// </summary>
  NVMNumonyxM25PE20 = 0x02208020,
  /// <summary>
  /// Numonyx M25PE40
  /// </summary>
  NVMNumonyxM25PE40 = 0x02208040,
}
;

typedef struct NVM_ADDRESS_
{
	BYTE addres1byte;
	BYTE addres2byte;
	BYTE addres3byte;
	
} NVM_ADDRESS;

#define NvmAddress_2_Dword(address) (DWORD)(((DWORD)address.addres1byte << 16)+((DWORD)address.addres2byte << 8)+(DWORD)address.addres3byte)

#define Dword_2_NvmAddress(dw_addr, nvm_addr)\
        do {nvm_addr.addres1byte=(BYTE)((dw_addr>>16) & 0xFF);\
		    nvm_addr.addres2byte=(BYTE)((dw_addr>>8) & 0xFF);\
			nvm_addr.addres3byte=(BYTE)(dw_addr & 0xFF);} while(0)
	
/*===========================================================================
**    Init the NVM inteface
**
**--------------------------------------------------------------------------*/
DWORD                     /*NVM chip type. Returns nonzero if chip is detected on the bus, 0 - incorrect chip on the bus or can't read the chip ID.*/
NVM_Init(
  volatile BYTE *port,    /*IN The chip select port address*/
  BYTE chipSel)           /*IN  chip select signal mask */
;

/*===========================================================================
**    Get NVM write status
**
**--------------------------------------------------------------------------*/
BYTE                      /*RET TRUE if NVM write in progress */
NVM_Busy(
  void)                   /*IN Nothing */
;

/*============================================================================
**    Read one byte from the NVM
**
**--------------------------------------------------------------------------*/
BYTE                      /*RET NVM data */
NVM_Get(
  NVM_ADDRESS offset)            /*IN offset in the NVM */
;

/*============================================================================
**    Write one byte to the NVM
**
**--------------------------------------------------------------------------*/
void                      /*RET Nothing */
NVM_Put(
  NVM_ADDRESS offset,            /*IN offset in the NVM */
  BYTE bData)             /*IN data to write */
;

/*===========================================================================
**    Read an array of bytes from the NVM to data buffer
**
**--------------------------------------------------------------------------*/
void                      /*RET Nothing */
NVM_GetArray(
  NVM_ADDRESS offset,            /*IN offset in the NVM */
  BYTE *buf,              /*IN destination buffer pointer */
  WORD length)            /*IN number of bytes to read (up to 64K)*/
;

/*==========================================================================
**    Write an array of bytes of up to 64K bytes to the NVM
**
**--------------------------------------------------------------------------*/
void                      /*RET Nothing */
NVM_PutArray(
  NVM_ADDRESS offset,            /*IN offset in the NVM */
  BYTE *buf,              /*IN destination buffer pointer */
  WORD length)            /*IN number of bytes to write (up to 64K)*/
;

/*===========================================================================
**  Write a constant value to an array of bytes to the NVM
**--------------------------------------------------------------------------*/
void
NVM_FillArray(
  NVM_ADDRESS offset,            /*In the offset of array in the NVM to write to*/
  BYTE val,               /*The value to write to the NVM*/
  WORD length)            /*The length of the array in NVM to fill (up to 64K)*/
;

#endif /* NVM_H_ */
