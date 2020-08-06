/*******************************  CONHANDLE.C  ******************************
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
 * Author:            Kim Jakobsen
 *
 * Last Changed By:  $Author: vvi $
 * Revision:         $Revision: 15466 $
 * Last Changed:     $Date: 2009-11-02 20:22:31 +0200 (Пн, 02 Лис 2009) $
 *
 ****************************************************************************/

/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
//#include <ZW_sysdefs.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "UART_buf_io.h"
#include "conhandle.h"

/****************************************************************************/
/*                      PRIVATE TYPES and DEFINITIONS                       */
/****************************************************************************/
/* serial Protocol handler states */
enum
{
  stateSOFHunt = 0,
  stateLenH = 1,
  stateLenL = 2,
  stateType = 3,
  stateCmd = 4,
  stateData = 5,
  stateChecksum = 6,
};

/****************************************************************************/
/*                              PRIVATE DATA                                */
/****************************************************************************/
//123 //BYTE* serBuf;

BYTE serBuf[SERBUF_MAX];
WORD serBufLen;
BYTE bChecksum, state;

/****************************************************************************/
/*                              EXPORTED DATA                               */
/****************************************************************************/


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
void          /*RET Nothing */
ConTxFrame(
  BYTE cmd,   /* IN Command */
  BYTE type,  /* IN frame Type to send (Response or Request) */
  BYTE *Buf, /* IN pointer to BYTE buffer containing DATA to send */
  WORD len)   /* IN the length of DATA to transmit */
{
  static BYTE wCmd, wType, *wBuf, tmp;
  static WORD wLen;
  BYTE  bChecksum;
  WORD i;


  if (Buf != NULL)
  {
    wBuf = Buf;
    wLen = len;
    wCmd = cmd;
    wType = type;
  }
  bChecksum = 0xFF; /* Initialize checksum */
  SerialPutByte(SOF);
   // Remember the 'len' word and  'type' and 'cmd' bytes
  SerialPutByte((wLen + 4) >> 8);
  SerialPutByte( (wLen + 4) & 0xFF);  // wLen lsb
  tmp = (wLen + 4) >> 8;
  bChecksum ^= tmp;

  tmp = (wLen + 4) & 0xFF;
  bChecksum ^= tmp;

  SerialPutByte(wType);
  bChecksum ^= wType;
  SerialPutByte(wCmd);
  bChecksum ^= wCmd;
  for (i = 0; (wBuf != NULL) && (i < wLen); i++)
  {
    SerialPutByte(wBuf[i]);
    bChecksum ^= wBuf[i];
  }
  SerialPutByte(bChecksum);       // XOR checksum of
  ResetSerialTimer();
  SIOReg.AckNakNeeded = 1;  // Now we need an ACK...
}


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
enum T_CON_TYPE     /*RET conState - See above */
ConUpdate(
  BYTE acknowledge) /* IN do we send acknowledge and handle frame if received correctly */
{
  BYTE c;
  enum T_CON_TYPE retVal = conIdle;
  static WORD len = 0;

  if (SerialCheck())
  {
    c = SerialGetByte();

    switch (state)
    {
      case stateSOFHunt :

        if (c == SOF)
        {
          state++;
          bChecksum = 0xff;
          serBufLen = 0;

          SIOReg.rxActive = 1;  // now we're receiving - check for timeout
          ResetSerialTimer();
        }
        else
        {
          if (SIOReg.AckNakNeeded)
          {
            if (c == ACK)
            {
              SIOReg.AckNakNeeded = 0;  // Done
              retVal = conFrameSent;
            }
            else
            {
              retVal = conError;
            }
          }
          else
          {
            retVal = conError;
          }

        }
        break;

      case stateLenH :
        ResetSerialTimer();
        state++;
        serBuf[serBufLen++] = c;
        len = c;
        len<<=8;
        bChecksum ^= c;
      break;
      case stateLenL:
        ResetSerialTimer();
        state++;
        serBuf[serBufLen++] = c;
        len |= c & 0xFF;
        bChecksum ^= c;
      break;

      case stateType :
      case stateCmd :
        state++;
          // Drop through...

      case stateData :
        ResetSerialTimer();
        serBuf[serBufLen] = c;
        serBufLen++;
        bChecksum ^= c;
        if (serBufLen >= len)
        {
          state++;
        }
        break;

      case stateChecksum :
        if (acknowledge)
        {
          if (c == bChecksum)
          {
            SerialPutByte(ACK);
            retVal = conFrameReceived;  // Tell THE world that we got a packet
          }
          else
          {
            SerialPutByte(NAK);       // Tell them something is wrong...
            retVal = conFrameErr;
          }
        }
        else
        {
          retVal = conFrameReceived;  // Tell THE world that we got a packet
        }
      default :
        state = stateSOFHunt; // Restart looking for SOF
        SIOReg.rxActive = 0;  // Not really active now...
        break;
    }
  }
  else
  {
    // Check for timeout
    if ((SIOReg.rxActive) || (SIOReg.AckNakNeeded))
    {
    	cli();
    	WORD tmp = GetSerialTimer();
    	sei();
      if (tmp >= RX_BYTE_TIMEOUT)
      {
        state = stateSOFHunt;
        SIOReg.Timeout = 1;
        if (SIOReg.rxActive)
        {
          retVal = conRxTimeout;
        }
        else
        {
          retVal = conTxTimeout;
        }
        ResetSerialTimer();
        SIOReg.rxActive = SIOReg.AckNakNeeded = 0;
      }
    }
    else
    {
    	ResetSerialTimer();
    }
  }
  return retVal;

}


/*==============================   ConInit   =============================
**
**   Initialize the module.
**
**--------------------------------------------------------------------------*/
//123 //extern BYTE compl_workbuf[1024 + 4*256 + 18];    /* Used for frames send to remote side. */

void                /*RET  Nothing */
ConInit(void)       /*IN   Nothing */
{
//123  //serBuf = compl_workbuf;

  serBufLen = 0;
  state = stateSOFHunt;
  InitSerialIf ();
}

/*
  Chech if there are any activity at the serial port
*/
BYTE
ConActive(void)
{
	return SerialCheck();
}
