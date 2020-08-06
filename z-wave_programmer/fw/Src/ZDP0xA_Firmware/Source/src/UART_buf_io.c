/******************************  UART_BUF_IO.C  ****************************
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
 * Last Changed By:  $Author: vvi $
 * Revision:         $Revision: 10051 $
 * Last Changed:     $Date: 2008-03-13 17:58:39 +0200 (Чт, 13 Бер 2008) $
 *
 ****************************************************************************/

/****************************************************************************/
/*                              INCLUDE FILES                               */
/****************************************************************************/
#include "UART_buf_io.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include  "device_def.h"

/****************************************************************************/
/*                      PRIVATE TYPES and DEFINITIONS                       */
/****************************************************************************/
//123 UART_RX_BUFSIZE
#define UART_RX_BUFSIZE 32 //32
#define UART_TX_BUFSIZE 128


#define UART_rx_vect USART0_RXC_vect
#define UART_tx_vect USART0_TXC_vect

/****************************************************************************/
/*                              PRIVATE DATA                                */
/****************************************************************************/
volatile WORD tickTime = 0;

/* Receive and transmit buffer */

volatile BYTE rxQueue[UART_RX_BUFSIZE];
volatile BYTE txQueue[UART_TX_BUFSIZE];

volatile WORD rxInPtr, rxOutPtr;
volatile WORD rxCount;

volatile WORD txInPtr, txOutPtr;
volatile WORD txCount;



/****************************************************************************/
/*                              EXPORTED DATA                               */
/****************************************************************************/

volatile struct SIOFlagReg_s SIOReg;


/****************************************************************************/
/*                            PRIVATE FUNCTIONS                             */
/****************************************************************************/


/****************************************************************************/
/*                           EXPORTED FUNCTIONS                             */
/****************************************************************************/


ISR(TIMER1_COMPA_vect)
{
	tickTime++;
}




/*============================   UART_RX_interrupt   =========================
**
**    INTERRUPT HANDLER -- NOT TO BE CALLED DIRECTLY
**
**    Handles reception of bytes through the UART. The received bytes
**    are placed in the rxQueue array. If buffer is full the just received
**    byte is dropped.
**
**--------------------------------------------------------------------------*/
ISR(USART0_RX_vect)
{
  if (rxCount < UART_RX_BUFSIZE)
  {
   /* Put the received byte in the RX queue */
    rxQueue[rxInPtr++] = UDR0;
    if (rxInPtr >= UART_RX_BUFSIZE)
    {
      rxInPtr = 0;
    }
    rxCount++;
  }
  else
  {
    /* Read UDR and loss the data. Without we will end up with continuously interrupts until */
    /* UDR is read causing a tremendous slowdown of the system. Can happen if protocol is busy */
    /* with EEPROM writing/reading while receiving data from PC - for example after LearnNodeState */
    /* callback is called during removal of switch */
    BYTE b = UDR0;
    b++;
  }
}

/*============================   UART_TX_interrupt   =========================
**
**    INTERRUPT HANDLER -- NOT TO BE CALLED DIRECTLY
**
**    Handles the transmission of bytes through the UART. The interrupt
**    is fired when the UART is finished sending a BYTE and UDR is empty
**
**--------------------------------------------------------------------------*/

ISR(USART0_TX_vect)
{
  if (txCount > 0)  /* Anything in buffer ? */
  {

    UDR0 = txQueue[txOutPtr++];
    txCount--;
    if (txOutPtr >= UART_TX_BUFSIZE)
    {
      txOutPtr = 0;
    }
  }
  else
  {
    SIOReg.txActive = 0; /* Tell the world we're ready for more */
   
  }
  ResetSerialTimer();
}

/*===============================   SerialCheck   ============================
**
**    Checks if the receive queue contains unread data.
**
**--------------------------------------------------------------------------*/
BYTE                  /*RET  non-zero if data is ready in the receive queue */
SerialCheck(void)     /*IN  Nothing     */
{
  return rxCount;
}

/*=============================   SerialGetByte   ============================
**
**    Read a byte from the receive queue. Don't call this without checking
**    the return value from SerialCheck()!
**
**--------------------------------------------------------------------------*/
BYTE                    /*RET   The read byte  */
SerialGetByte(void)     /*IN    Nothing */
{
  BYTE retVal;

  retVal = rxQueue[rxOutPtr++];


  if (rxCount > 0)  // Make sure...
  {
    cli();  	
    rxCount--;
    sei();    
  }

  if (rxOutPtr >= UART_RX_BUFSIZE)
  {
    rxOutPtr = 0;
  }
  return retVal;
}

/*=============================   SerialPutByte   ============================
**
**   Puts a byte into the transmit queue, if transmit queue is empty then
**   the BYTE is send immediately. If transmit queue is full the function
**   waits until buffer is ready for another BYTE.
**
**--------------------------------------------------------------------------*/
void                        /*RET  Nothing */
SerialPutByte(
BYTE val)                   /*IN   Byte value to transmit */
{
  while (txCount >= UART_TX_BUFSIZE);
  if (SIOReg.txActive == 1)
  {
    txQueue[txInPtr++] = val;
    cli();
    txCount++;
    sei();
  }
  else
  {
    UART_SendByte(val,&UART_STATUS_REG,&UART_DATA_REG,TRANSMIT_COMPLETE_BIT,TRANSMIT_READY_BIT);   // Send the byte
    SIOReg.txActive = 1;  // Interrupt clears flag when done
  }
  if (txInPtr >= UART_TX_BUFSIZE)
  {
    txInPtr = 0;
  }
}

/*===============================   ResetSerialTimer   =======================
**    Reset serial timer
**
**--------------------------------------------------------------------------*/
void                  /*RET Nothing */
ResetSerialTimer()    /*IN  Nothing */
{
	cli();
  tickTime = 0;
  sei();
}

/*================================   GetSerialTimer   ========================
**    Get serial timer
**
**--------------------------------------------------------------------------*/
WORD                  /* RET  Timer value (x*10 ms)  */
GetSerialTimer()      /* IN   Nothing */
{
	return tickTime;
}

/*================================   InitSerialIf   =========================
**
**  Initialize the buffered receive/transmit system
**
**--------------------------------------------------------------------------*/
void                    /*RET Nothing */
InitSerialIf(void)
{
	cli();
	/*enable timer interrupt to timeout every 10 ms*/
	
  OCR1A = 0x2400; /*10 ms using 1 MHz*/ 
  TCNT1 = 0x0000; 
  TCCR1B = ((1<<WGM12) | (1<<CS11)); /* enable the timer1 in output compare match mode. and make the clk divider clk/8*/
  TIMSK |= (1<<OCIE1A);  /* enable interrupt*/
  tickTime = 0;
  UART_Init(0,1152);
  /*enable the RX and TX interrupt.*/
  UCSR0B |= (1<<RXCIE0)|(1<<TXCIE0);
  UCSR0A &= ~((1<<RXC0)|(1<<TXC0));
	
  rxInPtr = rxOutPtr = txInPtr = txOutPtr = rxCount = txCount = 0;
  *((BYTE*)&SIOReg) = 0;
  sei();
}



/*===============================   UART_Init   =============================
**  Initializes the MCU's integrated UART.
**  Enables transmit and receive, selects 8 databits and
**  sets the specified baudrate.
**
**
**--------------------------------------------------------------------------*/
void /*RET Nothing */
UART_Init(BYTE uart_no, WORD baudRate) /*IN  Baud Rate / 100 (e.g. 96, 1152 (9600, 115200)) */
{
  WORD x;
  /* calculate baudrate register value */
  x = (WORD)(F_CPU/1600);
  x /= baudRate;
  x--;

  if (uart_no)
  {
    /* setup baud rate */
    UBRR1H = (BYTE)0;
    UBRR1L = (BYTE)x;

    /* enable Tx and Rx, 8 databit, 2 stop bits */
    UCSR1B |= (1<<TXEN1)|(1<<RXEN1);
    UCSR1C |= (1<<UCSZ11)|(1<<UCSZ10);
  }
  else
  {
    /* setup baud rate */
    UBRR0H = (BYTE)0;
    UBRR0L = (BYTE)x;

    /* enable Tx and Rx, 8 databit, 2 stop bits */
    UCSR0B |= (1<<TXEN0)|(1<<RXEN0);
		UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00);
  }
}

/*===============================   UART_RecStatus   ========================
**  Read the UART receive data status
**
**    Side effects:
**
**--------------------------------------------------------------------------*/
BYTE                    /*RET TRUE if data received */
UART_RecStatus(volatile BYTE *rx_status, BYTE rx_mask)    /*IN  Nothing */
{
  if (*rx_status & (1<<rx_mask))
    return(TRUE);
  else
    return(FALSE);
}

/*===============================   UART_RecByte   ==========================
**  Receive a byte over the UART
**
**    Side effects: Wait until data received
**
**--------------------------------------------------------------------------*/
BYTE                  /*RET received data */
UART_RecByte(volatile BYTE *rx_status,volatile BYTE *uart_data, BYTE rx_mask)    /*IN  Nothing */
{
  while (!(*rx_status & (1<<rx_mask)));
  return(*uart_data);

}

/*===============================   UART_SendStatus   ========================
**  Read the UART send data status
**
**    Side effects:
**
**--------------------------------------------------------------------------*/
extern	BYTE            /*RET TRUE if transmitter busy */
UART_SendStatus(volatile BYTE *tx_status, BYTE tx_mask)   /*IN  Nothing */
{
  if (*tx_status & (1<<tx_mask))
    return(FALSE);
  else
    return(TRUE);
}
/*===============================   UART_SendByte   =========================
**  Transmit a byte over the UART.
**
**    Side effects: Destroy scratch registers
**                  Wait until data register is free.
**
**--------------------------------------------------------------------------*/
void        /*RET Nothing */
UART_SendByte(BYTE bData, volatile BYTE *tx_status, volatile BYTE *uart_data ,BYTE tx_c, BYTE tx_rdy)  /*IN data to send */
{
  while (!(*tx_status & (1<<tx_rdy)));
  *tx_status |= (1<<tx_c); /* clear the send complete flag */
  *uart_data = bData;
}
