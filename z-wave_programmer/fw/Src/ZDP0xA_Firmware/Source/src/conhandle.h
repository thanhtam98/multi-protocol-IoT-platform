/******************************  CONHANDLE.H  *******************************
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
 * Description:       Source files, contains the functions for handling the
 *                    Programmer frames via the UART.
 *
 * Author:          Kim Jakobsen
 *
 * Last Changed By:  $Author: sse $
 * Revision:         $Revision: 7850 $
 * Last Changed:     $Date: 2006-07-05 16:05:09 +0300 (Ср, 05 Лип 2006) $
 *
 ****************************************************************************/
#ifndef _CONHANDLE_H_
#define _CONHANDLE_H_

#include "ZWaveFlash.h"

/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/


/****************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                       */
/****************************************************************************/
/* return values for ConUpdate */
enum T_CON_TYPE
{
  conIdle,            // returned if nothing special has happened
  conFrameReceived,   // returned when a valid frame has been received
  conFrameSent,       // returned if frame was ACKed by the other end
  conFrameErr,        // returned if frame has error in Checksum
  conRxTimeout,       // returned if Rx timeout has happened
  conTxTimeout,        // returned if Tx timeout (waiting for ACK) ahs happened
  conError
};

/* defines for accessing serial protocol data */
#define serFrameLen (*serBuf)
#define serFrameType (*(serBuf + 2))
#define serFrameCmd (*(serBuf + 3))
#define serFrameDataPtr (serBuf + 4)

/****************************************************************************/
/*                              EXPORTED DATA                               */
/****************************************************************************/
/* serial buffer size */
#define SERBUF_MAX  (1024 + 18) //264
#define FRAME_LENGTH_MIN  4
#define FRAME_LENGTH_MAX  SERBUF_MAX

#define RX_BYTE_TIMEOUT 100

#define SOF 0x01
#define ACK 0x06
#define NAK 0x15


//123 //extern BYTE* serBuf;

extern BYTE serBuf[SERBUF_MAX];
extern WORD serBufLen;


/****************************************************************************/
/*                           EXPORTED FUNCTIONS                             */
/****************************************************************************/

/*===============================   ConTxFrame   =============================
**
**   Transmit frame via serial port by adding SOF, Len, Type, cmd and Chksum.
**   Frame  : SOF-Len-Type-Cmd-DATA-Chksum
**    where SOF is Start of frame byte
**          Len is length of bytes between and including Len to last DATA byte
**          Type is Response or Request
**          Cmd Serial application command describing what DATA is
**          DATA as it says
**          Chksum is a XOR checksum taken on all BYTEs from Len to last DATA byte
**
**          NOTE: If Buf is NULL then the previously used cmd, type, Buf and len
**          is used again (Retransmission)
**
**--------------------------------------------------------------------------*/
extern void   /*RET Nothing */
ConTxFrame(
  BYTE cmd,   /* IN Command */
  BYTE type,  /* IN frame Type to send (Response or Request) */
  BYTE *Buf, /* IN pointer to BYTE buffer containing DATA to send */
  WORD len);   /* IN the length of DATA to transmit */
/*==============================   ConUpdate   =============================
**
**   Parses serial data sent from external controller module (PC-controller).
**   Should be frequently called by main loop.
**
**   Return: conIdle          if nothing has happened
**           conFrameReceived if valid frame was received
**           conFrameSent     if transmitted frame was ACKed by other end
**           conFrameErr      if received frame has error in Checksum
**           conRxTimeout     if a Rx timeout happened
**           conTxTimeout     if a Tx timeout waiting for ACK happened
**
**--------------------------------------------------------------------------*/
extern enum T_CON_TYPE  /*RET conState (See above) */
ConUpdate(
  BYTE acknowledge);    /* IN Nothing */

/*==============================   ConInit   =============================
**
**   Initialize the module.
**
**--------------------------------------------------------------------------*/
void                /*RET  Nothing */
ConInit(void);       /*IN   Nothing */

/*
  Chech if there are any activity at the serial port
*/
BYTE
ConActive(void);

#endif /* _CONHANDLE_H_ */
