/*******************************  ATMega_spi.H  *******************************
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
#ifndef _ATMEGA_SPI_H_
#define _ATMEGA_SPI_H_

/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/


/****************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                       */
/****************************************************************************/


/****************************************************************************/
/*                              EXPORTED DATA                               */
/****************************************************************************/
/*
  Send a byte on th flash interface
*/
extern void SPI_Write(BYTE txData);

/*
  Read a byte friom the flash interface
*/
extern BYTE SPI_Read(void);

/*========================   SPI_Exchange   ==============================
** Function description
** Exchange a byte on the flash interface (Write and Read)
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
extern BYTE SPI_Exchange(BYTE txData);
extern BYTE SPI_UseLowSpeed;

/*===============================   SPI_Init   ==============================
**    Serial Peripheral Interface enabling
**
**    Initializes the integrated SPI controller.
**
**--------------------------------------------------------------------------*/
void                /*RET  Nothing        */
SPI_Init( volatile BYTE *port1,  /*IN The MOSI and SCK port address*/
          volatile BYTE *Port2, /*IN the miso port address*/
          BYTE MOSI_Mask, /*IN  MOSI signal mask */
          BYTE MISO_Mask, /*IN MISO signal mask*/
          BYTE SCK_Mask); /*INSCK signal mask*/

#endif /* _ZW_SPIDRIV_H_ */
