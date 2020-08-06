/**********************  nvm_m25pe40.h  *******************************
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
 * Description: Source code of the Z-Wave Module External NVM interface driver
 *               for Serial Flash Memory
 *              Next chips are detected and supported:
 *               Numonyx M25PE10 (first 64K bytes only)
 *               Numonyx M25PE20 (first 64K bytes only)
 *               Numonyx M25PE40 (first 64K bytes only)
 *
 * Author:      Valeriy Vyshnyak
 *
 * Last Changed By:  $Author: vvi $
 * Revision:         $Revision: 0000 $
 * Last Changed:     $Date: 10 лют. 2010 11:22:45 $
 *
 ****************************************************************************/


#ifndef NVM_M25PE40_H_
#define NVM_M25PE40_H_

/****************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                       */
/****************************************************************************/


/****************************************************************************/
/*                              EXPORTED DATA                               */
/****************************************************************************/
DWORD                     /*NVM chip type. Returns nonzero if chip is detected on the bus, 0 - incorrect chip on the bus or can't read the chip ID.*/
NVMM25PE40_Init(
  volatile BYTE *port,    /*IN The chip select port address*/
  BYTE chipSel)           /*IN  chip select signal mask */
;

/*===========================================================================
**    Get External Flash write status
**
**--------------------------------------------------------------------------*/
BYTE                      /*RET TRUE if External Flash write in progress */
NVMM25PE40_Busy(
  void)                   /*IN Nothing */
;

/*============================================================================
**    Read one byte from the External Flash
**
**--------------------------------------------------------------------------*/
BYTE                      /*RET External Flash data */
NVMM25PE40_Get(
  NVM_ADDRESS offset)            /*IN offset in the External Flash */
;

/*============================================================================
**    Write one byte to the External Flash
**
**--------------------------------------------------------------------------*/
void                      /*RET Nothing */
NVMM25PE40_Put(
  NVM_ADDRESS offset,            /*IN offset in the External Flash */
  BYTE bData)             /*IN data to write */
;

/*===========================================================================
**    Read an array of bytes from the External Flash to data buffer
**
**--------------------------------------------------------------------------*/
void                      /*RET Nothing */
NVMM25PE40_GetArray(
  NVM_ADDRESS offset,            /*IN offset in the External Flash */
  BYTE *buf,              /*IN destination buffer pointer */
  WORD length)            /*IN number of bytes to read (up to 64K)*/
;

/*==========================================================================
**    Write an array of bytes of up to 64K bytes to the External Flash
**
**--------------------------------------------------------------------------*/
void                      /*RET Nothing */
NVMM25PE40_PutArray(
  NVM_ADDRESS offset,            /*IN offset in the External Flash */
  BYTE *buf,              /*IN destination buffer pointer */
  WORD length)            /*IN number of bytes to write (up to 64K)*/
;

/*===========================================================================
**  Write a constant value to an array of bytes to the External Flash
**--------------------------------------------------------------------------*/
void
NVMM25PE40_FillArray(
  NVM_ADDRESS offset,            /*In the offset of array in the External Flash to write to*/
  BYTE val,               /*The value to write to the External Flash*/
  WORD length)            /*The length of the array in External Flash to fill (up to 64K)*/
;

#endif /* NVM_M25PE40_H_ */
