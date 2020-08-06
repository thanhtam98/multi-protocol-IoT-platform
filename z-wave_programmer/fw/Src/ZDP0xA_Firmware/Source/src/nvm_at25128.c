/*******************************  nvm_at25128.c  *************************
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
 * Description: (see nvm_at25128.h)
 *
 * Author:   Ivar Jeppesen
 *
 * Last Changed By:  $Author: vvi $
 * Revision:         $Revision: 0000 $
 * Last Changed:     $Date: 2010 $
 *
 ****************************************************************************/

/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
#include "nvm.h"
#include "nvm_m25pe40.h"
#include "ports.h"
#include "ATMega_spi.h"
#include "lowlevel.h"
#include <inttypes.h>

/****************************************************************************/
/*                      PRIVATE TYPES and DEFINITIONS                       */
/****************************************************************************/
/* External EEPROM commands */
#define EEPROM_WREN   6 /* 110 Set Write Enable Latch     */
#define EEPROM_WRDI   4 /* 100 Reset Write Enable Latch   */
#define EEPROM_RDSR   5 /* 101 Read Status Register       */
#define EEPROM_WRSR   1 /* 001 Write Status Register      */
#define EEPROM_READ   3 /* 011 Read Data from Memory Array */
#define EEPROM_WRITE  2 /* 010 Write Data to Memory Array  */

/* EEPROM status flags */
#define EEPROM_RDSR_WPEN 0x80 /* Write Protect Enable */
#define EEPROM_RDSR_BP1  0x08 /* Array Addresses Protected */
#define EEPROM_RDSR_BP0  0x04 /* Array Addresses Protected */
#define EEPROM_RDSR_WEN  0x02 /* 1 indicates the device is WRITE ENABLED. */
#define EEPROM_RDSR_RDY  0x01 /* 1 indicates the write cycle is in progress. */

/* SPI status flags */
//#define SPI_SPA   0x02  /* Transmitting data */
//#define SPI_WCOL  0x01  /* Data colllision */

/* SPI control flags */
//#define SPI_SPE   0x20  /* SPI Enable */
//#define SPI_DORD  0x10  /* MSB first */

#define EEPROM_PAGE_SIZE   32
#define EEPROM_PAGE_MASK   (EEPROM_PAGE_SIZE - 1)

#define SPIDELAY


/****************************************************************************/
/*                              PRIVATE DATA                                */
/****************************************************************************/

static BYTE writeActive = FALSE;  /* TRUE when an EEPROM write is active */

volatile BYTE *csPort;   /*The chip select port address*/
BYTE csMask; /*  chip select signal mask */

/****************************************************************************/
/*                              EXPORTED DATA                               */
/****************************************************************************/

/****************************************************************************/
/*                            PRIVATE FUNCTIONS                             */
/****************************************************************************/
/*==============================   __ExEE_On   ==============================
**    Enable External EEPROM chip access
**
**--------------------------------------------------------------------------*/
static void   /*RET Nothing */
__ExEE_On(void)
{
  *csPort &= ~csMask;
  DELAY_10_US;
}

/*==============================   __ExEE_Off   =============================
**    Disable External EEPROM chip access
**
**--------------------------------------------------------------------------*/
static void   /*RET Nothing */
__ExEE_Off(void)
{
#ifdef SPIDELAY
  DELAY_10_US;
#endif
  *csPort |= csMask;
  DELAY_10_US;
}

/*============================   __ExEEStat   ===============================
**    Get External EEPROM register status
**
**--------------------------------------------------------------------------*/
BYTE               /*RET External EEPROM status register */
__ExEEStat( void ) /*IN Nothing */
{
  register BYTE status;
  __ExEE_On();
  SPI_Write(EEPROM_RDSR);
  status = SPI_Read();
  __ExEE_Off();
  return(status);
}

/****************************************************************************/
/*                           EXPORTED FUNCTIONS                             */
/****************************************************************************/

/*============================   __ExEEBusy   ===============================
**    Get External EEPROM write status
**
**--------------------------------------------------------------------------*/
BYTE               /*RET TRUE if External EEPROM write in progress */
__ExEEBusy( void ) /*IN Nothing */
{
  if (writeActive)
  {
    if (__ExEEStat() & EEPROM_RDSR_RDY)
    {
      return(TRUE);
    }
    else
    {
      writeActive = FALSE;
      return(FALSE);
    }
  }
  return (FALSE);
}


/*============================   __ExEEGet   ================================
**    Read one byte from the External EEPROM
**
**--------------------------------------------------------------------------*/
BYTE            /*RET External EEPROM data */
__ExEEGet(
  NVM_ADDRESS offset )   /*IN offset in the External EEPROM */
{
  register BYTE bData;
  while(__ExEEBusy());
  __ExEE_On();
  /* issue read operation */
  SPI_Write(EEPROM_READ);
  /* setup the read address */
  SPI_Write(offset.addres2byte);
  SPI_Write(offset.addres3byte);
  /* read the data */
  bData = SPI_Read();
  __ExEE_Off();
  return(bData);
}


/*============================   __ExEEPut   ================================
**    Write one byte to the External EEPROM
**
**--------------------------------------------------------------------------*/
void           /*RET Nothing */
__ExEEPut(
  NVM_ADDRESS offset,   /*IN offset in the External EEPROM */
  BYTE bData )   /*IN data to write */
{
  while(__ExEEBusy());
  writeActive = TRUE;
  __ExEE_On();
  /* issue write enable operation */
  SPI_Write(EEPROM_WREN);
  __ExEE_Off();
  __ExEE_On();
  /* issue write operation */
  SPI_Write(EEPROM_WRITE);
  /* setup the write address */
  SPI_Write(offset.addres2byte);
  SPI_Write(offset.addres3byte);
  /* write the data */
  SPI_Write(bData);
  __ExEE_Off();
}


/*============================   __ExEEGetArray   ===========================
**    Read an array of bytes from the External EEPROM to data buffer
**
**--------------------------------------------------------------------------*/
void            /*RET Nothing */
__ExEEGetArray(
  NVM_ADDRESS offset,    /*IN offset in the External EEPROM */
  BYTE *buf,      /*IN destination buffer pointer */
  WORD length)    /*IN number of bytes to read (up to 64K)*/
{
  while(__ExEEBusy());

  __ExEE_On();
  /* issue read operation */
  SPI_Write(EEPROM_READ);
  /* setup the read address */
  SPI_Write(offset.addres2byte);
  SPI_Write(offset.addres3byte);
  /* read the data */
  while (length--)
  {
    *buf++ = SPI_Read();
  }
  __ExEE_Off();
}


/*===========================================================================
**    Write an array of up to 64 bytes to the External EEPROM
**
**--------------------------------------------------------------------------*/
void                      /*RET Nothing */
__ExEEPutPage(
  NVM_ADDRESS offset,            /*IN offset in the External EEPROM */
  BYTE *buf,              /*IN destination buffer pointer, =NULL to write "defaultValue" to all "length" bytes */
  WORD length,            /*IN number of bytes to write */
  BYTE defaultValue)      /*IN default value of data byte*/
{
  while(__ExEEBusy());
  writeActive = TRUE;
  __ExEE_On();
  /* issue write enable operation */
  SPI_Write(EEPROM_WREN);
  __ExEE_Off();
  __ExEE_On();
  /* issue write operation */
  SPI_Write(EEPROM_WRITE);
  /* setup the write address */
  SPI_Write(offset.addres2byte);
  SPI_Write(offset.addres3byte);
  /* write the data */
  while (length--)
  {
    if (buf)
    {
      SPI_Write(*buf++);
    }
    else
    {
      SPI_Write(defaultValue);
    }
  }
  __ExEE_Off();

}

/*==========================================================================
**    Write or Fill an array of bytes of up to 65K bytes to the External Flash
**
**--------------------------------------------------------------------------*/
void                      /*RET Nothing */
__ExEEPutFillArray(
  NVM_ADDRESS offset,            /*IN offset in the External Flash */
  BYTE *buf,              /*IN destination buffer pointer, =NULL to write "defaultValue" to all "length" bytes */
  WORD length,            /*IN number of bytes to write */
  BYTE defaultValue)      /*IN default value of data byte*/
{
  register DWORD i;
  DWORD _offset = NvmAddress_2_Dword(offset);
  /*The bytes written should be aligned on 256 bytes page boundary*/
  /*if the start address is not aligned on this boundary then we first write
   the bytes from start address until the end of the page*/
  i = _offset & EEPROM_PAGE_MASK;
  if (i)
  {
    i = EEPROM_PAGE_SIZE - i;
    if ( length <= i)
    {
      i = length;
    }
	Dword_2_NvmAddress(_offset, offset);
    __ExEEPutPage(offset, buf, i, defaultValue);
    length -= i;
    _offset += i;
    if(buf != NULL)
    {
      buf += i;
    }
  }

  while(length)
  {
    if (length > EEPROM_PAGE_SIZE)
    {
      i = EEPROM_PAGE_SIZE;
    }
    else
    {
      i = length;
    }
	Dword_2_NvmAddress(_offset, offset);
    __ExEEPutPage(offset, buf, i, defaultValue);
    length-=i;
    _offset+=i;
    if(buf != NULL)
    {
      buf += i;
    }
  }
}

/*============================   __ExEEPutArray   ================================
**    Write an array of up to 64K bytes to the External EEPROM
**
**--------------------------------------------------------------------------*/
void           /*RET Nothing */
__ExEEPutArray(
  NVM_ADDRESS offset,   /*IN offset in the External EEPROM */
  BYTE *buf,      /*IN destination buffer pointer */
  WORD length)    /*IN number of bytes to write (up to 64K)*/
{
  __ExEEPutFillArray(offset, buf, length, 0);
}

/*============================   __ExEEFillArray   ===========================
**  Write a constant value to an array of bytes to the External EEPROM
**--------------------------------------------------------------------------*/
void
__ExEEFillArray(
  NVM_ADDRESS offset,     /*In the offset of array in the EEPROM to write to*/
  BYTE val,        /*The value to write to the EEPROM*/
  WORD length)     /*The length of the array in EEPROM to fill (up to 64K)*/
{
  __ExEEPutFillArray(offset, NULL, length, val);
}

/*============================   __ExEEProtect   ===========================
**    Setup EEPROM protection mode
**
**--------------------------------------------------------------------------*/
//void            /*RET Nothing */
//__ExEEProtect(
//  BYTE protectMode)   /*IN 0: unprotect */
//{
//  while(__ExEEBusy());
//  __ExEE_On();
//  SPI_Write(EEPROM_WREN); /* disable write operation*/
//  __ExEE_Off();
//  __ExEE_On();
//  /* issue write status operation */
//  SPI_Write(EEPROM_WRSR);
//  /* setup the protection mode */
//  SPI_Write(protectMode);
//  __ExEE_Off();
//  __ExEE_On();
//  SPI_Write(EEPROM_WRDI); /* disable write operation*/
//  __ExEE_Off();
//
//}


/*
 Init the EEPROM inteface
*/
DWORD                     /*NVM chip type. Returns nonzero if chip is detected on the bus, 0 - incorrect chip on the bus or can't read the chip ID.*/
_ExEEPInit( volatile BYTE *port   /*IN The chip select port address*/
                   ,BYTE chipSel) /*IN  chip select signal mask */
{
	csPort = port;
	csMask = chipSel;
	SPI_Init(&PORTD, &PIND, (1<<MOSI), (1<<MISO), (1<<SCK));
	return NVMSerialEEPROM;
}

