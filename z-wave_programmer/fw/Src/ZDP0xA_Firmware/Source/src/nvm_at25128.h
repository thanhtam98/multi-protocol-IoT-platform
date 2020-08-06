/*******************************  nvm_at25128.h  *******************************
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
 *              Copyright (c) 2001
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
 * Description: Source code of the Z-Wave Module External NVM interface driver
 *               for Serial EEPROM
 *              Next chips are supported (no autodetection):
 *               Atmel AT25128 and compatible.
 *               STMicroelectronics ST95128 and compatible.
 *               Other Seral EEPROM's with compatible SPI interface.
 *
 * Author:   Ivar Jeppesen
 *
 * Last Changed By:  $Author: vvi $
 * Revision:         $Revision: 0000 $
 * Last Changed:     $Date: 2010 $
 *
 ****************************************************************************/
#ifndef _EEPROM_IF_H_
#define _EEPROM_IF_H_

/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/


/****************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                       */
/****************************************************************************/


/****************************************************************************/
/*                              EXPORTED DATA                               */
/****************************************************************************/
/*============================   __ExEEBusy   ===============================
**    Get External EEPROM write status
**
**--------------------------------------------------------------------------*/
extern BYTE          /*RET TRUE if External EEPROM write in progress */
__ExEEBusy( void );  /*IN Nothing */

/*============================   __ExEEGet   ==================================
**    Read one byte from the External EEPROM
**
**--------------------------------------------------------------------------*/
extern BYTE     /*RET External EEPROM data */
__ExEEGet(
NVM_ADDRESS offset );  /*IN offset in the External EEPROM */

/*============================   __ExEEGetArray   ===========================
**    Read an array of bytes from the External EEPROM to data buffer
**
**--------------------------------------------------------------------------*/
void            /*RET Nothing */
__ExEEGetArray(
  NVM_ADDRESS offset,    /*IN offset in the External EEPROM */
  BYTE *buf,      /*IN destination buffer pointer */
  WORD length)    /*IN number of bytes to read (up to 64K)*/
;

/*============================   __ExEEPut   ================================
**    Write one byte to the External EEPROM
**
**--------------------------------------------------------------------------*/
extern void    /*RET Nothing */
__ExEEPut(
NVM_ADDRESS offset,   /*IN offset in the External EEPROM */
BYTE bData );  /*IN data to write */


/*============================   __ExEEPutArray   ================================
**    Write an array of up to 64K bytes to the External EEPROM
**
**--------------------------------------------------------------------------*/
void           /*RET Nothing */
__ExEEPutArray(
  NVM_ADDRESS offset,   /*IN offset in the External EEPROM */
  BYTE *buf,      /*IN destination buffer pointer */
  WORD length)    /*IN number of bytes to write (up to 64K)*/
;

/*============================   __ExEEFillArray   ===========================
**  Write a constant value to an array of bytes to the External EEPROM
**--------------------------------------------------------------------------*/
void
__ExEEFillArray(
  NVM_ADDRESS offset,     /*In the offset of array in the EEPROM to write to*/
  BYTE val,        /*The value to write to the EEPROM*/
  WORD length)     /*The length of the array in EEPROM to fill (up to 64K)*/
;

/*============================   __ExEEProtect   ===========================
**    Setup EEPROM protection mode
**
**--------------------------------------------------------------------------*/
//void            /*RET Nothing */
//__ExEEProtect(
//  BYTE protectMode);   /*IN 0: unprotect */

/*
 Init the EEPROM inteface
*/
DWORD                     /*NVM chip type. Returns nonzero if chip is detected on the bus, 0 - incorrect chip on the bus or can't read the chip ID.*/
_ExEEPInit( volatile BYTE *port   /*IN The chip select port address*/
                   ,BYTE chipSel); /*IN  chiop select signal mask */
#endif /* _ZW_SPIDRIV_H_ */
