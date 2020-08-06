/*******************************  mtp.c  **************************************
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
 *              Copyright (c) 2009
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
 * Description: Source code of the ZW040x Execute out of SRAM application,
 *              which implements the ZW040x MTP memory interface.
 *
 * Author:      Valeriy Vyshnyak
 *
 * Last Changed By:  $Author: vvi $
 * Revision:         $Revision: 0000 $
 * Last Changed:     $Date: Nov 17, 2009 1:09:58 PM $
 *
 ****************************************************************************/


#include "mtp.h"

sfr P0        = 0x80;
sfr P1        = 0x90;
sfr P2        = 0xA0;
sfr P3        = 0xB0;
sfr P0DIR     = 0xFD;
sfr P1DIR     = 0xC9;
sfr P2DIR     = 0xCA;
sfr P3DIR     = 0xCB;
sfr UARTCON   = 0xEC;
sfr UARTSTAT  = 0xED;
sfr UART0BRL  = 0xAF;
sfr UART0BUF  = 0xEF;
sfr WDTCON    = 0xE4;
sfr RSETVEC   = 0xE6;
sfr SFRPAGE   = 0xFF;

sfr MTPCON1             = 0xC4;
#define MTPCON1_PAGE    SFRPAGE=0x01
#define MTPCON1_ACTIVE  0x80
#define MTPCON1_STATUS  0x40
#define MTPCON1_IE      0x20
#define MTPCON1_INT     0x10
#define MTPCON1_SS      0x08
#define MTPCON1_WR      0x04
#define MTPCON1_CTRL    0x03
#define MTPCON1_CTRL_SFR_REG      0x00
#define MTPCON1_CTRL_SFR_REG_NVM  0x01
#define MTPCON1_CTRL_REG_NVM      0x02
#define MTPCON1_CTRL_SFR_NVM      0x03

sfr MTPCON2             = 0xC7;
#define MTPCON2_PAGE    SFRPAGE=0x01

sfr MTPDATA             = 0xC6;
#define MTPDATA_PAGE    SFRPAGE=0x01

sfr MTPADR              = 0xC5;
#define MTPADR_PAGE     SFRPAGE=0x01


#define P0_PAGE 0==0/* ANY */
#define P1_PAGE 0==0/* ANY */
#define P2_PAGE 0==0/* ANY */
#define P3_PAGE 0==0/* ANY */
#define P0DIR_PAGE SFRPAGE=0x01
#define P1DIR_PAGE SFRPAGE=0x01
#define P2DIR_PAGE SFRPAGE=0x01
#define P3DIR_PAGE SFRPAGE=0x01
#define UARTCON_PAGE 0==0/* ANY */
#define UARTSTAT_PAGE 0==0/* ANY */
#define UART1BRL_PAGE SFRPAGE=0x02
#define UART0BRL_PAGE SFRPAGE=0x02
#define UART1BUF_PAGE 0==0/* ANY */
#define UART0BUF_PAGE 0==0/* ANY */
#define WDTCON_PAGE SFRPAGE=0x02
#define RSETVEC_PAGE SFRPAGE=0x02

#define SFR_WR(r,v)             (r##_PAGE, (r)=(v))
#define SFR_RD(r)               (r##_PAGE, (r))
#define SFR_OR(r,v)             (r##_PAGE, (r = ((r)|(v))))
#define SFR_AND(r,v)            (r##_PAGE, (r = ((r)&(v))))
#define SFR_TEST(r,mask)        (r##_PAGE, ((r)&(mask)))
#define SFR_BSET(r,mask)        (r##_PAGE, (r = ((r)|(mask))))
#define SFR_BCLR(r,mask)        (r##_PAGE, (r = ((r)&(~(mask)))))
#define SFR_BCOPY(r,mask,value) (r##_PAGE, (r = (((r)&(~(mask))) | ((value)&(mask)))))
#define SFR_ISSET(r,mask)       (r##_PAGE, (((r)&(mask)) != 0))
#define SFR_ISCLR(r,mask)       (r##_PAGE, (((r)&(mask)) == 0))


// ********************************************************************************************************************************

#define UARTCON_CTX0        0x08
#define UARTCON_REN0        0x04
#define UARTCON_BRH0        0x03
#define UARTCON_BRH0_115K2  0x00
#define UART0BRL_BRL0_115K2 0x44
#define UART1BRL_BRL1_115K2 0x44

#define UARTSTAT_RI0        0x04
#define UARTSTAT_TI0        0x02
#define UARTSTAT_TA0        0x01

#define RSETVEC_PORTKEEP    0x40

#define WDTCON_NOTIFY       0x04
#define WDTCON_EN           0x02
#define WDTCON_KICK         0x01

#define OPEN_IOS {SFR_AND(RSETVEC,~(RSETVEC_PORTKEEP));}

#define UART0_BAUDRATE_115K2 {SFR_AND(UARTCON,~UARTCON_BRH0);SFR_OR(UARTCON,UARTCON_BRH0_115K2); SFR_WR(UART0BRL,UART0BRL_BRL0_115K2);}

#define UART0_READ_RX_BUFFER UART0BUF
#define UART0_WRITE_TX_BUFFER(TX_DATA) {UART0BUF = TX_DATA;}
#define UART0_RX_FLAG (UARTSTAT & UARTSTAT_RI0)  // Returns non-zero value if RX flag is set
#define UART0_TX_FLAG (UARTSTAT & UARTSTAT_TI0)  // Returns non-zero value if TX flag is set
#define UART0_CLEAR_RX_FLAG {UARTSTAT = UARTSTAT_RI0;}
#define UART0_CLEAR_TX_FLAG {UARTSTAT = UARTSTAT_TI0;}
#define UART0_ENABLE_IO {UARTCON |= UARTCON_CTX0;}
#define UART0_DISABLE_IO {UARTCON &= ~UARTCON_CTX0;}
#define UART0_ENABLE_RX {UARTCON |= UARTCON_REN0;}
#define UART0_DISABLE_RX {UARTCON &= ~UARTCON_REN0;}
#define WATCHDOG_DISABLE {WDTCON_PAGE; WDTCON |= WDTCON_NOTIFY ;WDTCON &= ~(WDTCON_NOTIFY|WDTCON_EN);}

// ********************************************************************************************************************************







typedef unsigned char BYTE;



/* Software Slave SPI definitions */
#define SSPI_PORT       P2
#define SSPI_PORT_PAGE  P2_PAGE
#define SSPI_DIR        P2DIR
#define SSPI_DIR_PAGE   P2DIR_PAGE
#define SSPI_MOSI       0x04
#define SSPI_MISO       0x08
#define SSPI_SCK        0x10

/* */
BYTE MtpLastAddr = 0;     //Last accessed addres in MTP memory.




/* Software Slave SPI implementation */
void SSPI_Init(void)
{
  SFR_BCLR(SSPI_DIR, SSPI_MISO);             // Set as outputs
  SFR_BSET(SSPI_DIR, SSPI_MOSI | SSPI_SCK);  // Set as inputs
}

BYTE SSPI_Exchange(BYTE value)
{
  BYTE res = 0;
  BYTE i = 8;
  do
  {
    //wait falling edge of the SCK:
    while (SFR_ISSET(SSPI_PORT, SSPI_SCK));

    // write the bit value to the MISO (most significant bits first (MSB))
    if (value & 0x80)
    {
      SFR_BSET(SSPI_PORT, SSPI_MISO);
    }
    else
    {
      SFR_BCLR(SSPI_PORT, SSPI_MISO);
    }
    value <<= 1;

    // wait rising edge of the SCK:
    while (SFR_ISCLR(SSPI_PORT, SSPI_SCK));

    // read the bit value on the MOSI (most significant bits first (MSB))
    res <<= 1;
    if (SFR_ISSET(SSPI_PORT, SSPI_MOSI))
    {
      res |= 0x01;
    }

  } while (--i != 0);

  return res;
}

/* MTP memory interface implementation */
void MTP_Init()
{
  SFR_BCLR(MTPCON1, MTPCON1_IE);
  SFR_BSET(MTPCON1, MTPCON1_SS);
  while (SFR_ISSET(MTPCON1, MTPCON1_ACTIVE));
}

BYTE MTP_ReadByte(BYTE addr)
{
  if(SFR_ISCLR(MTPCON1, MTPCON1_ACTIVE))
  {
    SFR_WR(MTPADR, addr);
    SFR_BCOPY(MTPCON1, MTPCON1_CTRL, MTPCON1_CTRL_SFR_REG);
    SFR_BCLR(MTPCON1, MTPCON1_WR);
    SFR_BSET(MTPCON1, MTPCON1_ACTIVE);
    while (SFR_ISSET(MTPCON1, MTPCON1_ACTIVE));
    return SFR_RD(MTPDATA);
  }
  return 0x56;
}

void MTP_WriteByte(BYTE addr, BYTE value)
{
  if(SFR_ISCLR(MTPCON1, MTPCON1_ACTIVE))
  {
    SFR_WR(MTPADR, addr);
    SFR_WR(MTPDATA, value);
    SFR_BCOPY(MTPCON1, MTPCON1_CTRL, MTPCON1_CTRL_SFR_REG);
    SFR_BSET(MTPCON1, MTPCON1_WR | MTPCON1_ACTIVE);
    while (SFR_ISSET(MTPCON1, MTPCON1_ACTIVE));
  }
}

void MTP_Recall(void)
{
  if(SFR_ISCLR(MTPCON1, MTPCON1_ACTIVE))
  {
    SFR_BCOPY(MTPCON1, MTPCON1_CTRL, MTPCON1_CTRL_REG_NVM);
    SFR_BCLR(MTPCON1, MTPCON1_WR);
    SFR_BSET(MTPCON1, MTPCON1_ACTIVE);
    while (SFR_ISSET(MTPCON1, MTPCON1_ACTIVE));
  }
}

void MTP_Program(void)
{
  if(SFR_ISCLR(MTPCON1, MTPCON1_ACTIVE))
  {
    SFR_BCOPY(MTPCON1, MTPCON1_CTRL, MTPCON1_CTRL_REG_NVM);
    SFR_BSET(MTPCON1, MTPCON1_WR | MTPCON1_ACTIVE);
    while (SFR_ISSET(MTPCON1, MTPCON1_ACTIVE));
  }
}

void MTP_Fill(BYTE value)
{
  BYTE i;
  if(SFR_ISCLR(MTPCON1, MTPCON1_ACTIVE))
  {
    for (i = 0; i < MTP_SIZE; i++)
    {
      MTP_WriteByte(i, value);
    }
    MTP_Program();
  }
}







//void DisplayString(BYTE *string)
//{
//  while (*string)
//  {
//    while (UARTSTAT & UARTSTAT_TA0);
//    UART0_WRITE_TX_BUFFER(*string++);
//  }
//}
//
//void DisplayByte(BYTE value)
//{
//  BYTE out;
//
//  while (UARTSTAT & UARTSTAT_TA0);
//  out = (value >> 4) + '0';
//  if(out > '9')
//    out += 'A' - '9' - 1;
//  UART0_WRITE_TX_BUFFER(out);
//
//  while (UARTSTAT & UARTSTAT_TA0);
//  out = (value & 0x0f) + '0';
//  if(out > '9')
//    out += 'A' - '9' - 1;
//  UART0_WRITE_TX_BUFFER(out);
//}

void main()
{
  BYTE cmd, byte2, byte3, byte4;

  WATCHDOG_DISABLE;
  OPEN_IOS;  // Enable IO ports
  SSPI_Init();
  MTP_Init();

//UART0_BAUDRATE_115K2; // Set baudrate to 115.2 kbaud
//UART0_ENABLE_IO;
//UART0_ENABLE_RX;
//DisplayString("\r\nMTP Handler in ZW040x\r\n");

  while(1)
  {
    cmd = SSPI_Exchange(0);

    switch(cmd)
    {
      case READ_MTP_REG_1_BYTE:
      {
        byte2 = SSPI_Exchange(READ_MTP_REG_2_BYTE_OUT);
        byte3 = SSPI_Exchange(READ_MTP_REG_3_BYTE_OUT);
        byte4 = MTP_ReadByte(byte3);
        /*byte4 = */SSPI_Exchange(byte4);
        MtpLastAddr = byte3 + 1;
        break;
      }

      case CONTINUE_READ_MTP_REG_1_BYTE:
      {
        byte2 = MTP_ReadByte(MtpLastAddr++);
        byte3 = MTP_ReadByte(MtpLastAddr++);
        byte4 = MTP_ReadByte(MtpLastAddr++);
        /*byte2 = */SSPI_Exchange(byte2);
        /*byte3 = */SSPI_Exchange(byte3);
        /*byte4 = */SSPI_Exchange(byte4);
        break;
      }

      case WRITE_MTP_REG_1_BYTE:
      {
        byte2 = SSPI_Exchange(WRITE_MTP_REG_2_BYTE_OUT);
        byte3 = SSPI_Exchange(WRITE_MTP_REG_3_BYTE_OUT);
        byte4 = SSPI_Exchange(byte3);
        MTP_WriteByte(byte3, byte4);
        MtpLastAddr = byte3 + 1;
        break;
      }

      case CONTINUE_WRITE_MTP_REG_1_BYTE:
      {
        byte2 = SSPI_Exchange(CONTINUE_WRITE_MTP_REG_2_BYTE_OUT);
        byte3 = SSPI_Exchange(byte2);
        byte4 = SSPI_Exchange(byte3);
        MTP_WriteByte(MtpLastAddr++, byte2);
        MTP_WriteByte(MtpLastAddr++, byte3);
        MTP_WriteByte(MtpLastAddr++, byte4);
        break;
      }

      case READ_MTP_1_BYTE:
      {
        byte2 = SSPI_Exchange(READ_MTP_2_BYTE_OUT);
        byte3 = SSPI_Exchange(READ_MTP_3_BYTE_OUT);
        byte4 = SSPI_Exchange(READ_MTP_4_BYTE_OUT);
        MTP_Recall();
        break;
      }

      case WRITE_MTP_1_BYTE:
      {
        byte2 = SSPI_Exchange(WRITE_MTP_2_BYTE_OUT);
        byte3 = SSPI_Exchange(WRITE_MTP_3_BYTE_OUT);
        byte4 = SSPI_Exchange(WRITE_MTP_4_BYTE_OUT);
        MTP_Program();
        break;
      }

      case FILL_MTP_1_BYTE:
      {
        byte2 = SSPI_Exchange(FILL_MTP_2_BYTE_OUT);
        byte3 = SSPI_Exchange(FILL_MTP_3_BYTE_OUT);
        byte4 = SSPI_Exchange(FILL_MTP_4_BYTE_OUT);
        MTP_Fill(byte4);
        break;
      }
    }
//DisplayByte(cmd);DisplayByte(byte2);DisplayByte(byte3);DisplayByte(byte4);DisplayString("\r\n");
//byte2 = byte3 = byte4 = 0;
  }
}


