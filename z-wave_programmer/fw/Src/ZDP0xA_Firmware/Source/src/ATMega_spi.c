/*******************************  ATMega_spi.C  *******************************
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
 * Description: Source code of the implementation of the software SPI, which
 *              is connected to the Z-Wave Module.
 *
 * Author:   Ivar Jeppesen
 *
 * Last Changed By:  $Author: vvi $
 * Revision:         $Revision: 15627 $
 * Last Changed:     $Date: 2009-11-18 21:49:04 +0200 (Ср, 18 Лис 2009) $
 *
 ****************************************************************************/

/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
#include "ZWaveFlash.h"
#include <inttypes.h>
#include "lowlevel.h"

#define SPIDELAY
#define BITS_IN_BYTE           8
#define MSB_MASK               0x80

BYTE SPI_UseLowSpeed = FALSE;
volatile BYTE* spiPort =  0;
volatile BYTE* misoPort =  0;
static BYTE MOSIMask = 0;
static BYTE MISOMask = 0;
static BYTE SCKMask = 0;

/****************************************************************************/
/*                      PRIVATE TYPES and DEFINITIONS                       */
/****************************************************************************/

/****************************************************************************/
/*                              PRIVATE DATA                                */
/****************************************************************************/

/****************************************************************************/
/*                              EXPORTED DATA                               */
/****************************************************************************/



/****************************************************************************/
/*                            PRIVATE FUNCTIONS                             */
/****************************************************************************/


/*
 Genetrate High to low clk pulse
*/

static void
ToggleClkHighLow(void)
{
  *spiPort |= SCKMask;
  DELAY_1_CLK;
  *spiPort &= ~SCKMask;
  DELAY_1_CLK;
}
/*
 Genetrate low to high  clk pulse
*/
static void
ToggleClkLowHigh(void)
{
  *spiPort &= ~SCKMask;
  DELAY_1_CLK;
  *spiPort |= SCKMask;
  DELAY_1_CLK;
}


/****************************************************************************/
/*                           EXPORTED FUNCTIONS                             */
/****************************************************************************/
/****************************************************************************/
/*                           EXPORTED FUNCTIONS                             */
/****************************************************************************/
/*===============================   SPI_Init   ==============================
**    Serial Peripheral Interface enabling
**
**    Initializes the integrated SPI controller.
**
**--------------------------------------------------------------------------*/
void                /*RET  Nothing        */
SPI_Init( volatile BYTE *port1,  /*IN The MOSI and SCK port address*/
          volatile BYTE *port2, /*IN the miso port address*/
          BYTE MOSI_Mask, /*IN  MOSI signal mask */
          BYTE MISO_Mask, /*IN MISO signal mask*/
          BYTE SCK_Mask)  /*INSCK signal mask*/
{
	spiPort = port1;
	misoPort = port2;
	MOSIMask = MOSI_Mask;
	MISOMask = MISO_Mask;
	SCKMask = SCK_Mask;
}



void
SPI_Write(BYTE txData)
{
	  BYTE bitNo = 0;
  for (bitNo = 0; bitNo < BITS_IN_BYTE; bitNo++)
  {
  	if ((txData & (MSB_MASK>>bitNo)))
  	{
  	  *spiPort |= MOSIMask;
  	}
  	else
  	{
  		*spiPort &= ~MOSIMask;
  	}
    ToggleClkLowHigh();
  }
  *spiPort &= ~SCKMask;
#ifdef SPIDELAY
  DELAY_10_US;
#endif
}

BYTE
SPI_Read(
		void)
{
  BYTE rxByte = 0, bitNo;
  /*Read the MISP port then togle the sck high low*/
  for (bitNo = 0; bitNo < BITS_IN_BYTE; bitNo++)
  {
    rxByte <<=1;
  	if (*misoPort & MISOMask)
  	{
  		rxByte |= 0x01;
  	}
  	else
  	{
  		rxByte &= ~0x01;
  	}
    ToggleClkHighLow();
  }
#ifdef SPIDELAY
  DELAY_10_US;
#endif
  return rxByte;

}

/*========================   SPI_Exchange   =============================
** Function description
** Exchange a byte on the flash interface (Write and Read)
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
BYTE
SPI_Exchange(
		BYTE txData)
{
  BYTE rxByte = 0, bitNo;
  for(bitNo = 0; bitNo < BITS_IN_BYTE; bitNo++)
  {
  	/*Write the MOSI port (MSB first)*/
  	if (txData & (MSB_MASK >> bitNo))
  	{
  	  *spiPort |= MOSIMask;
  	}
  	else
  	{
  	  *spiPort &= ~MOSIMask;
  	}

    /*Read the MISO port*/
  	rxByte <<= 1;
  	if (*misoPort & MISOMask)
  	{
  		rxByte |= 0x01;
  	}
  	else
  	{
  		rxByte &= ~0x01;
  	}

    /*togle the sck high low*/
  	if(SPI_UseLowSpeed)
  	{
      *spiPort |= SCKMask;
      DELAY_50_US;
      *spiPort &= ~SCKMask;
      DELAY_50_US;
  	}
  	else
  	{
  	  ToggleClkHighLow();
  	}
  }
  if(SPI_UseLowSpeed)
  {
    for(bitNo = 0; bitNo < 32; bitNo++)
    {
      DELAY_50_US;
    }
  }
  else
  {
#ifdef SPIDELAY
  DELAY_10_US;
#endif
  }
  return rxByte;
}
