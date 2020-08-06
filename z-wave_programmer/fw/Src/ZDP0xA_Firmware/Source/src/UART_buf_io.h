/*****************************  UART_BUF_IO.H  ******************************
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
 * Description:       Buffered transmit/receive of data through the UART
 *
 * Author:            Kim Jakobsen
 *
 * Last Changed By:  $Author: sse $
 * Revision:         $Revision: 7382 $
 * Last Changed:     $Date: 2006-03-07 14:00:15 +0200 (Вв, 07 Бер 2006) $
 *
 ****************************************************************************/

#ifndef _UART_BUF_IO_H_
#define _UART_BUF_IO_H_
#include "ZWaveFlash.h"
/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/


/****************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                       */
/****************************************************************************/

struct SIOFlagReg_s
{
  BYTE txActive : 1;
  BYTE rxActive : 1;
  BYTE Timeout  : 1;
  BYTE AckNakNeeded : 1;
  BYTE dummy    : 4;
};

/****************************************************************************/
/*                              EXPORTED DATA                               */
/****************************************************************************/

extern volatile struct SIOFlagReg_s SIOReg;



#define	UART_CONTROL_REG	UCSR0B
#define	ENABLE_TRANSMITTER_BIT	TXEN0
#define	ENABLE_RECEIVER_BIT	RXEN0
#define	UART_STATUS_REG	UCSR0A
#define	TRANSMIT_COMPLETE_BIT	TXC0
#define	TRANSMIT_READY_BIT	  UDRE0
#define	RECEIVE_COMPLETE_BIT	RXC0

#define	UART_DATA_REG	UDR0


/****************************************************************************/
/*                           EXPORTED FUNCTIONS                             */
/****************************************************************************/
BYTE SerialCheck(void);
BYTE SerialGetByte(void);
void SerialPutByte(BYTE ch);
void InitSerialIf(void);
void SerialBackgroundHandler(void);
void ResetSerialTimer(void);
WORD GetSerialTimer(void);


void             /*RET Nothing */
UART_Init(BYTE uart_no,WORD baudRate);   /*IN  Baud Rate / 100 (e.g. 96, 1152 (9600, 115200)) */
/*===============================   UART_RecStatus   ========================
**  Read the UART receive data status
**
**    Side effects:
**
**--------------------------------------------------------------------------*/
BYTE                    /*RET TRUE if data received */
UART_RecStatus(volatile BYTE *rx_status, BYTE rx_mask);    /*IN  Nothing */

/*===============================   UART_RecByte   ==========================
**  Receive a byte over the UART
**
**    Side effects: Wait until data received
**
**--------------------------------------------------------------------------*/
BYTE                  /*RET received data */
UART_RecByte(volatile BYTE *rx_status,volatile BYTE *uart_data, BYTE rx_mask);    /*IN  Nothing */

/*===============================   UART_SendStatus   ========================
**  Read the UART send data status
**
**    Side effects:
**
**--------------------------------------------------------------------------*/
extern	BYTE            /*RET TRUE if transmitter busy */
UART_SendStatus(volatile BYTE *tx_status, BYTE tx_mask);   /*IN  Nothing */
/*===============================   UART_SendByte   =========================
**  Transmit a byte over the UART.
**
**    Side effects: Destroy scratch registers
**                  Wait until data register is free.
**
**--------------------------------------------------------------------------*/
void        /*RET Nothing */
UART_SendByte(BYTE bData, volatile BYTE *tx_status, volatile BYTE *uart_data ,BYTE tx_c, BYTE tx_rdy);  /*IN data to send */

#endif /* _UART_BUF_IO_H_ */
