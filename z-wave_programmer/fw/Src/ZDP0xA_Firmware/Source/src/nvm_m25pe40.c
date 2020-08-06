/**********************  nvm_m25pe40.c  *******************************
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
 * Description: (see nvm_m25pe40.h)
 *
 * Author:      Valeriy Vyshnyak
 *
 * Last Changed By:  $Author: vvi $
 * Revision:         $Revision: 0000 $
 * Last Changed:     $Date: 10 лют. 2010 11:22:11 $
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
#define NVMM25PE40_MANUFACTURER_ID      0x20
#define NVMM25PE40_DEVICE_ID_MEMTYPE    0x80
//#define NVMM25PE40_DEVICE_ID_MEMSIZE  0x13  //ignore memsize for now
#define NVMM25PE40_DEVICE_ID_MEMSIZE_1M 0x11  //M25PE10 - 1 Mbit = 128 KBytes
#define NVMM25PE40_DEVICE_ID_MEMSIZE_1M_SIZE (1*1024/8)
#define NVMM25PE40_DEVICE_ID_MEMSIZE_2M 0x12  //M25PE40 - 2 Mbit = 256 KBytes
#define NVMM25PE40_DEVICE_ID_MEMSIZE_2M_SIZE (2*1024/8)
#define NVMM25PE40_DEVICE_ID_MEMSIZE_4M 0x13  //M25PE40 - 4 Mbit = 512 KBytes
#define NVMM25PE40_DEVICE_ID_MEMSIZE_4M_SIZE (4*1024/8)


/*This commands according to data sheet of M2SPE40*/
/*Not all commands are impelemnted*/
/* External serial FLASH commands */
#define NVMM25PE40_WREN             0x06 /* Write Enable   */
#define NVMM25PE40_WRDI             0x04 /* Write disable  */
#define NVMM25PE40_RDSR             0x05 /* Read Status Register       */
#define NVMM25PE40_WRSR             0x01 /* Write Status Register      */
#define NVMM25PE40_READ             0x03 /* Read Data bytes */
#define NVMM25PE40_PW               0x0A /* Page program */
#define NVMM25PE40_RDID             0x9F /* Read chip ID */
#define NVMM25PE40_FAST_READ        0x0B /* Read Data Bytes at Higher Speed */
#define NVMM25PE40_PE               0xDB /* Page Erase */
#define NVMM25PE40_DP               0xB9 /* Deep power-down */
#define NVMM25PE40_RDP              0xAB /* ReleaseDeep power-down */

/* external FALSH  status flags */
#define NVMM25PE40_RDSR_WIP         0x01 /* Write In Progress */

#define NVMM25PE40_PAGE_SIZE        256
#define NVMM25PE40_PAGE_MASK        (NVMM25PE40_PAGE_SIZE - 1)

#define SPIDELAY



/****************************************************************************/
/*                              PRIVATE DATA                                */
/****************************************************************************/
static BYTE NVMM25PE40_writeActive = FALSE;   /* TRUE when an External Flash write is active */
volatile BYTE *NVMM25PE40_csPort;             /*The chip select port address*/
BYTE NVMM25PE40_csMask;                       /*  chip select signal mask */


/****************************************************************************/
/*                               FUNCTIONS                                  */
/****************************************************************************/

/*============================================================================
**    Enable External Flash chip access
**
**--------------------------------------------------------------------------*/
static void               /*RET Nothing */
NVMM25PE40_On(
  void)
{
  *NVMM25PE40_csPort &= ~NVMM25PE40_csMask;
  DELAY_10_US;
}

/*============================================================================
**    Disable External Flash chip access
**
**--------------------------------------------------------------------------*/
static void               /*RET Nothing */
NVMM25PE40_Off(
  void)
{
#ifdef SPIDELAY
  DELAY_10_US;
#endif
  *NVMM25PE40_csPort |= NVMM25PE40_csMask;
  DELAY_10_US;
}

/*===========================================================================
**    Init the M25PE40 External Flash inteface
**
**--------------------------------------------------------------------------*/
DWORD                     /*NVM chip type. Returns nonzero if chip is detected on the bus, 0 - incorrect chip on the bus or can't read the chip ID.*/
NVMM25PE40_Init(
  volatile BYTE *port,    /*IN The chip select port address*/
  BYTE chipSel)           /*IN  chip select signal mask */
{
  DWORD nvm_type = NVMInvalid;
  BYTE manufacturerID, deviceID, sizeID;
  NVMM25PE40_csPort = port;
  NVMM25PE40_csMask = chipSel;
  SPI_Init(&PORTD, &PIND, (1<<MOSI), (1<<MISO), (1<<SCK));

  // wait until the External Flash is ready for next operation
  for (sizeID = 0; sizeID != 255; sizeID++) //delay for about 25 ms
  {
    DELAY_50_US;
    DELAY_50_US;
  }

  // Reading the signature bits.
  NVMM25PE40_On();
  SPI_Write(NVMM25PE40_RDID);
  manufacturerID = SPI_Read();
  if(manufacturerID == NVMM25PE40_MANUFACTURER_ID)
  {
    deviceID = SPI_Read();
    if(deviceID == NVMM25PE40_DEVICE_ID_MEMTYPE)
    {
      sizeID = SPI_Read();
#ifdef NVMM25PE40_DEVICE_ID_MEMSIZE
      if(sizeID == NVMM25PE40_DEVICE_ID_MEMSIZE)
#endif
      {
        BYTE uniqueIDLen = SPI_Read();
        if(uniqueIDLen != 0xff)
        {
          while(uniqueIDLen--)
          {
            SPI_Read();
          }
          //convert ID of the size to the size in units of kilobytes:
          switch (sizeID)
          {
          case NVMM25PE40_DEVICE_ID_MEMSIZE_1M:
            sizeID = NVMM25PE40_DEVICE_ID_MEMSIZE_1M_SIZE / NVMSizeUnit;
            break;
          case NVMM25PE40_DEVICE_ID_MEMSIZE_2M:
            sizeID = NVMM25PE40_DEVICE_ID_MEMSIZE_2M_SIZE / NVMSizeUnit;
            break;
          case NVMM25PE40_DEVICE_ID_MEMSIZE_4M:
            sizeID = NVMM25PE40_DEVICE_ID_MEMSIZE_4M_SIZE / NVMSizeUnit;
            break;
          default:
            sizeID = 0;
          }
          nvm_type = (NVMSerialFlash & NVMTypeMask) |
              ((((DWORD)manufacturerID) << NVMManufacturerPos) & NVMManufacturerMask) |
              ((((DWORD)deviceID) <<  NVMDevicePos) & NVMDeviceMask) |
              ((((DWORD)sizeID) <<  NVMSizePos) & NVMSizeMask);
        }
      }
    }
  }
  NVMM25PE40_Off();
  return nvm_type;
}

/*============================================================================
**    Get External Flash register status
**
**--------------------------------------------------------------------------*/
BYTE                      /*RET External Flash status register */
NVMM25PE40_Stat(
  void)                   /*IN Nothing */
{
  BYTE status;
  NVMM25PE40_On();
  SPI_Write(NVMM25PE40_RDSR);
  status = SPI_Read();
  NVMM25PE40_Off();
  return(status);
}

/*===========================================================================
**    Get External Flash write status
**
**--------------------------------------------------------------------------*/
BYTE                      /*RET TRUE if External Flash write in progress */
NVMM25PE40_Busy(
  void)                   /*IN Nothing */
{
  if (NVMM25PE40_writeActive)
  {
    if (NVMM25PE40_Stat() & NVMM25PE40_RDSR_WIP)
    {
      return TRUE;
    }
    else
    {
      NVMM25PE40_writeActive = FALSE;
      return FALSE;
    }
  }
  return FALSE;
}


/*============================================================================
**    Read one byte from the External Flash
**
**--------------------------------------------------------------------------*/
BYTE                      /*RET External Flash data */
NVMM25PE40_Get(
  NVM_ADDRESS offset)            /*IN offset in the External Flash */
{
  BYTE bData;
  while(NVMM25PE40_Busy()); /* wait until the External Flash is ready for next operation */
  NVMM25PE40_On();
  /* issue read operation */
  SPI_Write(NVMM25PE40_READ);
  /* setup the read address */
  
  SPI_Write(offset.addres1byte);
  SPI_Write(offset.addres2byte);
  SPI_Write(offset.addres3byte);
  /* read the data */
  bData = SPI_Read();
  NVMM25PE40_Off();
  return bData;
}


/*============================================================================
**    Write one byte to the External Flash
**
**--------------------------------------------------------------------------*/
void                      /*RET Nothing */
NVMM25PE40_Put(
  NVM_ADDRESS offset,            /*IN offset in the External Flash */
  BYTE bData)             /*IN data to write */
{
  while(NVMM25PE40_Busy());  /* wait until the External Flash is ready for next operation */
  NVMM25PE40_writeActive = TRUE;
  NVMM25PE40_On();
  /* issue write enable operation */
  SPI_Write(NVMM25PE40_WREN);
  NVMM25PE40_Off();
  NVMM25PE40_On();
  /* issue write operation */
  SPI_Write(NVMM25PE40_PW);
  /* setup the write address */
  
  SPI_Write(offset.addres1byte);
  SPI_Write(offset.addres2byte);
  SPI_Write(offset.addres3byte);
  /* write the data */
  SPI_Write(bData);
  NVMM25PE40_Off();
}


/*===========================================================================
**    Read an array of bytes from the External Flash to data buffer
**
**--------------------------------------------------------------------------*/
void                      /*RET Nothing */
NVMM25PE40_GetArray(
  NVM_ADDRESS offset,            /*IN offset in the External Flash */
  BYTE *buf,              /*IN destination buffer pointer */
  WORD length)            /*IN number of bytes to read (up to 64K)*/
{
  while(NVMM25PE40_Busy()); /* wait until the External Flash is ready for next operation */
  NVMM25PE40_On();
  /* issue read operation */
  SPI_Write(NVMM25PE40_READ);
  /* setup the read address */
  
  SPI_Write(offset.addres1byte);
  SPI_Write(offset.addres2byte);
  SPI_Write(offset.addres3byte);
  /* read the data */
  while (length--)
  {
    *buf++ = SPI_Read();
  }
  NVMM25PE40_Off();
}

/*==========================================================================
**    Write an array of bytes of up to PAGE_SIZE bytes to the one page
**     of the External Flash
**--------------------------------------------------------------------------*/
void                      /*RET Nothing */
NVMM25PE40_PutPage(
  NVM_ADDRESS offset,            /*IN offset in the External Flash */
  BYTE *buf,              /*IN destination buffer pointer, =NULL to write "defaultValue" to all "length" bytes */
  WORD length,            /*IN number of bytes to write */
  BYTE defaultValue)      /*IN default value of data byte*/
{
  while(NVMM25PE40_Busy()); /* wait until the External Flash is ready for next operation */
  NVMM25PE40_writeActive = TRUE;
  NVMM25PE40_On();
  /* issue write enable operation */
  SPI_Write(NVMM25PE40_WREN);
  NVMM25PE40_Off();
  NVMM25PE40_On();
  /* issue write operation */
  SPI_Write(NVMM25PE40_PW);
  /* setup the write address */
 
  SPI_Write(offset.addres1byte);
  SPI_Write(offset.addres2byte);
  SPI_Write(offset.addres3byte);
  /* write the data */
  while (length--)
  {
    if (buf != NULL)
    {
      SPI_Write(*buf++);
    }
    else
    {
      SPI_Write(defaultValue);
    }
  }
  NVMM25PE40_Off();
}

/*==========================================================================
**    Write or Fill an array of bytes of up to 65K bytes to the External Flash
**
**--------------------------------------------------------------------------*/
void                      /*RET Nothing */
NVMM25PE40_PutFillArray(
  NVM_ADDRESS offset,            /*IN offset in the External Flash */
  BYTE *buf,              /*IN destination buffer pointer, =NULL to write "defaultValue" to all "length" bytes */
  WORD length,            /*IN number of bytes to write */
  BYTE defaultValue)      /*IN default value of data byte*/
{
  DWORD i;
  DWORD _offset = NvmAddress_2_Dword(offset);
  /*The bytes written should be aligned on 256 bytes page boundary*/
  /*if the start address is not aligned on this boundary then we first write
   the bytes from start address until the end of the page*/
  i = _offset & NVMM25PE40_PAGE_MASK;
  if (i)
  {
    i = NVMM25PE40_PAGE_SIZE - i;
    if (i > length)
    {
      i = length;
    }
	Dword_2_NvmAddress(_offset, offset);
    NVMM25PE40_PutPage(offset, buf, i, defaultValue);
    length -= i;
    _offset += i;
    if(buf != NULL)
    {
      buf += i;
    }
  }

  while(length)
  {
    if (length > NVMM25PE40_PAGE_SIZE)
    {
      i = NVMM25PE40_PAGE_SIZE;
    }
    else
    {
      i = length;
    }
	Dword_2_NvmAddress(_offset, offset);
    NVMM25PE40_PutPage(offset, buf, i, defaultValue);
    length-=i;
    _offset+=i;
    if(buf != NULL)
    {
      buf += i;
    }
  }
}

/*==========================================================================
**    Write an array of bytes of up to 64K bytes to the External Flash
**
**--------------------------------------------------------------------------*/
void                      /*RET Nothing */
NVMM25PE40_PutArray(
  NVM_ADDRESS offset,            /*IN offset in the External Flash */
  BYTE *buf,              /*IN destination buffer pointer */
  WORD length)            /*IN number of bytes to write (up to 64K)*/
{
  NVMM25PE40_PutFillArray(offset, buf, length, 0);
}

/*===========================================================================
**  Write a constant value to an array of bytes to the External Flash
**--------------------------------------------------------------------------*/
void
NVMM25PE40_FillArray(
  NVM_ADDRESS offset,            /*In the offset of array in the External Flash to write to*/
  BYTE val,               /*The value to write to the External Flash*/
  WORD length)            /*The length of the array in External Flash to fill (up to 64K)*/
{
  NVMM25PE40_PutFillArray(offset, NULL, length, val);
}
