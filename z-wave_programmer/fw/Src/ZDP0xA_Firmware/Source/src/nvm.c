/**********************  nvm.c  *******************************
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
 * Description: (see nvm.h)
 *
 * Author:      Valeriy Vyshnyak
 *
 * Last Changed By:  $Author: vvi $
 * Revision:         $Revision: 0000 $
 * Last Changed:     $Date: 11 лют. 2010 14:57:31 $
 *
 ****************************************************************************/

/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
#include "nvm.h"
#include "nvm_at25128.h"
#include "nvm_m25pe40.h"

/****************************************************************************/
/*                      PRIVATE TYPES and DEFINITIONS                       */
/****************************************************************************/
typedef BYTE (*NVM_BusyFunc)(void);

typedef struct NVM_INTERFACE_
{
  BYTE (*BusyFunc)(void);
  BYTE (*GetFunc)(NVM_ADDRESS offset);
  void (*PutFunc)(NVM_ADDRESS offset, BYTE bData);
  void (*GetArrayFunc)(NVM_ADDRESS offset, BYTE *buf, WORD length);
  void (*PutArrayFunc)(NVM_ADDRESS offset, BYTE *buf, WORD length);
  void (*FillArray)(NVM_ADDRESS offset, BYTE val, WORD length);
} NVM_INTERFACE;


/****************************************************************************/
/*                              PRIVATE DATA                                */
/****************************************************************************/
BOOL NVM_InterfaceIsValid = FALSE;
NVM_INTERFACE NVM_Interface;

/****************************************************************************/
/*                               FUNCTIONS                                  */
/****************************************************************************/

/*===========================================================================
**    Init the M25PE40 NVM inteface
**
**--------------------------------------------------------------------------*/
DWORD                     /*NVM chip type. Returns nonzero if chip is detected on the bus, 0 - incorrect chip on the bus or can't read the chip ID.*/
NVM_Init(
  volatile BYTE *port,    /*IN The chip select port address*/
  BYTE chipSel)           /*IN  chip select signal mask */
{
  DWORD nvm_type = NVMInvalid;
  NVM_InterfaceIsValid = FALSE;
  if((nvm_type = NVMM25PE40_Init(port, chipSel)) != NVMInvalid)
  {
    NVM_Interface.BusyFunc = NVMM25PE40_Busy;
    NVM_Interface.GetFunc = NVMM25PE40_Get;
    NVM_Interface.PutFunc = NVMM25PE40_Put;
    NVM_Interface.GetArrayFunc = NVMM25PE40_GetArray;
    NVM_Interface.PutArrayFunc = NVMM25PE40_PutArray;
    NVM_Interface.FillArray = NVMM25PE40_FillArray;
    NVM_InterfaceIsValid = TRUE;
  }
  else
  {
    nvm_type = _ExEEPInit(port, chipSel);
    NVM_Interface.BusyFunc = __ExEEBusy;
    NVM_Interface.GetFunc = __ExEEGet;
    NVM_Interface.PutFunc = __ExEEPut;
    NVM_Interface.GetArrayFunc = __ExEEGetArray;
    NVM_Interface.PutArrayFunc = __ExEEPutArray;
    NVM_Interface.FillArray = __ExEEFillArray;
    NVM_InterfaceIsValid = TRUE;
  }
  return nvm_type;
}

/*===========================================================================
**    Get NVM write status
**
**--------------------------------------------------------------------------*/
BYTE                      /*RET TRUE if NVM write in progress */
NVM_Busy(
  void)                   /*IN Nothing */
{
  if(!NVM_InterfaceIsValid)
  {
    return 0;
  }
  return NVM_Interface.BusyFunc();
}


/*============================================================================
**    Read one byte from the NVM
**
**--------------------------------------------------------------------------*/
BYTE                      /*RET NVM data */
NVM_Get(
  NVM_ADDRESS offset)            /*IN offset in the NVM */
{
  if(!NVM_InterfaceIsValid)
  {
    return 0;
  }
  return NVM_Interface.GetFunc(offset);
}


/*============================================================================
**    Write one byte to the NVM
**
**--------------------------------------------------------------------------*/
void                      /*RET Nothing */
NVM_Put(
  NVM_ADDRESS offset,            /*IN offset in the NVM */
  BYTE bData)             /*IN data to write */
{
  if(!NVM_InterfaceIsValid)
  {
    return;
  }
  NVM_Interface.PutFunc(offset, bData);
}


/*===========================================================================
**    Read an array of bytes from the NVM to data buffer
**
**--------------------------------------------------------------------------*/
void                      /*RET Nothing */
NVM_GetArray(
  NVM_ADDRESS offset,            /*IN offset in the NVM */
  BYTE *buf,              /*IN destination buffer pointer */
  WORD length)            /*IN number of bytes to read (up to 64K)*/
{
  if(!NVM_InterfaceIsValid)
  {
    return;
  }
  NVM_Interface.GetArrayFunc(offset, buf, length);
}

/*==========================================================================
**    Write an array of bytes of up to 64K bytes to the NVM
**
**--------------------------------------------------------------------------*/
void                      /*RET Nothing */
NVM_PutArray(
  NVM_ADDRESS offset,            /*IN offset in the NVM */
  BYTE *buf,              /*IN destination buffer pointer */
  WORD length)            /*IN number of bytes to write (up to 64K)*/
{
  if(!NVM_InterfaceIsValid)
  {
    return;
  }
  NVM_Interface.PutArrayFunc(offset, buf, length);
}

/*===========================================================================
**  Write a constant value to an array of bytes to the NVM
**--------------------------------------------------------------------------*/
void
NVM_FillArray(
  NVM_ADDRESS offset,            /*In the offset of array in the NVM to write to*/
  BYTE val,               /*The value to write to the NVM*/
  WORD length)            /*The length of the array in NVM to fill (up to 64K)*/
{
  if(!NVM_InterfaceIsValid)
  {
    return;
  }
  NVM_Interface.FillArray(offset, val, length);
}

