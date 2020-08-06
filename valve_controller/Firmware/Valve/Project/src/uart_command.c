/****************************************************************************
 *
 * Copyright (c) 2001-2013
 * Sigma Designs, Inc.
 * All Rights Reserved
 *
 *---------------------------------------------------------------------------
 *
 * Description: Slave application for evaluation kit LED dimmer
 *
 * Author:   Tuan Nguyen Khac Phuong
 *
 * Last Changed By:  $User: NTT $
 * Revision:         $Revision: 1.0 $
 * Last Changed:     $Date: 2018-01-31 14:01:44 +0700 (We, 31 January 2018) $
 
 *
 ****************************************************************************/


/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
#include <uart_command.h>

/****************************************************************************/
/*                              PRIVATE DATA                                */
/****************************************************************************/
volatile BYTE ZW_UART_RECEIVE=0;
volatile BYTE ZW_UART_RECEIVE_STATUS=0;

BYTE serBuf[SERBUF_MAX];
IBYTE serBufLen, con_state;
IBYTE bChecksum_RX;
volatile BOOL AckNakNeeded = FALSE;
volatile BOOL AckNak = ACK;
/****************************************************************************/
/*                           EXPORTED FUNCTIONS                             */
/****************************************************************************/


/*===========================   Uart_send_command   =========================
**    This function send command via uart.
**
**    Side effects:
**
**--------------------------------------------------------------------------*/
BOOL Uart_send_command(ZW_UART_COMMAND cmd)
{
  BYTE* cmd_point;
  BYTE j,bChecksum;
  DWORD i;
  for(j=1;j>0;j--)
  {
    bChecksum =0xFF;
    ZW_UART_SEND_BYTE(SOF);
    cmd_point= &cmd.zw_uartcommand.length;
    i= *(cmd_point);
    for(;i>0;i--)
    {
      ZW_UART_SEND_BYTE(*(cmd_point));
      bChecksum ^= *(cmd_point++);
    }
    ZW_UART_SEND_BYTE(bChecksum);
    AckNak = NAK;
    AckNakNeeded = TRUE;  // Now we need an ACK...
    for(i=100000;i>0;i--)
    {
      if(AckNakNeeded==FALSE)
      {
        break;
      }
    }
    if(AckNak == ACK)
    {
      return TRUE;
    }
  }
  return FALSE;
}
T_CON_TYPE Uart_update(BYTE acknowledge)
{
  BYTE c;
  T_CON_TYPE retVal = conIdle;
  if(ZW_UART_REC_STATUS)
  {
    do
    {
      c = ZW_UART_REC_BYTE;
       switch (con_state)
      {
        case stateSOFHunt :
          if (c == SOF)
          {
            bChecksum_RX = 0xff;
            serBufLen = 0;
            con_state++;
          }
          else
          {
            if (AckNakNeeded)
            {
              if (c == ACK)
              {
                retVal = conFrameSent;
                AckNakNeeded = FALSE;  // Done
                AckNak = ACK;
              }
              else if (c == NAK)
              {
                retVal = conTxTimeout;
                AckNakNeeded = FALSE;
                AckNak = NAK;
              }
              else
              {
                // Bogus character received...
              }
            }
          }
          break;
        case stateLen :
          // Check for length to be inside valid range
          if ((c < FRAME_LENGTH_MIN) || (c > FRAME_LENGTH_MAX))
          {
            con_state = stateSOFHunt; // Restart looking for SOF
            break;
          }
          // Drop through...  

        case stateCmd :
          if (serBufLen && (c > COMMAND_END))
          {
            con_state = stateSOFHunt; // Restart looking for SOF
            break;
          }
          // Drop through...

        case stateType :
          con_state++;
          // Drop through...
        case stateData :
          if (serBufLen < SERBUF_MAX)
          {
            serBuf[serBufLen] = c;
            serBufLen++;
            bChecksum_RX ^= c;
            if (serBufLen >= serFrameLen)
            {
              con_state++;
            }
          }
          else
          {
            con_state++;
          }
          break;
          
        case stateChecksum :
          /* Do we send ACK/NAK according to checksum... */
          /* if not then the received frame is dropped! */
        
          if (acknowledge)
          {
            if (c == bChecksum_RX)
            {   
              ZW_UART_SEND_BYTE(ACK);
              retVal = conFrameReceived;  // Tell THE world that we got a packet
            }
            else
            {
              ZW_UART_SEND_BYTE(NAK);       // Tell them something is wrong...
              retVal = conFrameErr;
            }
          }
          else
          {
            // We are in the process of looking for an acknowledge to a callback request
            // Drop the new frame we received - we don't have time to handle it.
            // Send a CAN to indicate what is happening...
            ZW_UART_SEND_BYTE(CAN);
          }
          // Drop through

        default :
          con_state = stateSOFHunt; // Restat looking for SOF
          break;
      }
    }while((retVal == conIdle) && ZW_UART_REC_STATUS);
  }
  return retVal;
}
