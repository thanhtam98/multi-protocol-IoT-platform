/************************  ZWaveFlash.c  *******************************
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
 * Description: Main source code of the Z-Wave Programmer Firmware. Contains
 *              the implementation of all programmer commands handlers and
 *              Z-Wave chips programming algorithms.
 *
 * Author:      Valeriy Vyshnyak
 *
 * Last Changed By:  $Author:  $
 * Revision:         $Revision: 0000 $
 * Last Changed:     $Date: 2012-10-24 $
 *
 ****************************************************************************/
#define BOOT_LOADER
// #define ZWAVE_PROGRAMMER
#include "ZWaveFlash.h"
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#ifdef BOOT_LOADER
#include <avr/boot.h>
#endif
#include <avr/pgmspace.h>
#include "device_def.h"

#include "conhandle.h"
#include "ports.h"
#include "commands.h"
#include "lowlevel.h"

#ifdef ZWAVE_PROGRAMMER
#include "nvm.h"
#include "ATMega_spi.h"
#include "mtp.h"
#include "mtp_ZW040x.h"
#endif

#define ZWAVE_PROGRAMMER_VERSION  1
#define ZWAVE_PROGRAMMER_REVISION 32

//123      ZWAVE_PROGRAMMER_VERSION_ID
volatile const char ZWAVE_PROGRAMMER_VERSION_ID[] PROGMEM = {'Z','W','A','V','E','_','P','R','O','G','R','A','M','M','E','R','_','V','E','R','S','I','O','N','_','V','V','I','_','4','5','2','3','@','7','7','4','0','=',ZWAVE_PROGRAMMER_VERSION,ZWAVE_PROGRAMMER_REVISION};


/* definitions for device recognition */
#define SIG_BYTE_1 0xF0
#define SIG_BYTE_2 0x02


/*Constant used for ZW0x0x flash programming*/
#define FLASH_ERASE_TIME_ZW010x       500
#define FLASH_ERASE_TIME_ZW020x       1000
#define LOCK_MASK              0x1F
#define MSB_MASK               0x80
#define BITS_IN_BYTE           8
#define C_VALUE                30
#define PAGE_WRITE_DELAY       8    /*Max 5.4ms*/

#define MAX_SYNC               32
#define NO_OF_DELAY_CLK        5

#define DONT_CARE              0x00

#define PROG_EN_TPE_DURATION_US   4096
#define PROG_EN_1_BYTE            0xAC
#define PROG_EN_2_BYTE            0x53
#define PROG_EN_3_BYTE            0xAA
#define PROG_EN_4_BYTE            0x55

#define WRITE_CYC_TIME_1_BYTE  0xAC
#define WRITE_CYC_TIME_2_BYTE  0x5D

#define CHIP_ERASE_1_BYTE      0xAC
#define CHIP_ERASE_2_BYTE      0x80

#define ODD_BYTE               0x08
#define EVEN_BYTE              0x00

#define LOAD_PROG_MEM_1_BYTE   0x40

#define WRITE_PROG_1_BYTE      0x4C

#define READ_PROG_MEM_1_BYTE   0x20

//Read Lock Bits '1' = unprogrammed. '0' = programmed
#define FLASH_READ_LOCK_1_BYTE       0x58
#define FLASH_READ_LOCK_2_BYTE       0x00
#define FLASH_READ_LOCK_3_BYTE       0x00
#define FLASH_READ_LOCK_4_BYTE       0x00

//Write Lock Bits. Set bits 4-0 to �0� to set.
#define FLASH_WRITE_LOCK_1_BYTE      0xAC
#define FLASH_WRITE_LOCK_2_BYTE      0xE0
#define FLASH_WRITE_LOCK_3_BYTE      0x00
#define FLASH_WRITE_LOCK_4_BYTE      0x00
#define FLASH_WRITE_LOCK_BITS_MASK   0x1F
#define FLASH_WRITE_LOCK_BITS_TWL_DURATION_US_ZW010x          150
#define FLASH_WRITE_LOCK_BITS_TWL_DURATION_US_ZW020x_ZW030x   62
#define FLASH_WRITE_LOCK_BITS_TWL_DURATION_US_ZW050x 6

#define BLANK_VALUE            0xFF

/* ZW0x0x read signature command definition */
#define READ_SIG_1_BYTE        0x30               /* read signature command 1st byte */
#define READ_SIG_4_BYTE        0x00               /* read signature command 4th byte */
#define SIG_BYTE_MANUFACTURER_ID1         0       /* address of maufacturer ID byte 1 */
#define SIG_BYTE_MANUFACTURER_ID2         1       /* address of maufacturer ID byte 2 */
#define SIG_BYTE_MANUFACTURER_ID3         2       /* address of maufacturer ID byte 3 */
#define SIG_BYTE_MANUFACTURER_ID4         3       /* address of maufacturer ID byte 4 */
#define SIG_BYTE_MANUFACTURER_ID5         4       /* address of maufacturer ID byte 5 */
#define SIG_BYTE_CHIP_TYPE                5       /* address of Chip Type */
#define SIG_VALUE_CHIP_TYPE_NORMAL_DEVICE 0x02    /* value of Chip Type for Normal Device*/
#define SIG_VALUE_CHIP_TYPE_MODEM_DEVICE  0x03    /* value of Chip Tyoe for Modem/Z-WaveAV device */
#define SIG_BYTE_CHIP_REVISION            6       /* addres of chip revision number */

#define PROGRAM_MEM_ERASE_1_BYTE  0xAC
#define PROGRAM_MEM_ERASE_2_BYTE  0x50

#define INFO_PAGE_WRITE_1_BYTE    0xAC
#define INFO_PAGE_WRITE_2_BYTE    0x00

#define INFO_PAGE_READ_1_BYTE     0xAC
#define INFO_PAGE_READ_2_BYTE     0x40

#define PAGE_ERASE_1_BYTE         0xAC
#define PAGE_ERASE_2_BYTE         0xC0

/*ZW040x specific commands. If some value of byte are not defined, then this is */
/* an anrgument. Read comments to find what must be sent as argument. */

/* Read OTP Command. Read data at OTP memory address Addr1(2_BYTE):Addr0(3_BYTE) */
#define READ_OTP_1_BYTE             0x10
#define READ_OTP_DURATION_US        2     // wait here specified time before sending next bytes
#define READ_OTP_4_BYTE             0x00

/* Read SRAM. Read data at SRAM address Addr1(2_BYTE):Addr0(3_BYTE) */
#define READ_SRAM_1_BYTE            0x06
#define READ_SRAM_DURATION_US       2     // wait here specified time before sending next bytes
#define READ_SRAM_4_BYTE            0x00

/* Continue Read OTP or SRAM. The Command can follow a �Read SRAM� or a �Read OTP� */
/* command. The command will read the next three memory locations from the SRAM or */
/* OTP, depending on the preceding command.*/
#define CONTINUE_READ_1_BYTE        0xA0
#define CONTINUE_READ_DURATION_US   2     // wait here specified time before sending next bytes
#define CONTINUE_READ_2_BYTE        0x00
#define CONTINUE_READ_3_BYTE        0x00
#define CONTINUE_READ_4_BYTE        0x00

/* Write SRAM byte. Write data Data to SRAM page buffer at address Addr1(2_BYTE):Addr0(3_BYTE) */
#define WRITE_SRAM_1_BYTE           0x04
#define WRITE_SRAM_4_BYTE           0x00
#define WRITE_SRAM_DURATION_US      2     // wait here specified time before sending next bytes

/* Continue Write SRAM. The command can follow a �Write SRAM� command and it */
/* writes three bytes to the next memory locations in SRAM. */
#define CONTINUE_WRITE_1_BYTE       0x80
#define CONTINUE_WRITE_2_BYTE       0x00
#define CONTINUE_WRITE_3_BYTE       0x00
#define CONTINUE_WRITE_4_BYTE       0x00
#define CONTINUE_WRITE_DURATION_US  2     // wait here specified time before sending next bytes

/* Write OTP from SRAM buffer. Write the data written to the */
/*  SRAM to the OTP Memory location starting at Addr1(2_BYTE):Addr0(3_BYTE). */
#define WRITE_OTP_1_BYTE            0x20
#define WRITE_OTP_4_BYTE            0x00
#define WRITE_OTP_TWP_DURATION_MIN_US   116    /* Minimum Time of OTP Write Byte duration*/
#define WRITE_OTP_TWP_DURATION_MAX_US   580    /* Maximum Time of OTP Write Byte duration*/

/* Read State byte. Read the status the programming logic*/
#define CHECK_STATE_1_BYTE          0x7F
#define CHECK_STATE_2_BYTE          0xFE
#define CHECK_STATE_3_BYTE          0x00
#define CHECK_STATE_4_BYTE          0x00
/* Sate bits: */
#define STATE_CRC_BUSY              (1<<0)    /*CRC busy. This bit will go high when a �Run CRC Check�*/
/*  command has been sent to the Single Chip. It will */
/*  return to low when the CRC check procedure is done. */
#define STATE_CRC_DONE              (1<<1)    /*CRC done. This bit is cleared when a �Run CRC Check� */
/*  command is issued and it will be set if the CRC check */
/*  procedure passes. */
#define STATE_CRC_FAILED            (1<<2)    /*CRC failed. This bit is cleared when a �Run CRC Check� */
/*  command is issued and it will be set if the CRC check */
/*  procedure fails. */
#define STATE_WRITE_BUSY            (1<<3)    /*Write Operation busy. This bit is high if the OTP programming */
/*  logic is busy programming the OTP*/
#define STATE_WRITE_FAILED          (1<<4)    /*Write Operation failed. This bit is cleared when a �Write OTP�*/
/*  command is issued and it will be set if the OTP write */
/*  operation fails.*/
#define STATE_CONTINUE_FEFUSED      (1<<5)    /*Cont operation refused. This bit will be set if either a */
/*  �Continue Write Operation� or a Continue Read Operation� are */
/*  refused. These operations will be refused if: */
/*  A �Continue Write Operation� is not succeeding a */
/*  �Write SRAM� or a �Continue Write Operation� */
/*  command. */
/*  A �Continue Read Operation� is not succeeding a */
/*  �Read OTP�, a �Read SRAM� or a �Continue Read */
/*  Operation� command */
#define STATE_DEV_MODE_ENABLED      (1<<6)    /*Development mode enabled. This bit is set if the */
/*  �Development Mode� has been enabled */
#define STATE_EXEC_SRAM_MODE_ENABLED (1<<7)   /*Exec SRAM mode enabled. This bit is set if the*/
/*  �Execute out of SRAM� Mode has been enabled */
/*Read the number of excessive writes, Stat1:Stat0. */
#define READ_WRITE_OTP_STATS_1_BYTE 0x7E
#define READ_WRITE_OTP_STATS_2_BYTE 0x00
#define READ_WRITE_OTP_STATS_3_BYTE 0x00
#define READ_WRITE_OTP_STATS_4_BYTE 0x00

/*Enable Dev Mode. Running this command will set the mode where the Dev SRAM is mapped into the code space*/
/* after a reset. The mode is disabled by power cycling the chip.*/
#define ENABLE_DEV_MODE_1_BYTE      0xD0
#define ENABLE_DEV_MODE_2_BYTE      0x00
#define ENABLE_DEV_MODE_3_BYTE      0x00
#define ENABLE_DEV_MODE_4_BYTE      0x00

/*Execute Out Of SRAM. Running this command will set the mode where the MCU will */
/* execute code out of SRAM after a reset. The mode is disabled by power cycling the chip.*/
#define EXECUTE_OUT_OF_SRAM_1_BYTE  0xC0
#define EXECUTE_OUT_OF_SRAM_2_BYTE  0x00
#define EXECUTE_OUT_OF_SRAM_3_BYTE  0x00
#define EXECUTE_OUT_OF_SRAM_4_BYTE  0x00

/*Set Lock Bits. Set lock bits to LockData(2_BYTE) contents*/
#define SET_LOCK_BITS_1_BYTE        0xF0
#define SET_LOCK_BITS_3_BYTE        0x00
#define SET_LOCK_BITS_4_BYTE        0x00
#define SET_LOCK_BITS_MASK          0x03 /* it's seems, that bit 2 internally used as Set Modem Bit. It can't be written by Set Lock Bits command, but it read as '1' by Read Lock Bits command if it is set by Set Modem Bits command. */
#define SET_LOCK_BITS_TWL_DURATION_US 580

/*Read Lock Bits. */
#define READ_LOCK_BITS_1_BYTE       0xF1
#define READ_LOCK_BITS_2_BYTE       0x00
#define READ_LOCK_BITS_3_BYTE       0x00
#define READ_LOCK_BITS_4_BYTE       0x00

/*Set Modem Bits. Set the bit that identifies the Chip as a �Modem� device*/
#define SET_MODEM_BITS_1_BYTE       0xFE
#define SET_MODEM_BITS_2_BYTE       0x00
#define SET_MODEM_BITS_3_BYTE       0x00
#define SET_MODEM_BITS_4_BYTE       0x00
#define SET_MODEM_BITS_TWL_DURATION_US  580

/*Run CRC Check. Run the CRC check procedure. */
#define RUN_CRC_CHECK_1_BYTE        0xC3
#define RUN_CRC_CHECK_2_BYTE        0x00
#define RUN_CRC_CHECK_3_BYTE        0x00
#define RUN_CRC_CHECK_4_BYTE        0x00
#define RUN_CRC_CHECK_TWH_DURATION_US 2100

/* get the l-value low byte from the varriable in memory: */
#define TOLOBYTE(word) (((unsigned char*)(&(word)))[0])
/* get the l-value high byte from the varriable in memory: */
#define TOHIBYTE(word) (((unsigned char*)(&(word)))[1])
/* get the r-value low byte from the variable or constant: */
#define LOBYTE(word) (((word)>>0)&0xff)
/* get the r-value high byte from the variable or constant: */
#define HIBYTE(word) (((word)>>8)&0xff)
/*---*/
#define SECTOR_SIZE_ZW050x     2048                     //123
#define PACKET_SIZE_ZW5XX      1024

#define PAGE_SIZE_ZW040x       256      /*ZW040x do not have paged memories (not SRAM nor OTP), but we paged it logically because:*/
/* When programming the OTP a range of data is written to the SRAM and thereafter programmed into the OTP.
This procedure is repeated until all data has been programmed in to the OTP.
Each of the ranges of data MUST be written to the SRAM first by using the 'Write SRAM Byte' then followed
by �Continue Write� commands until the SRAM is filled or until there is no more data to be written.
That is, using the �Write SRAM� command will set the base address of the data range in SRAM and the
following �Continue Write� commands will set the data size.
TEREFORE - THE SIZE OF PAGE MUST EQUAL: 1 byte for 'Write SRAM Byte' command and N*3 bytes  for �Continue Write� commands,
 where N must be an INTEGER NUMBER.  256 = 1 + 85 * 3 , therefore this page size is good!  */
#define PAGE_SIZE_ZW020x       256
#define PAGE_SIZE_ZW010x       128
#define HOMEID_LENGTH  4
#define EX_EEPROM_HOME_ID  8


/*ZW050x specific commands. If some value of byte are not defined, then this is */
/* an anrgument. Read comments to find what must be sent as argument. */

/* Read Flash Command. Read data at Flash memory sector  */
#define READ_FLASH_ZW050X      0x10
#define WRITE_FLASH_ZW050X     0x20
#define WRITE_FLASH_ZW050X_BYTE_DURATION_MAX_US 6
#define SECTOR_ERASE_ZW050X    0x0B
#define CHIP_ERASE_ZW050X      0x0A
#define FLASH_ERASE_TIME_ZW050x 50


#define CAL_EN_HIGH  PIN_OFF(CAL_EN)
#define CAL_EN_LOW   PIN_ON(CAL_EN)

#define PROG_VPP_HIGH  PIN_OFF(PROG_VPP)         /*set HIGH VPP voltage (6.5V)*/
#define RESET_N_HIGH  PIN_OFF(RESET_N)
#define SCK_HIGH      PIN_OFF(SCK)
#define MOSI_HIGH     PIN_OFF(MOSI)
#define SS_N_HIGH     PIN_OFF(SS_N)
#define IO9_HIGH      PIN_OFF(IO9)


#define PROG_VPP_LOW  PIN_ON(PROG_VPP)          /*set LOW VPP voltage (3.3V)*/
#define RESET_N_LOW  PIN_ON(RESET_N)
#define SCK_LOW      PIN_ON(SCK)
#define MOSI_LOW     PIN_ON(MOSI)
#define SS_N_LOW     PIN_ON(SS_N)
#define IO9_LOW     PIN_ON(IO9)

#define  PASS_LED   0
#define  ERR_LED    1
#define  BUSY_LED   2

#define __NOPS__   asm volatile ("nop\n\t" "nop\n\t" "nop\n\t"::)

#define  ZW010x   0x01
#define  ZW020x   0x02
#define  ZW030x   0x03
#define  ZW040x   0x04                          /*ZW040x Id in PC to Programmer interface*/
#define  ZW050x   0x05                          /*ZW050x Id in PC to Programmer interface*/
#define  ATM128   0x80

#define AUTOPROG_0 0x02
#define AUTOPROG_1 0x04
#define RBAP_BYTE_INDEX 8


//123   BUF_SIZE
//#define BUF_SIZE 1024 + 4*256 + 18 //266
#define BUF_SIZE (1024 + 18) //266

//static
BYTE compl_workbuf[BUF_SIZE];    /* Used for frames send to remote side. */


extern volatile WORD tickTime;

static BYTE seqNo = 0;
static BYTE requestSeqNo = 0;
#ifdef ZWAVE_PROGRAMMER
static BYTE buttonState = 0;
static BYTE buttonPressed = 0;

static BYTE mtpIfEn = FALSE;
#define MTP_HANDLER_OLD_SRAM_SIZE    ( (((DWORD)(MTPHANDLER_LENGTH / PAGE_SIZE_ZW040x)) + 1) * PAGE_SIZE_ZW040x)   //we must ensure, that we have enough buffer for all SRAM pages of MTP Handler
//BYTE MtpHandlerOldSRAM[MTP_HANDLER_OLD_SRAM_SIZE];
//123
//BYTE MtpHandlerOldSRAM[3*256 + 128 + 32];
BYTE MtpHandlerOldSRAM[2*256];
BYTE MtpHandlerOldStateByte = 0;
#endif

unsigned char btSig[5];  /*array that hold 4 bytes random value and a checksum that should be verified before programming the ATMEL*/
#ifdef BOOT_LOADER
unsigned char BlockLoad(BYTE verify, unsigned int size, unsigned char mem, uint16_t pageNo);
void BlockRead(unsigned int size, unsigned char mem, uint16_t pageNo);
/* BLOCKSIZE should be chosen so that the following holds: BLOCKSIZE*n = PAGESIZE,  where n=1,2,3... */
#define BLOCKSIZE SPM_PAGESIZE
#define eeprom_is_ready()             bit_is_clear(EECR, EEWE)
#define my_eeprom_busy_wait()         do{}while(!eeprom_is_ready())
#endif

/* States for ApplicationPoll function */
enum
{
  stateIdle,
  stateTxSerial,
  stateFrameParse,
  stateTxNoAck
};

static BYTE state = stateIdle, retry = 0, waitCmd = 0;



#ifdef ZWAVE_PROGRAMMER
BOOL isInAutoProgMode = TRUE;
static void
Reset_N_Low(void)
{
  if(isInAutoProgMode==FALSE)
  {
    RESET_N_LOW;
  }
}
static void
Reset_N_High(void)
{
  if(isInAutoProgMode==FALSE)
  {
    RESET_N_HIGH;
  }
}
/*
  Make busy waiting loop using the 10 ms timer counter
*/
static void
DelayMs(
  WORD delayMs) /*delay in ms*/
{
  /*conver the delaty in ms to unit of 50 us each*/
  delayMs *= 20;

  while (delayMs--)
  {
    DELAY_50_US;
  }
}


/*============================   DelayUS   ===============================
** Function description
**   Make busy waiting loop in us
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static void
DelayUS(
  WORD delayUS) /*delay in us*/
{
  BYTE cnt;

  /*If required delay are less than max delay, which can be*/

  /*  generated by delay_loop, then just use delay_loop*/
  if (delayUS < DELAY_LOOP_MAX_US)
  {
    cnt = DELAY_LOOP_FROM_US(delayUS);

    if (cnt)
      delay_loop(cnt);
  }
  else
    /*If required delay are greater than max delay, which can be*/
    /*  generated by delay_loop, then we must use several calls to delay_loop*/
  {
    /*count delay with help of maximum delay_loop delay*/
    cnt = delayUS / DELAY_LOOP_MAX_US;

    while (cnt--)
      DELAY_LOOP_MAX;

    /*count remainind delay*/
    cnt = DELAY_LOOP_FROM_US(delayUS % DELAY_LOOP_MAX_US);

    if (cnt)
      delay_loop(cnt);
  }
}


//Disable the flash interface:
// Flash (OTP) interface is disabled by continuous toggling the
// reset_n signal of the ZW0x0x chip at frequency about 3.6 KHz.
// This lead to constantly reseting the ZW0x0x chip preventing him
// to enter the programming mode and also to start executing
// the firmware in ZW0x0x chip.
static void
DisableFlashInterfaceButStayInProgrammingMode(
  BYTE state)
{
  if (state)
  {
    OCR0 = 0xFA;
    TCNT0 = 0x00;
    /* setup the timer0 in fast PWM mode. and make the clk divider clk/8*/
    /*Clear OC0 on counter TOP value and set it when TCNT0 == OCR0 */
    TCCR0 = ((1 << WGM00) | (1 << WGM01) | (1 << CS01) | (1 << COM00) | (1 << COM01));

  }
  else
  {
    TCCR0 &= ~ ((1 << CS00) | (1 << CS01) | (1 << COM00) | (1 << COM01)); /* enable the timer3 in output compare match mode. and make the clk divider clk/8*/
  }
}

static BYTE
SyncDevice(
  void)
{
  BYTE receivedByte, noOfSync;
  noOfSync = 0;
  //Enable serial programming after reset has been low during TPE:
  DelayUS(PROG_EN_TPE_DURATION_US);

  do
  {
    SPI_Exchange(PROG_EN_1_BYTE);
    SPI_Exchange(PROG_EN_2_BYTE);
    receivedByte = SPI_Exchange(PROG_EN_3_BYTE);
    SPI_Exchange(PROG_EN_4_BYTE);

    if (receivedByte == PROG_EN_2_BYTE)   /* did device echo back*/
      return noOfSync;          /*yes*/
    else
      noOfSync++;             /*try again*/

    /*disbale spi ainterface and toggle the MOSI pin low high low*/
    DELAY_9_CLK;
    /* issue pulse on SCK*/
    SCK_HIGH;
    DELAY_1_CLK;
    SCK_LOW;
    DELAY_1_CLK;
    DELAY_9_CLK;
  }
  while (noOfSync < MAX_SYNC);

  return noOfSync;
}


/*===============================   ProgEnable_020x   ===============================
**  This function enable the ZW0x0x external flash programming interface
**
** return the number of sync retries
**--------------------------------------------------------------------------*/
static BYTE
ProgEnable(
  BYTE chipType)
{
//123 if ((chipType == ZW020x) || (chipType == ZW030x) || (chipType == ZW040x))
  if ((chipType == ZW020x) || (chipType == ZW030x) || (chipType == ZW040x) || (chipType == ZW050x))
  {
    PROG_VPP_LOW; // Set VPP to low value (3.3V)
    Reset_N_High(); // set HIGH to prepare for a negative pulse
    Reset_N_Low();  // set LOW to make a negative pulse
    DelayMs(200);   // has to be removed
    //Test if 200series
    return SyncDevice();
  }

  if (chipType == ZW010x)
  {
    PROG_VPP_HIGH;  /*ZW010x are not use a VPP voltage, instead of this is use a*/
    /*  logical level of this pin. Set it to log "1" to be able to set is to "0" later*/
    Reset_N_High(); /* set HIGH to prepare for a negative pulse*/
    DELAY_9_CLK;

    /*Set 100 series in programming mode*/
    Reset_N_Low();  /* set LOW to make a negative pulse*/
    DELAY_1_CLK;
    Reset_N_High();
    PROG_VPP_LOW; /* set low to allow entering in programming mode*/
    return SyncDevice();
  }

  return 0xFF;
}

/**
  Write the write cycle time to the target ASIC
**/
void
SendWriteCycleTime(
  BYTE writeTime)
{
  SPI_Exchange(WRITE_CYC_TIME_1_BYTE);
  SPI_Exchange(WRITE_CYC_TIME_2_BYTE);
  SPI_Exchange(DONT_CARE);
  SPI_Exchange(writeTime);
}

/*============================   ReadSignatureByte   ======================
** Function description
**  Read specified signature byte.
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static BYTE   /* signature byte*/
ReadSignatureByte(
  BYTE chipType,
  BYTE sigByteNum)    /*signature byte number*/
{
  SPI_Exchange(READ_SIG_1_BYTE);
  SPI_Exchange(sigByteNum);
  SPI_Exchange(sigByteNum);
  DELAY_9_CLK;
  return SPI_Exchange(READ_SIG_4_BYTE);
}

/*===================   ReadSignatureBits   ===============================
** Function description
**  This function read the sig bytes from the target ASIC
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static void
ReadSignatureBits(
  BYTE chipType,
  BYTE * sigBits)
{
  BYTE i;

  for (i = 0; i < 7 ; i++)
    sigBits[i] = ReadSignatureByte(chipType, i);
}


/*
  This function auto init the target ASIC and set the write cycle time
*/
static BYTE  /*return number of sync attemps*/
AutoInit(
  void)
{
  BYTE syncs;
  syncs = ProgEnable(ZW020x);

  if ( syncs >= MAX_SYNC)
  {
    syncs = ProgEnable(ZW010x);
  }

  return syncs;
}

/*===============================   ChipErase   ===============================
**  This function erase the entire device
**
** return none
**--------------------------------------------------------------------------*/
static BYTE    /*return true if OK else false*/
ChipErase(
  BYTE chipType,
  BYTE writeTime)
{
  WORD delay = 0;

  if (chipType == ZW050x)
  {
      SPI_Write(CHIP_ERASE_ZW050X);
      SPI_Write(DONT_CARE);
      SPI_Write(DONT_CARE);
      SPI_Write(DONT_CARE);
      DelayMs(FLASH_ERASE_TIME_ZW050x);
  }
  else if ((chipType == ZW010x) || (chipType == ZW020x) || (chipType == ZW030x))
  {
    SendWriteCycleTime(writeTime);
    SPI_Write(CHIP_ERASE_1_BYTE);
    SPI_Write(CHIP_ERASE_2_BYTE);
    SPI_Write(DONT_CARE);
    SPI_Write(DONT_CARE);

    if ((chipType == ZW020x) || (chipType == ZW030x))
    {
      delay = FLASH_ERASE_TIME_ZW020x;
    }

    if (chipType == ZW010x)
    {
      delay = FLASH_ERASE_TIME_ZW010x;
    }

    DelayMs(delay);
  }
  else
  {
//      if (chipType == ZW040x)
    return FAIL;
  }
  return SUCCESS;
}

/*================   CheckState   ========================================
** Function description
**  Read the State byte
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static BYTE    /*return the State byte*/
CheckState(
  BYTE chipType)
{
  if ((chipType == ZW040x)||(chipType == ZW050x))
  {
    SPI_Exchange(CHECK_STATE_1_BYTE);
    SPI_Exchange(CHECK_STATE_2_BYTE);
    SPI_Exchange(CHECK_STATE_3_BYTE);
    return SPI_Exchange(CHECK_STATE_4_BYTE);
  }
  else
  {
    return 1;
  }
}


/*================   ReadWriteOtpStats   ========================================
** Function description
**  Read the number of excessive writes, Stat1:Stat0.
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static WORD    /*return the number of excessive writes*/
ReadWriteOtpStats(
  BYTE chipType)
{
  WORD num;

  if (chipType != ZW040x)
    return 0;

  SPI_Exchange(READ_WRITE_OTP_STATS_1_BYTE);
  SPI_Exchange(READ_WRITE_OTP_STATS_2_BYTE);
  num = 0;
  num |= (((WORD)SPI_Exchange(READ_WRITE_OTP_STATS_3_BYTE)) << 8);
  num |= (((WORD)SPI_Exchange(READ_WRITE_OTP_STATS_4_BYTE)) << 0);
  return num;
}


/*================   IsChipReady   ========================================
** Function description
**  Check - is chip is ready.
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static BYTE    /*return the true if chip is ready, false - if not ready*/
IsChipReady(
  BYTE chipType)
{
  if ((chipType == ZW040x)||(chipType == ZW050x))
  {
    if (CheckState(chipType) & (STATE_CRC_BUSY | STATE_WRITE_BUSY))
    {
      return FALSE;
    }
  }
  return TRUE;
}

/*================   ExecuteOutOfSRAM   ==================================
** Function description
**  Running this command will set the mode where the MCU will
**  execute code out of SRAM after a reset. The mode is disabled by
**  power cycling the chip.
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static BYTE    /*return true if OK else false*/
ExecuteOutOfSRAM(
  BYTE chipType)
{
  if (chipType != ZW040x)
    return FAIL;

  if (!IsChipReady(chipType))
    return FAIL;

  SPI_Exchange(EXECUTE_OUT_OF_SRAM_1_BYTE);
  SPI_Exchange(EXECUTE_OUT_OF_SRAM_2_BYTE);
  SPI_Exchange(EXECUTE_OUT_OF_SRAM_3_BYTE);
  SPI_Exchange(EXECUTE_OUT_OF_SRAM_4_BYTE);
  DELAY_36_CLK;

  if (CheckState(chipType) & STATE_EXEC_SRAM_MODE_ENABLED)
    return SUCCESS;

  return FAIL;
}



static BYTE    /*return true if OK else false*/
ExecuteOutOfSRAM_500(
  BYTE chipType)
{
      /*Issue command*/
      SPI_Exchange(EXECUTE_OUT_OF_SRAM_1_BYTE);
      SPI_Exchange(EXECUTE_OUT_OF_SRAM_2_BYTE);
      SPI_Exchange(EXECUTE_OUT_OF_SRAM_3_BYTE);
      SPI_Exchange(EXECUTE_OUT_OF_SRAM_4_BYTE);
      return SUCCESS;
}




/*================   EnableDevMode   =====================================
** Function description
**  Running this command will set the mode where the Dev SRAM
**  is mapped into the code space after a reset. The mode is
**  disabled by power cycling the chip.
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static BYTE    /*return true if OK else false*/
EnableDevMode(
  BYTE chipType)
{
  if (chipType != ZW040x)
    return FAIL;

  if (!IsChipReady(chipType))
    return FAIL;

  SPI_Exchange(ENABLE_DEV_MODE_1_BYTE);
  if (chipType == ZW050x)
  {
    SPI_Exchange(0xFF);
    SPI_Exchange(0xFF);
    SPI_Exchange(0xFF);
  }
  else
  {
    SPI_Exchange(ENABLE_DEV_MODE_2_BYTE);
    SPI_Exchange(ENABLE_DEV_MODE_3_BYTE);
    SPI_Exchange(ENABLE_DEV_MODE_4_BYTE);
  }
  DELAY_36_CLK;

  if (chipType == ZW050x)
    return SUCCESS;
  if (CheckState(chipType) & STATE_DEV_MODE_ENABLED)
    return SUCCESS;

  return FAIL;
}

/*================   SetModemBits   =====================================
** Function description
**  Set the Modem Bits
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static BYTE    /*return true if OK else false*/
SetModemBits(
  BYTE chipType)
{
  if (chipType != ZW040x)
    return FAIL;

  if (!IsChipReady(chipType))
    return FAIL;

  /*Tur ON VPP*/
  PROG_VPP_HIGH;

  /*issue SPI command*/
  SPI_Exchange(SET_MODEM_BITS_1_BYTE);
  SPI_Exchange(SET_MODEM_BITS_2_BYTE);
  SPI_Exchange(SET_MODEM_BITS_3_BYTE);
  SPI_Exchange(SET_MODEM_BITS_4_BYTE);

  /*wait programming*/
  DelayUS(SET_MODEM_BITS_TWL_DURATION_US);

  /*Turn OFF VPP*/
  PROG_VPP_LOW;

  /*check bits (if modem bit is set signature bytes of chip are changed)*/
  if (ReadSignatureByte(chipType, SIG_BYTE_CHIP_TYPE) == SIG_VALUE_CHIP_TYPE_MODEM_DEVICE)
    return SUCCESS;

  return FAIL;
}


/*================   ReadLockBits500   ====================================
** Function description
**  Read the lock bits. Meaning of bits are depend from Chip Type!!!
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static BYTE    /*return byte with lock bits*/
ReadLockBits500(
  BYTE chipType,
  BYTE num
  )
{
  if (chipType ==  ZW050x)
  {
    if (IsChipReady(chipType))
    {
      SPI_Exchange(READ_LOCK_BITS_1_BYTE);
      SPI_Exchange(num);
      SPI_Exchange(0xFF);
      return SPI_Exchange(0);
    }

    return 0;
  }

  return 0;
}


/*================   ReadLockBits   ====================================
** Function description
**  Read the lock bits. Meaning of bits are depend from Chip Type!!!
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static BYTE    /*return byte with lock bits*/
ReadLockBits(
  BYTE chipType)
{
  if (chipType == ZW010x || chipType == ZW020x || chipType == ZW030x)
  {
    if (IsChipReady(chipType))
    {
      SPI_Exchange(FLASH_READ_LOCK_1_BYTE);
      SPI_Exchange(FLASH_READ_LOCK_2_BYTE);
      SPI_Exchange(FLASH_READ_LOCK_3_BYTE);

      if (chipType == ZW010x)
      {
        DELAY_9_CLK;
      }
      else if ((chipType == ZW020x) || (chipType == ZW030x))
      {
        DELAY_36_CLK;
      }

      return SPI_Exchange(FLASH_READ_LOCK_4_BYTE);
    }

    return 0;
  }
  else if (chipType == ZW040x)
  {
    if (IsChipReady(chipType))
    {
      SPI_Exchange(READ_LOCK_BITS_1_BYTE);
      SPI_Exchange(READ_LOCK_BITS_2_BYTE);
      SPI_Exchange(READ_LOCK_BITS_3_BYTE);
      return SPI_Exchange(READ_LOCK_BITS_4_BYTE);
    }

    return 0;
  }

  return 0;
}

/*================   SetLockBits500   ========================================
** Function description
**  Set the lock bits. Meaning of bits are depend from Chip Type!!!
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static BYTE    /*return true if OK else false*/
SetLockBits500(
  BYTE chipType,
  BYTE num,
  BYTE lockBits)
{
  if ((chipType == ZW050x)&&(num < 9))
  {
    if (IsChipReady(chipType))
    {
      /*Issue set lock bits command*/
      SPI_Exchange(SET_LOCK_BITS_1_BYTE);
      SPI_Exchange(num);
      SPI_Exchange(0xFF);
      SPI_Exchange(lockBits);
      /*wait programming of lock bits*/;

      DelayUS(FLASH_WRITE_LOCK_BITS_TWL_DURATION_US_ZW050x);

      return SUCCESS;
    }

    return FAIL;
  }

  return FAIL;
}



/*================   SetLockBits   ========================================
** Function description
**  Set the lock bits. Meaning of bits are depend from Chip Type!!!
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static BYTE    /*return true if OK else false*/
SetLockBits(
  BYTE chipType,
  BYTE lockBits)
{
  if (chipType == ZW010x || chipType == ZW020x || chipType == ZW030x)
  {
    if (IsChipReady(chipType))
    {
      /*set unused bits to desired values*/
      lockBits &= FLASH_WRITE_LOCK_BITS_MASK;
      /*Issue set lock bits command*/
      SPI_Exchange(FLASH_WRITE_LOCK_1_BYTE);
      SPI_Exchange(FLASH_WRITE_LOCK_2_BYTE);
      SPI_Exchange(FLASH_WRITE_LOCK_3_BYTE);
      SPI_Exchange(FLASH_WRITE_LOCK_4_BYTE | lockBits);
      /*wait programming of lock bits*/;

      if (chipType == ZW010x)
      {
        DelayUS(FLASH_WRITE_LOCK_BITS_TWL_DURATION_US_ZW010x);
      }
      else if (chipType == ZW020x || chipType == ZW030x)
      {
        DelayUS(FLASH_WRITE_LOCK_BITS_TWL_DURATION_US_ZW020x_ZW030x);
      }

      /*check bits*/
      if ((ReadLockBits(chipType) & FLASH_WRITE_LOCK_BITS_MASK) == lockBits)
        return SUCCESS;
    }

    return FAIL;
  }
  else if (chipType == ZW040x)
  {
    if (IsChipReady(chipType))
    {
      /*Tur ON VPP*/
      PROG_VPP_HIGH;

      /*set unused bits to desired values*/
      //lockBits &= SET_LOCK_BITS_MASK;

      /*Issue set lock bits command*/
      SPI_Exchange(SET_LOCK_BITS_1_BYTE);
      SPI_Exchange(lockBits);
      SPI_Exchange(SET_LOCK_BITS_3_BYTE);
      SPI_Exchange(SET_LOCK_BITS_4_BYTE);

      /*wait programming of lock bits*/;
      DelayUS(SET_LOCK_BITS_TWL_DURATION_US);

      /*Turn OFF VPP*/
      PROG_VPP_LOW;

      /*check bits*/
      if ((ReadLockBits(chipType) & SET_LOCK_BITS_MASK) == lockBits)
        return SUCCESS;
    }

    return FAIL;
  }

  return FAIL;
}

/*================   CRCCheck   ========================================
** Function description
**  Check the CRC
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static BYTE    /*return true if OK else false*/
CRCCheck(
  BYTE chipType)
{
  if ((chipType == ZW040x)||(chipType == ZW050x)){
      /*Issue command*/
      SPI_Exchange(RUN_CRC_CHECK_1_BYTE);
      SPI_Exchange(RUN_CRC_CHECK_2_BYTE);
      SPI_Exchange(RUN_CRC_CHECK_3_BYTE);
      SPI_Exchange(RUN_CRC_CHECK_4_BYTE);
      return SUCCESS;
  }
  else {
    return FAIL;
  }
}

/*================   ReadPageByTriades   ==================================
** Function description
**  This function Read data from page by triades.
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static BOOL               /*return true if OK else false*/
ReadPageByTriades(
  BYTE readCmd,           /*command by which we must read data*/
  BYTE continueReadCmd,   /*command by which we must continue read data*/
  BYTE pageNo,            /*The page address*/
  WORD pageSize,          /*Page size*/
  BYTE *data,             /*Buffer to which will be placed Data readed from page Addr*/
  BYTE isCompare,         /*TRUE to only compare readed data to data in "*data"*/
  WORD readCmdDelayUs,    /*delay in ms before data byte can be readed*/
  WORD continueReadCmdDelayUs) /*delay in ms before 3 data bytes (triada) can be readed*/
{
  WORD byteNo, triadSize;
  BYTE byte1, byte2, byte3;

  /*Read the first byte of page*/
  byteNo = pageNo * pageSize;
  SPI_Exchange(readCmd);
  SPI_Exchange(TOHIBYTE(byteNo));
  SPI_Exchange(TOLOBYTE(byteNo));
  DelayUS(readCmdDelayUs);
  byte1 = SPI_Exchange(0);

  if (isCompare)
  {
    if (data[0] != byte1)
      return FALSE;
  }
  else
  {
    data[0] = byte1;
  }

  /*Continue reads by 3 bytes*/
  triadSize = pageSize - (pageSize - 1) % 3;
  byteNo = 1;

  while (byteNo < triadSize)
  {
    SPI_Exchange(continueReadCmd);
    DelayUS(continueReadCmdDelayUs);
    byte1 = SPI_Exchange(0);
    byte2 = SPI_Exchange(0);
    byte3 = SPI_Exchange(0);

    if (isCompare)
    {
      if (data[byteNo++] != byte1)
        return FALSE;

      if (data[byteNo++] != byte2)
        return FALSE;

      if (data[byteNo++] != byte3)
        return FALSE;
    }
    else
    {
      data[byteNo++] = byte1;
      data[byteNo++] = byte2;
      data[byteNo++] = byte3;
    }
  }

  /*Reads the last bytes of page*/
  WORD addr = pageNo * pageSize + byteNo;

  while (byteNo < pageSize)
  {
    SPI_Exchange(readCmd);
    SPI_Exchange(TOHIBYTE(addr));
    SPI_Exchange(TOLOBYTE(addr));
    DelayUS(readCmdDelayUs);
    byte1 = SPI_Exchange(0);

    if (isCompare)
    {
      if (data[byteNo++] != byte1)
        return FALSE;
    }
    else
    {
      data[byteNo++] = byte1;
    }

    addr++;
  }

  return TRUE;
}

/*================   ReadPageByTriadesZW050x   ==================================
** Function description
**  This function Read data from page by triades.
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
//123    ReadPageByTriadesZW050x
static BOOL               /*return true if OK else false*/
ReadPageByTriadesZW050x(
  BYTE readCmd,           /*command by which we must read data*/
  BYTE continueReadCmd,   /*command by which we must continue read data*/
  BYTE pageNo,            /*The page address*/
  WORD pageSize,          /*Page size*/
  BYTE *data,             /*Buffer to which will be placed Data readed from page Addr*/
  BYTE isCompare,         /*TRUE to only compare readed data to data in "*data"*/
  WORD readCmdDelayUs,    /*delay in ms before data byte can be readed*/
  WORD continueReadCmdDelayUs) /*delay in ms before 3 data bytes (triada) can be readed*/
{
  static BYTE savedPageNo;
  BYTE byte1, byte2, byte3;
  WORD n;
  WORD i=0;

  if ((pageNo % 2) == 0)
  {
    //1st half
    savedPageNo = pageNo;

    /*Read the first byte of page*/
    SPI_Exchange(readCmd);
    SPI_Exchange(pageNo >> 1);
    SPI_Exchange(0);
    DelayUS(readCmdDelayUs);
    byte1 = SPI_Exchange(0);

    if (isCompare)
    {
      if (data[i++] != byte1)
        return FALSE;
    }
    else
    {
      data[i++] = byte1;
    }


    for (n = 0; n < (PACKET_SIZE_ZW5XX-1)/3; n++)
    {
      SPI_Exchange(continueReadCmd);
      DelayUS(continueReadCmdDelayUs);
      byte1 = SPI_Exchange(0);
      byte2 = SPI_Exchange(0);
      byte3 = SPI_Exchange(0);

      if (isCompare)
      {
        if (data[i++] != byte1)
          return FALSE;

        if (data[i++] != byte2)
          return FALSE;

        if (data[i++] != byte3)
          return FALSE;
      }
      else
      {
        data[i++] = byte1;
        data[i++] = byte2;
        data[i++] = byte3;
      }
    }
  }
  else
  {  //2nd half
//    if ((savedPageNo + 1) != pageNo))
//      return FALSE; // error in page sequence

    for (n = 0; n < PACKET_SIZE_ZW5XX/3 + 1; n++)
    {
      SPI_Exchange(continueReadCmd);
      DelayUS(continueReadCmdDelayUs);
      byte1 = SPI_Exchange(0);
      byte2 = SPI_Exchange(0);
      byte3 = SPI_Exchange(0);

      if (isCompare)
      {
        if ((i < PACKET_SIZE_ZW5XX) && (data[i++] != byte1))
          return FALSE;

        if ((i < PACKET_SIZE_ZW5XX) && (data[i++] != byte2))
          return FALSE;

        if ((i < PACKET_SIZE_ZW5XX) && (data[i++] != byte3))
          return FALSE;
      }
      else
      {
        if (i < PACKET_SIZE_ZW5XX)
        {
          data[i++] = byte1;
        }
        if (i < PACKET_SIZE_ZW5XX)
        {
          data[i++] = byte2;
        }
        if (i < PACKET_SIZE_ZW5XX)
        {
          data[i++] = byte3;
        }
      }
    }

  }

  return TRUE;
}


#if 0
/*================   ReadPageByTriadesZW050x   ==================================
** Function description
**  This function Read data from page by triades.
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
//123    ReadPageByTriadesZW050x
static BOOL               /*return true if OK else false*/
__ReadPageByTriadesZW050x(
  BYTE readCmd,           /*command by which we must read data*/
  BYTE continueReadCmd,   /*command by which we must continue read data*/
  BYTE pageNo,            /*The page address*/
  WORD pageSize,          /*Page size*/
  BYTE *data,             /*Buffer to which will be placed Data readed from page Addr*/
  BYTE isCompare,         /*TRUE to only compare readed data to data in "*data"*/
  WORD readCmdDelayUs,    /*delay in ms before data byte can be readed*/
  WORD continueReadCmdDelayUs) /*delay in ms before 3 data bytes (triada) can be readed*/
{
  WORD byteNo;
//    WORD triadSize;
  BYTE byte1, byte2, byte3;

  /*Read the first byte of page*/
  byteNo = pageNo * pageSize;
  SPI_Exchange(readCmd);
  SPI_Exchange(pageNo);
  SPI_Exchange(0);
  DelayUS(readCmdDelayUs);
  byte1 = SPI_Exchange(0);

  if (isCompare)
  {
    if (data[0] != byte1)
      return FALSE;
  }
  else
  {
    data[0] = byte1;
  }

  /*Continue reads by 3 bytes*/
  byteNo = 1;

  while (byteNo < pageSize + 1)
  {
    SPI_Exchange(continueReadCmd);
    DelayUS(continueReadCmdDelayUs);
    byte1 = SPI_Exchange(0);
    byte2 = SPI_Exchange(0);
    byte3 = SPI_Exchange(0);

    if (byteNo == pageSize - 1)
    {
      if (isCompare)
      {
        if (data[byteNo++] != byte1)
          return FALSE;
      }
      else
      {
        data[byteNo++] = byte1;
      }
    }
    else
    {
      if (isCompare)
      {
        if (data[byteNo++] != byte1)
          return FALSE;

        if (data[byteNo++] != byte2)
          return FALSE;

        if (data[byteNo++] != byte3)
          return FALSE;
      }
      else
      {
        data[byteNo++] = byte1;
        data[byteNo++] = byte2;
        data[byteNo++] = byte3;
      }
    }
  }

  return TRUE;
}
#endif

static BOOL             /*return true if OK else false*/
WritePageByTriadesZW050x(
      BYTE writeCmd,        /*Write command*/
      BYTE continueWriteCmd,/*Command by which we must continue write data*/
      BYTE pageNo,          /*The page address*/
      WORD pageSize,        /*Page Size*/
      BYTE *data,           /*Data to be ritten to page Addr*/
      WORD dataLength,      /*Data Length*/
      WORD writeCmdDelayUs, /*Delay in ms after data byte was written*/
      WORD continueWriteCmdDelayUs) /*delay in ms after 3 data bytes (triada) was written*/
{
    WORD n;
    WORD i = 0;
    WORD numOfTriplets = 0;
    if ((pageNo % 2) == 0)
    {
      // 1st half
      /*Write the first byte of page*/
      SPI_Exchange(writeCmd);
//      SPI_Exchange(0x00);
      SPI_Exchange(pageNo >> 1);
      SPI_Exchange(0x00);
      SPI_Exchange(data[i++]);
      DelayUS(writeCmdDelayUs);

      numOfTriplets = ((dataLength - 1) / 3);
      for (n = 0; n < numOfTriplets; n++)
      {
        SPI_Exchange(continueWriteCmd);
        SPI_Exchange(data[i++]);
        SPI_Exchange(data[i++]);
        SPI_Exchange(data[i++]);
        DelayUS(continueWriteCmdDelayUs);
      }

    }
    else
    {
      // 2nd half
      numOfTriplets = ((dataLength - 1) / 3);//+1;
      for (n = 0; n < numOfTriplets; n++)
        {
          SPI_Exchange(continueWriteCmd);
          SPI_Exchange(data[i++]);
          SPI_Exchange(data[i++]);
          SPI_Exchange(data[i++]);
          DelayUS(continueWriteCmdDelayUs);
        }
      }
    return TRUE;
}


/*================   WritePageByTriades   ================================
** Function description
**  This function Write a page by triades.
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static BOOL             /*return true if OK else false*/
WritePageByTriades(
  BYTE writeCmd,        /*Write command*/
  BYTE continueWriteCmd,/*Command by which we must continue write data*/
  BYTE pageNo,          /*The page address*/
  WORD pageSize,        /*Page Size*/
  BYTE *data,           /*Data to be ritten to page Addr*/
  WORD writeCmdDelayUs, /*Delay in ms after data byte was written*/
  WORD continueWriteCmdDelayUs) /*delay in ms after 3 data bytes (triada) was written*/
{
  WORD byteNo, triadSize;

  /*Write the first byte of page*/
  byteNo = pageNo * pageSize;
  SPI_Exchange(writeCmd);
  SPI_Exchange(TOHIBYTE(byteNo));
  SPI_Exchange(TOLOBYTE(byteNo));
  SPI_Exchange(data[0]);
  DelayUS(writeCmdDelayUs);

  /*Continue writes by 3 bytes*/
  triadSize = pageSize - (pageSize - 1) % 3;
  byteNo = 1;

  while (byteNo < triadSize)
  {
    SPI_Exchange(continueWriteCmd);
    SPI_Exchange(data[byteNo++]);
    SPI_Exchange(data[byteNo++]);
    SPI_Exchange(data[byteNo++]);
    DelayUS(continueWriteCmdDelayUs);
  }

  /*Writes the last bytes of page*/
  WORD addr = pageNo * pageSize + byteNo;

  while (byteNo < pageSize)
  {
    SPI_Exchange(writeCmd);         /*Actually this block of code MUST not execute  */
    SPI_Exchange(TOHIBYTE(addr));   /* while programming OTP!!! because OTP will be damaged!!!*/
    SPI_Exchange(TOLOBYTE(addr));   /*For more info see comments near PAGE_SIZE_ZW040x define !!!*/
    SPI_Exchange(data[byteNo++]);
    DelayUS(writeCmdDelayUs);
    addr++;
  }

  return TRUE;
}



/*===============================   ReadPage_020x   ===============================
**  This function Read  data from a flash page.
** return none
**--------------------------------------------------------------------------*/
static WORD /*return page size*/
ReadPage(
  BYTE chipType,
  BYTE pageNo, /*The page address*/
  BYTE *data)/*Data to be read from page Addr*/
{

  WORD pageSize = 0;

  if (chipType == ZW050x) //123
  {
    /* Read the ZW050x Flash sector*/
    pageSize = PACKET_SIZE_ZW5XX; // SECTOR_SIZE_ZW050x / 2;

    if (!IsChipReady(chipType))
    {
      return 0;
    }

    if (!ReadPageByTriadesZW050x(
//          READ_OTP_1_BYTE,
          READ_FLASH_ZW050X,
          CONTINUE_READ_1_BYTE,
          pageNo,
          pageSize,
          data,
          FALSE,
          READ_OTP_DURATION_US,
          CONTINUE_READ_DURATION_US)
       )
    {
      return 0;
    }

    return pageSize;
  }

  if (chipType == ZW040x)
  {
    /* Read the ZW040x OTP page*/
    pageSize = PAGE_SIZE_ZW040x;

    if (!IsChipReady(chipType))
    {
      return 0;
    }

    if (!ReadPageByTriades(
          READ_OTP_1_BYTE,
          CONTINUE_READ_1_BYTE,
          pageNo,
          pageSize,
          data,
          FALSE,
          READ_OTP_DURATION_US,
          CONTINUE_READ_DURATION_US)
       )
    {
      return 0;
    }

    return pageSize;
  }

  if ((chipType == ZW020x) || (chipType == ZW030x))
  {
    pageSize = PAGE_SIZE_ZW020x;
  }
  else if (chipType == ZW010x)
  {
    pageSize = PAGE_SIZE_ZW010x;
  }

  for (WORD byteNo = 0; byteNo < pageSize; byteNo++)
  {
    if ((byteNo & 0x01))
    {
      SPI_Write((READ_PROG_MEM_1_BYTE | ODD_BYTE));
    }
    else
    {
      SPI_Write((READ_PROG_MEM_1_BYTE | EVEN_BYTE));
    }

    SPI_Write(pageNo);

    if (chipType == ZW010x)
    {
      SPI_Write((BYTE)((byteNo & 0xFE) << 1));
    }
    else
    {
      SPI_Write((BYTE)(byteNo & 0xFE));
    }

    if (chipType == ZW010x)
    {
      DELAY_9_CLK;
    }
    else if ((chipType == ZW020x) || (chipType == ZW030x))
    {
      DELAY_36_CLK;

    }

    data[byteNo] = SPI_Read();
  }

  return pageSize;

}


/*================   SRAMReadPage   ========================================
** Function description
**  This function Read data from a SRAM page.
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static WORD           /*return page size*/
SRAMReadPage(
  BYTE chipType,
  BYTE pageNo,       /*The page address*/
  BYTE *data)        /*Data to be read from page Addr*/
{

  if (chipType == ZW050x)
  {
//    WORD pageSize = PACKET_SIZE_ZW5XX;

    if (!IsChipReady(chipType))
    {
      return 0;
    }
    if (pageNo == 2)
    {
      pageNo = 0x8<<1;
    }
    if (pageNo == 3)
    {
      pageNo = (0x8<<1) + 1;
    }
    if (!ReadPageByTriadesZW050x(
                           READ_SRAM_1_BYTE,
                           CONTINUE_READ_1_BYTE,
                           pageNo,
                           PACKET_SIZE_ZW5XX, //pageSize,
                           data,
                           FALSE,
                           READ_SRAM_DURATION_US,
                           CONTINUE_READ_DURATION_US)
                          )
    {
      return 0;
    }

    return PACKET_SIZE_ZW5XX; // pageSize;
  }

  else if (chipType == ZW040x)
  {
//    WORD pageSize = PAGE_SIZE_ZW040x;

    if (!IsChipReady(chipType))
    {
      return 0;
    }

    if (!ReadPageByTriades(READ_SRAM_1_BYTE,
                           CONTINUE_READ_1_BYTE,
                           pageNo,
                           PAGE_SIZE_ZW040x, //pageSize,
                           data,
                           FALSE,
                           READ_SRAM_DURATION_US,
                           CONTINUE_READ_DURATION_US)
                          )
    {
      return 0;
    }

    return PAGE_SIZE_ZW040x; //pageSize;
  }

  else
  {
    return 0;
  }
}


/*================   SRAMWritePage   ======================================
** Function description
**  This function Write a SRAM page.
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static BYTE          /*return SUCCESS if data written correctly else FAIL*/
SRAMWritePage(
  BYTE chipType,
  BYTE pageNo,       /*The page address*/
  BYTE verify,
  WORD dataLength,
  BYTE *data)        /*Data to be written to page Addr*/
{

  WORD pageSize;

  if (chipType == ZW050x)
  {
//      pageSize = PACKET_SIZE_ZW5XX; // SECTOR_SIZE_ZW050x / 2;
      if (pageNo == 2)
      {
        pageNo = 0x8<<1;
      }
      if (pageNo == 3)
      {
        pageNo = (0x8<<1) + 1;
      }
      if (!WritePageByTriadesZW050x(
                              WRITE_SRAM_1_BYTE,
                              CONTINUE_WRITE_1_BYTE,
                              pageNo,
                              //pageSize,
                              PACKET_SIZE_ZW5XX,
                              data,
                              dataLength,
                              WRITE_SRAM_DURATION_US,
                              CONTINUE_WRITE_DURATION_US)
         )
      {
        return FAIL;
      }

      if (verify)
      {
        if (!ReadPageByTriadesZW050x(
                               READ_SRAM_1_BYTE,
                               CONTINUE_READ_1_BYTE,
                               pageNo,
                               //pageSize,
                               PACKET_SIZE_ZW5XX,
                               data,
                               TRUE,
                               READ_SRAM_DURATION_US,
                               CONTINUE_READ_DURATION_US)
           )
        {
          return FAIL;
        }
      }
  }

  else if (chipType == ZW040x)
  {
    pageSize = PAGE_SIZE_ZW040x;

    if (!IsChipReady(chipType))
    {
      return FAIL;
    }

    if (!WritePageByTriades(WRITE_SRAM_1_BYTE,
                            CONTINUE_WRITE_1_BYTE,
                            pageNo,
                            pageSize,
                            data,
                            WRITE_SRAM_DURATION_US,
                            CONTINUE_WRITE_DURATION_US)
       )
    {
      return FAIL;
    }

    if (verify)
    {
      if (!ReadPageByTriades(READ_SRAM_1_BYTE,
                             CONTINUE_READ_1_BYTE,
                             pageNo,
                             pageSize,
                             data,
                             TRUE,
                             READ_SRAM_DURATION_US,
                             CONTINUE_READ_DURATION_US)
         )
      {
        return FAIL;
      }
    }
  }
  else
  {
    return FAIL;
  }

  return SUCCESS;
}

/*================   SRAMReadWrite   ======================================
** Function description
**  This function Reads or Writes a SRAM to of from a buffer by pages.
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static BOOL          /*return TRUE if data readed / written correctly else FALSE*/
SRAMReadWrite(
  BOOL isRead,       /*true - read SRAM, false - write SRAM */
  BYTE *data,        /*Data to be read / write*/
  WORD dataSize,     /*Size of Data */
  BYTE verify)       /*true if verify after write is needed*/
{
  BYTE pages_count = dataSize / PAGE_SIZE_ZW040x;

  if ((dataSize % PAGE_SIZE_ZW040x) != 0)
  {
    pages_count++;
  }

  BYTE cur_page = 0;
  WORD buffer_offset = 0;

  do
  {
    if (isRead)
    {
      if (SRAMReadPage(ZW040x, cur_page, &data[buffer_offset]) != PAGE_SIZE_ZW040x)
      {
        break;
      }
    }
    else
    {
      if (SRAMWritePage(ZW040x, cur_page, verify, 0, &data[buffer_offset]) != SUCCESS)
      {
        break;
      }
    }

    cur_page++;
    buffer_offset += PAGE_SIZE_ZW040x;
  }
  while (--pages_count != 0);

  return (pages_count == 0);
}

/*================   MTPRead   ========================================
** Function description
**  This function Read data from a MTP Memory.
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static WORD           /*return MTP size*/
MTPRead(
  BYTE chipType,
  BYTE *data)         /*Buffer where readed data will be stored*/
{
  if (chipType != ZW040x)
  {
    return 0;
  }

  BYTE old_spi_speed = SPI_UseLowSpeed;
  SPI_UseLowSpeed = TRUE;

  /*Issue Read MTP to MTP Reg file Data Command*/
  SPI_Exchange(READ_MTP_1_BYTE);
  SPI_Exchange(READ_MTP_2_BYTE);
  SPI_Exchange(READ_MTP_3_BYTE);
  SPI_Exchange(READ_MTP_4_BYTE);

  /*Wait for MTP Reading ends*/
  DelayUS(READ_MTP_DURATION_US);

  /* Read the MTP data from MTP Reg file*/
  if (!ReadPageByTriades(READ_MTP_REG_1_BYTE, CONTINUE_READ_MTP_REG_1_BYTE, 0, MTP_SIZE, data,
                         FALSE, READ_MTP_REG_DURATION_US, CONTINUE_READ_MTP_REG_DURATION_US))
  {
    SPI_UseLowSpeed = old_spi_speed;
    return 0;
  }

  SPI_UseLowSpeed = old_spi_speed;
  return MTP_SIZE;
}


/*================   MTPWrite   ======================================
** Function description
**  This function Write a data to the MTP Memory.
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static BYTE    /*return SUCCESS if data written correctly else FAIL*/
MTPWrite(
  BYTE chipType,
  BYTE verify,
  BYTE *data)   /*Data to be written to memory*/
{
  if (chipType != ZW040x)
  {
    return FAIL;
  }

  BYTE old_spi_speed = SPI_UseLowSpeed;
  SPI_UseLowSpeed = TRUE;

  /*Store data to the MTP Register file*/
  if (!WritePageByTriades(WRITE_MTP_REG_1_BYTE, CONTINUE_WRITE_MTP_REG_1_BYTE, 0, MTP_SIZE, data,
                          WRITE_MTP_REG_DURATION_US, CONTINUE_WRITE_MTP_REG_DURATION_US))
  {
    SPI_UseLowSpeed = old_spi_speed;
    return FAIL;
  }

  /*Issue Write MPT Reg file Data to the MTP Memory Command*/
  SPI_Exchange(WRITE_MTP_1_BYTE);
  SPI_Exchange(WRITE_MTP_2_BYTE);
  SPI_Exchange(WRITE_MTP_3_BYTE);
  SPI_Exchange(WRITE_MTP_4_BYTE);

  /*Wait for MTP Programming ends*/
  DelayUS(WRITE_MTP_DURATION_US);

  /*Verify data in MTP if requested*/
  if (verify)
  {
    if (!ReadPageByTriades(READ_MTP_REG_1_BYTE, CONTINUE_READ_MTP_REG_1_BYTE, 0, MTP_SIZE, data,
                           TRUE, READ_MTP_REG_DURATION_US, CONTINUE_READ_MTP_REG_DURATION_US))
    {
      SPI_UseLowSpeed = old_spi_speed;
      return FAIL;
    }
  }

  SPI_UseLowSpeed = old_spi_speed;
  return SUCCESS;
}

/*================   MTPFill   ======================================
** Function description
**  This function fills the entire MTP memory by specified value
**
** Side effects: none
**
**-------------------------------------------------------------------------*/
static BYTE       /*return SUCCESS if MTP filled correctly else FAIL*/
MTPFill(
  BYTE chipType,  /*chip type*/
  BYTE fillValue) /*fill value to be writted to entire MTP Memory*/
{
  if (chipType != ZW040x)
  {
    return FAIL;
  }

  BYTE old_spi_speed = SPI_UseLowSpeed;
  SPI_UseLowSpeed = TRUE;

  /*Issue MTP Fill Command*/
  SPI_Exchange(FILL_MTP_1_BYTE);
  SPI_Exchange(FILL_MTP_2_BYTE);
  SPI_Exchange(FILL_MTP_3_BYTE);
  SPI_Exchange(fillValue);

  /*Wait until MTP will be filled by value*/
  DelayUS(FILL_MTP_DURATION_US);

  /*Verify data in MTP*/
  if (MTPRead(chipType, compl_workbuf) != MTP_SIZE)
  {
    SPI_UseLowSpeed = old_spi_speed;
    return FAIL;
  }

  for (BYTE i = 0; i < MTP_SIZE; i++)
  {
    if (compl_workbuf[i] != fillValue)
    {
      SPI_UseLowSpeed = old_spi_speed;
      return FAIL;
    }
  }

  SPI_UseLowSpeed = old_spi_speed;
  return SUCCESS;
}

/*#include "UART_buf_io.h"
void SerialPutWord(WORD data)
{
  SerialPutByte((data >>  8) & 0xff);
  SerialPutByte((data >>  0) & 0xff);
}
void SerialPutDword(DWORD data)
{
  SerialPutByte((data >> 24) & 0xff);
  SerialPutByte((data >> 16) & 0xff);
  SerialPutByte((data >>  8) & 0xff);
  SerialPutByte((data >>  0) & 0xff);
}*/

/*===============================   WritePage   ===============================
**  This function Write a flash page.
** return none
**--------------------------------------------------------------------------*/

static BYTE    /*return true if data writen correctly else false*/
WritePage(
  BYTE chipType,
  BYTE pageNo, /*The page address*/
  BYTE verify,
  WORD startAd,
  WORD stopAd,
  WORD dataLength,
  BYTE *data)/*Data to be ritten to page Addr*/
{
  WORD pageSize = 0;
  BYTE i;
  static int startAddr;
  static int stopAddr;
  static int currentAddr;
  static BYTE numBytes;
  static char bytes[2];

  if (chipType == ZW050x) //123 WritePage
  {
    /*Write ZW050x flash*/
    static BYTE wrPageNo;
    if ((pageNo % 2) == 0)
    {
      // 1st half
      wrPageNo = pageNo;

      currentAddr = startAd;  // 1
      startAddr = startAd;
      stopAddr = stopAd;
      numBytes = 0;

      if (currentAddr > stopAddr)
      {
        return SUCCESS;
      }

      if (currentAddr > (PACKET_SIZE_ZW5XX - 1))      // 2
      {
        return SUCCESS;
      }
      else
      {
                SPI_Exchange(WRITE_SRAM_1_BYTE);        // 3
                SPI_Exchange(HIBYTE(currentAddr));
                SPI_Exchange(LOBYTE(currentAddr));
                SPI_Exchange(data[currentAddr]);
                DelayUS(WRITE_SRAM_DURATION_US);
                currentAddr++;

                for(;;)                                 // 8
                {
                  if ((currentAddr > stopAddr) || (currentAddr > (PACKET_SIZE_ZW5XX - 1))) //4 //  4
                    break;

                  if (numBytes > 1)                               // 6
                  {
                    SPI_Exchange(CONTINUE_WRITE_1_BYTE);         // 7
                    SPI_Exchange(bytes[0]);
                    SPI_Exchange(bytes[1]);
                    SPI_Exchange(data[currentAddr]);
                    DelayUS(CONTINUE_WRITE_DURATION_US);
                    numBytes = 0;
                  }
                  else
                  {
                    bytes[numBytes] = data[currentAddr];   // 5
                    numBytes++;
                  }
                  currentAddr++;
                }                              // 8

                if (stopAddr  < PACKET_SIZE_ZW5XX )        // 9
                {
                  /*Issue Write Write Flash Sector Command*/
                  SPI_Exchange(WRITE_FLASH_ZW050X);         // 10
                  SPI_Exchange(pageNo >> 1);
                  SPI_Exchange(0xFF);
                  SPI_Exchange(0xFF);
                  DelayUS(WRITE_FLASH_ZW050X_BYTE_DURATION_MAX_US * SECTOR_SIZE_ZW050x);

                   for (i = 0; i < numBytes; i++)
                   {
                     if(bytes[i]!=0xFF){
                       SPI_Exchange(WRITE_SRAM_1_BYTE);
                       SPI_Exchange(HIBYTE(currentAddr-numBytes));
                       SPI_Exchange(LOBYTE(currentAddr-numBytes));
                       SPI_Exchange(bytes[i]);
                       DelayUS(WRITE_SRAM_DURATION_US);

                       /*Issue Write Write Flash Sector Command*/
                       SPI_Exchange(WRITE_FLASH_ZW050X);
                       SPI_Exchange(pageNo >> 1);
                       SPI_Exchange(0xFF);
                       SPI_Exchange(0xFF);
                       DelayUS(WRITE_FLASH_ZW050X_BYTE_DURATION_MAX_US * SECTOR_SIZE_ZW050x);
                     }
                     currentAddr++;
                   }
                   numBytes = 0;
                }
                return SUCCESS;       // 11
      }
    }
    else
    {
      // 2nd half

      if ((wrPageNo +1 ) != pageNo)
      {
          return FAIL;//invalid page number sequence
      }
      if (stopAddr < PACKET_SIZE_ZW5XX)   // 1
      {
          return SUCCESS; //nothing todo
      }

      currentAddr -= PACKET_SIZE_ZW5XX;
      stopAddr -= PACKET_SIZE_ZW5XX;

      if (startAddr > (PACKET_SIZE_ZW5XX - 1))      // 2
      {
          SPI_Exchange(WRITE_SRAM_1_BYTE);        // 3
          SPI_Exchange(HIBYTE(currentAddr + PACKET_SIZE_ZW5XX));
          SPI_Exchange(LOBYTE(currentAddr + PACKET_SIZE_ZW5XX));
          SPI_Exchange(data[currentAddr]);
          DelayUS(WRITE_SRAM_DURATION_US);
          currentAddr++;
      }

      for(;;)                                 // 4
      {
        if (currentAddr > stopAddr)        // 5
          break;

        if (numBytes > 1)                               // 6
        {
          SPI_Exchange(CONTINUE_WRITE_1_BYTE);         // 7
          SPI_Exchange(bytes[0]);
          SPI_Exchange(bytes[1]);
          SPI_Exchange(data[currentAddr]);
          DelayUS(CONTINUE_WRITE_DURATION_US);
          numBytes = 0;
        }
        else
        {
          bytes[numBytes] = data[currentAddr];   // 8
          numBytes++;
        }
        currentAddr++;                           //9
      }                                       // 4

      /*Issue Write Write Flash Sector Command*/
      SPI_Exchange(WRITE_FLASH_ZW050X);         // 10
      SPI_Exchange(pageNo >> 1);
      SPI_Exchange(0xFF);
      SPI_Exchange(0xFF);
      DelayUS(WRITE_FLASH_ZW050X_BYTE_DURATION_MAX_US * SECTOR_SIZE_ZW050x);

      for (i = 0; i < numBytes; i++)
      {
        if(bytes[i]!=0xFF){
          SPI_Exchange(WRITE_SRAM_1_BYTE);
          SPI_Exchange(HIBYTE(currentAddr + PACKET_SIZE_ZW5XX-numBytes));
          SPI_Exchange(LOBYTE(currentAddr + PACKET_SIZE_ZW5XX-numBytes));
          SPI_Exchange(bytes[i]);
          DelayUS(WRITE_SRAM_DURATION_US);

          /*Issue Write Write Flash Sector Command*/
          SPI_Exchange(WRITE_FLASH_ZW050X);
          SPI_Exchange(pageNo >> 1);
          SPI_Exchange(0xFF);
          SPI_Exchange(0xFF);
          DelayUS(WRITE_FLASH_ZW050X_BYTE_DURATION_MAX_US * SECTOR_SIZE_ZW050x);
        }
        currentAddr++;
      }
      numBytes = 0;
      return SUCCESS;
    }
  }

  if (chipType == ZW040x)
  {
    /*Write ZW040x OTP Page*/
    pageSize = PAGE_SIZE_ZW040x;

    if (!IsChipReady(chipType))
    {
      return FAIL;
    }

    /*Store data to the SRAM*/
    if (SRAMWritePage(chipType, 0, TRUE, 0, data) != SUCCESS)
    {
      return FAIL;
    }

    if (!IsChipReady(chipType))
    {
      return FAIL;
    }

    /*Tur ON VPP*/
    PROG_VPP_HIGH;

    /*Issue Write OTP Data Command*/
    SPI_Exchange(WRITE_OTP_1_BYTE);
    WORD addr = pageNo * pageSize;
    SPI_Exchange(TOHIBYTE(addr));
    SPI_Exchange(TOLOBYTE(addr));
    SPI_Exchange(WRITE_OTP_4_BYTE);

    /*Wait for OTP Programming ends*/
    DWORD tryCount = pageSize;

    /*wait number of bytes delays*/

//{BYTE s = CheckState(chipType); WORD w = ReadWriteOtpStats(chipType);
//SerialPutByte(0xB0); SerialPutByte(s); SerialPutWord(w); SerialPutDword(tryCount);}
    do
      DelayUS(WRITE_OTP_TWP_DURATION_MIN_US);

    while (--tryCount);

//{BYTE s = CheckState(chipType); WORD w = ReadWriteOtpStats(chipType);
//SerialPutByte(0xB1); SerialPutByte(s); SerialPutWord(w);}
    /*make additional delay (maximum) with parallel checking of Write Busy*/
#define WRITE_OTP_TRY_PERIOD_US 10
    tryCount = ((10 * WRITE_OTP_TWP_DURATION_MAX_US) - WRITE_OTP_TWP_DURATION_MIN_US) / WRITE_OTP_TRY_PERIOD_US;
    tryCount *= pageSize;
    BYTE state;

//SerialPutDword(tryCount);
    do
    {
      state = CheckState(chipType);

      if (!(state & STATE_WRITE_BUSY) ||
          (state & STATE_WRITE_FAILED))
        break;

      DelayUS(WRITE_OTP_TRY_PERIOD_US);
    }
    while (tryCount--);

//{BYTE s = CheckState(chipType); WORD w = ReadWriteOtpStats(chipType);
//SerialPutByte(0xB2); SerialPutByte(s); SerialPutWord(w);}

    /*Turn OFF VPP*/
    PROG_VPP_LOW;

//{BYTE s = CheckState(chipType); WORD w = ReadWriteOtpStats(chipType);
//SerialPutByte(0xB3); SerialPutByte(s); SerialPutWord(w);}
    /*check last state*/
    if (state & (STATE_WRITE_BUSY | STATE_WRITE_FAILED))
      return FAIL;

//SerialPutByte(0xB4);

    /*verify data in OTP if requested*/
    if (verify)
    {
//SerialPutByte(0xB5);
      if (!IsChipReady(chipType))
      {
        return FAIL;
      }

//SerialPutByte(0xB6);
      if (!ReadPageByTriades(
            READ_OTP_1_BYTE,
            CONTINUE_READ_1_BYTE,
            pageNo,
            pageSize,
            data,
            TRUE,
            READ_OTP_DURATION_US,
            CONTINUE_READ_DURATION_US)
         )
      {
        return FAIL;
      }
    }

//SerialPutByte(0xB7);

    return SUCCESS;
  }

  /*The memory load command is 'LOAD_CMD DONT_CARE byte_Addr Data_byte'  */
  if ((chipType == ZW020x) || (chipType == ZW030x))
  {
    pageSize = PAGE_SIZE_ZW020x;
  }
  else if (chipType == ZW010x)
  {
    pageSize = PAGE_SIZE_ZW010x;
  }

  for (WORD byteNo = 0; byteNo < pageSize; byteNo++)
  {
    if (byteNo & 0x01)
    {/*The first byte of the load cmd contain the first bit of the data byte index*/
      SPI_Write((LOAD_PROG_MEM_1_BYTE | ODD_BYTE));
    }
    else
    {
      SPI_Write((LOAD_PROG_MEM_1_BYTE | EVEN_BYTE));
    }

    SPI_Write(DONT_CARE);

    if (chipType == ZW010x)
    {
      SPI_Write((BYTE)((byteNo & 0xFE) << 1));
    }
    else if ((chipType == ZW020x) || (chipType == ZW030x))
    {
      SPI_Write((BYTE)(byteNo & 0xFE));
    }

    SPI_Write(data[byteNo]);
  }

  /*Issue a write page commad*/
  SPI_Write(WRITE_PROG_1_BYTE);
  SPI_Write(pageNo);
  SPI_Write(DONT_CARE);
  SPI_Write(DONT_CARE);

  DelayMs(PAGE_WRITE_DELAY);

  if (verify)
  {
    // 123     readBuf[256]; local var
#if 0
    BYTE readBuf[256];
    ReadPage(chipType, pageNo, readBuf);

    for (WORD byteNo = 0; byteNo < pageSize; byteNo++)
    {
      if (readBuf[byteNo] != data[byteNo])
      {
        return FAIL;
      }
    }
#else
    ReadPage(chipType, pageNo, &compl_workbuf[512]);
    for (WORD byteNo = 0; byteNo < pageSize; byteNo++)
    {
      if (compl_workbuf[512 + byteNo] != data[byteNo])
      {
        return FAIL;
      }
    }
#endif
    return SUCCESS;

  }
  else
  {
    return SUCCESS;
  }
}



/*=============================================================================
  Erase page of flash
**-----------------------------------------------------------------------------*/
BYTE    /*return true if OK else false*/
ErasePage(BYTE chipType,
          BYTE pageNo)
{
  if ((serFrameDataPtr[0] == ZW020x) || (serFrameDataPtr[0] == ZW030x))
  {
    SPI_Exchange(PAGE_ERASE_1_BYTE);
    SPI_Exchange(PAGE_ERASE_2_BYTE);
    SPI_Exchange(serFrameDataPtr[1]);
    SPI_Exchange(DONT_CARE);
    return SUCCESS;
  }
  else if (chipType == ZW050x)
  {
    SPI_Exchange(SECTOR_ERASE_ZW050X);
    SPI_Exchange(pageNo);
    SPI_Exchange(0xFF);
    SPI_Exchange(0xFF);
    return SUCCESS;
  }

  return FAIL;
}

#endif /*ZWAVE_PROGRAMMER*/

/*
 Check the sequence no in the frame

*/
static BYTE
CheckSeqNo(BYTE  *buf )
{
  WORD seqOffset =  ((((WORD)buf[0]) << 8) | buf[1]) - 1;

  if (buf[seqOffset] != seqNo)
  {
    seqNo = buf[seqOffset];
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

static BYTE dummybuf;
/*===============================   Respond   ===============================
**    Send immediate respons to remote side
**
**    Side effects: Sets state variable to stateTxSerial (wait for ack)
**
**--------------------------------------------------------------------------*/
void             /*RET  Nothing                 */
Respond(
  BYTE cmd,       /*IN   Command                  */
  BYTE *pData,    /*IN   pointer to data          */
  WORD len )      /*IN   Length of data           */
{
  /* If there are no data; pData == NULL and len == 0 we must set the data pointer
     to some dummy data. ConTxFrame interprets NULL pointer as retransmit indication*/
  if (len == 0)
    pData = &dummybuf;

  pData[len] = seqNo;
  len++;
  ConTxFrame(cmd, RESPONSE, pData, len);
  state = stateTxSerial;  /* We want ACK/NAK...*/
#ifdef ZWAVE_PROGRAMMER
  wdt_reset();
#endif
}

/*===============================   Request   ===============================
**    Send immediate request to remote side
**
**    Side effects: Sets state variable to stateTxSerial (wait for ack)
**
**--------------------------------------------------------------------------*/
void             /*RET  Nothing                 */
Request(
  BYTE cmd,       /*IN   Command                  */
  BYTE *pData,    /*IN   pointer to data          */
  WORD len )      /*IN   Length of data           */
{
  /* If there are no data; pData == NULL and len == 0 we must set the data pointer
     to some dummy data. ConTxFrame interprets NULL pointer as retransmit indication*/
  if (len == 0)
    pData = &dummybuf;

  pData[len] = ++requestSeqNo;
  len++;
  ConTxFrame(cmd, REQUEST, pData, len);
  state = stateTxSerial;  /* We want ACK/NAK...*/
#ifdef ZWAVE_PROGRAMMER
  wdt_reset();
#endif
}

#ifdef BOOT_LOADER

void
EraseAtmel(void)
{
  long address = 0;
  BYTE tmp = 0xFF ;
  tmp ^= btSig[0];
  tmp ^= btSig[1];
  tmp ^= btSig[2];
  tmp ^= btSig[3];
  tmp ^= btSig[4];

  if (tmp)  /*the boot loade signature is not correct abondon erasing*/
  {
    compl_workbuf[0] = 0xF0;
    PIN_ON(LEDB);
  }
  else
  {
    tmp = SREG;
    cli();

    while ( APP_END > address )
    {
      boot_page_erase(address); // Perform page erase
      boot_spm_busy_wait();   // Wait until the memory is erased.
      address += SPM_PAGESIZE;
    }

    boot_rww_enable();
    compl_workbuf[0] = DONE;
    SREG = tmp;
    sei();
  }

  compl_workbuf[1] = btSig[0];
  compl_workbuf[2] = btSig[1];
  compl_workbuf[3] = btSig[2];
  compl_workbuf[4] = btSig[3];
  compl_workbuf[5] = btSig[4];

  Respond(serFrameCmd, compl_workbuf, 6);
}
#endif

#ifdef ZWAVE_PROGRAMMER

BYTE ZW0x0xProgEnable(void)
{
  PIN_IN(SS_N, 1);
  MOSI_LOW;
  SCK_LOW;
  PIN_OUT(MOSI);
  PIN_OUT(SCK);

  SPI_Init(&PORTD, &PIND, (1 << MOSI), (1 << MISO), (1 << SCK));

  compl_workbuf[0] = AutoInit();

  if (compl_workbuf[0] < MAX_SYNC)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }

}
#endif




//123   freeRam
/*
int freeRam (void)
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
*/
int main(void)
{
  /*
   extern int __heap_start, *__brkval;
   int a;
   a = freeRam();
   a = __heap_start;
  */
#ifdef ZWAVE_PROGRAMMER
  NVM_ADDRESS startAdr;
  NVM_ADDRESS addr;
  NVM_ADDRESS offset;
#endif
  cli();
  /* disable WDT*/
  wdt_disable();
  sei();

#ifdef BOOT_LOADER
  /* Enable change of interrupt vectors */
  MCUCR  = (1 << IVCE);
  /* Move interrupts to bootboot flash section */
  MCUCR  = (1 << IVSEL);
#endif
  /*Reset the module after the ATMEL reset*/
  PIN_OUT(RESET_N);
  PROG_VPP_LOW;         /*Set VPP to the low value (3.3V)*/
  PIN_OUT(PROG_VPP);    /*Configure the VPP control pin as output*/
#ifdef ZWAVE_PROGRAMMER
  Reset_N_Low();
  DelayMs(1);
  Reset_N_High();

#endif

  //  Set the JTD bit high to disable the JTAG interface.
  // the bit should be written twice with 4 cycles
  BYTE tmp = MCUCSR;
  tmp |=  (1 << JTD);
  MCUCSR = tmp;
  MCUCSR = tmp;
  // init the calibration enabled pin pin 4 on port F
  CAL_EN_HIGH;
  PIN_OUT(CAL_EN);
  /* start with all I/O as inputs*/

  PIN_IN(SCK, 0);
  PIN_IN(MOSI, 0);
  PIN_IN(MISO, 0);
  PIN_IN(SS_N, 1);
  PIN_IN(IO9, 1);
  PIN_IN(RESET_N, 1);

  /* Initialization */
#ifdef ZWAVE_PROGRAMMER
  void (*BootLoader)( void ) =  (void*)0x1F000; // Set up function pointer to BootLoader code.
  PROGPORT |= (1 << PROG_NO); // Enable pull-up on PROG_NO line on PROGPORT.
#endif
  PIN_OUT(LEDA);
  PIN_OUT(LEDB);
  PIN_OUT(LEDC);
  PIN_OUT(LEDD);

  PIN_OUT(SCK_CS);
  PIN_OFF(SCK_CS);

  PIN_OFF(LEDA);
  PIN_OFF(LEDB);
  PIN_OFF(LEDC);
  PIN_OFF(LEDD);


  PIN_IN(LED1, FALSE);
  PIN_IN(LED2, FALSE);
  PIN_IN(LED3, FALSE);
  PIN_IN(LED4, FALSE);
  PIN_IN(LED5, FALSE);
  PIN_IN(LED6, FALSE);


  /* Branch to bootloader or application code? */
  ConInit();

  state = stateIdle;
  seqNo = 0;
  requestSeqNo = 0;

#ifdef ZWAVE_PROGRAMMER
  PIN_IN(PUSHB1, TRUE);
  buttonState = 0;
  buttonPressed = 0;

  wdt_enable(WDTO_2S);
#endif
#ifdef ZWAVE_PROGRAMMER
  PIN_ON(LEDA); /*We are in bootloader mode set LEDA On*/
#endif
#ifdef BOOT_LOADER
  PIN_ON(LEDD);
#endif
  while (1)
  {
#ifdef ZWAVE_PROGRAMMER
    wdt_reset();

    if (!PIN_GET(PUSHB1))
    {
      if (!buttonState)
      {
        buttonPressed = 1;
      }

      buttonState = 1;
    }
    else
    {
      buttonState = 0;
    }

#endif

    /* ApplicationPoll is controlled by a statemachine with the four states:
    stateIdle, stateFrameParse, stateTxSerial, stateCbTxSerial.

    stateIdle: If there is anything to transmit do so. -> stateCbTxSerial
    If not, check if anything is received. -> stateFrameParse
    If neither, stay in the state
    Note: frames received while we are transmitting are lost
    and must be retransmitted by PC

    stateFrameParse: Parse received frame.
    If the request has no response -> stateIdle
    If there is an immediate response send it. -> stateTxSerial

    stateTxSerial:  Waits for ack on responses send in stateFrameParse.
    Retransmit frame as needed.
    -> stateIdle

    */

    switch (state)
    {
      case stateIdle :
      {
#ifdef ZWAVE_PROGRAMMER
        wdt_reset();
#endif
        BYTE conState = ConUpdate(TRUE);

        /* Nothing to transmit. Check if we received anything */
        if (conState == conFrameReceived)
        {
          /* We got a frame... */
          state = stateFrameParse;
        }
        else if (conState == conIdle)
        {
#ifdef ZWAVE_PROGRAMMER

          if (waitCmd == FUNC_ID_M128_ENTER_PROG_MODE)
          {
            waitCmd = 0;
            cli();
            BootLoader(); /*jum to boot loader*/
          }
          else
#endif
            if (waitCmd == FUNC_ID_EXIT_PROG_MODE)/*We have received EXIT command*/
            {// restart
              PIN_IN(SCK, 0);
              PIN_IN(MOSI, 0);
              PIN_IN(MISO, 1);
              PIN_IN(SS_N, 1);
              PIN_IN(IO9, 1);
              wdt_enable(WDTO_15MS); // Enable Watchdog Timer to give reset

              while (1);
            }

#ifdef ZWAVE_PROGRAMMER
          //send the button pressed request, if needed:
          if (buttonPressed)
          {
            /* buttons state */
            compl_workbuf[0] = buttonPressed;
            Request(FUNC_ID_BUTTON_PRESSED, compl_workbuf, 1);
            buttonPressed = 0;
          }
#endif
        }
      }
      break;

      case stateFrameParse :
      {
        /* Parse received frame */
        if (!CheckSeqNo(serBuf))
        {
          state = stateIdle;
          break;
        }
        PIN_OFF(SCK_CS);

        switch (serFrameCmd)
        {
          case FUNC_ID_CONNECT_PROGRAMMER:
          {
            compl_workbuf[0] = SIG_BYTE_1;
            compl_workbuf[1] = SIG_BYTE_2;
            Respond(serFrameCmd, compl_workbuf, 2);
          }
          break;

#ifdef BOOT_LOADER
          case FUNC_ID_M128_ENTER_PROG_MODE:
            btSig[0] = serFrameDataPtr[0];
            btSig[1] = serFrameDataPtr[1];
            btSig[2] = serFrameDataPtr[2];
            btSig[3] = serFrameDataPtr[3];
            btSig[4] = serFrameDataPtr[4];
            compl_workbuf[0] = DONE;
            Respond(serFrameCmd, compl_workbuf, 1);
            break;
#endif
#ifdef ZWAVE_PROGRAMMER
          case FUNC_ID_M128_ENTER_PROG_MODE:
            compl_workbuf[0] = DONE;
            Respond(serFrameCmd, compl_workbuf, 1);
            waitCmd = FUNC_ID_M128_ENTER_PROG_MODE;
            break;

          case FUNC_ID_M128_GET_SW_VER:
            compl_workbuf[0] = ZWAVE_PROGRAMMER_VERSION;
            compl_workbuf[1] = ZWAVE_PROGRAMMER_REVISION;
            Respond(serFrameCmd, compl_workbuf, 2);
            break;

          case FUNC_ID_BLOCK_SET_EEP:
          {
            /*Sets a block of EEP to the value supplied
              old eeprom ,StartAdr(MSB) | startAdr(LSB) | endAdr(MSB) | endAdr(LSB) | value to set */
            startAdr.addres1byte = (BYTE)serFrameDataPtr[0];
            startAdr.addres2byte = (BYTE)serFrameDataPtr[1];
            startAdr.addres3byte = (BYTE)serFrameDataPtr[2];
            WORD _startAdr = (WORD)serFrameDataPtr[1];
            _startAdr <<= 8;
            _startAdr |= serFrameDataPtr[2];

            WORD endAdr = (WORD)serFrameDataPtr[3];
            endAdr <<= 8;
            endAdr |= serFrameDataPtr[4];
            NVM_FillArray(startAdr, serFrameDataPtr[5], (endAdr - _startAdr));
            compl_workbuf[0] = DONE;
            Respond( serFrameCmd, compl_workbuf, 1);
          }
          break;
          case FUNC_ID_MEMORY_GET_ID:
          {
            for (int i = 0; i < HOMEID_LENGTH; i++)
            {
              compl_workbuf[i] = i;
            }

            addr.addres1byte = 0x00;
            addr.addres2byte = 0x00;
            addr.addres3byte = (BYTE)EX_EEPROM_HOME_ID;
            NVM_GetArray(addr, compl_workbuf, HOMEID_LENGTH);
            Respond(serFrameCmd, compl_workbuf, HOMEID_LENGTH);
          }
          break;
          case FUNC_ID_STORE_HOMEID:
            /*old eeprom, homeID1|homeID2|homeID3|homeID4*/
            addr.addres1byte = 0x00;
            addr.addres2byte = 0x00;
            addr.addres3byte = (BYTE)EX_EEPROM_HOME_ID;
            NVM_PutArray(addr, &serFrameDataPtr[1], HOMEID_LENGTH);
            compl_workbuf[0] = DONE;
            Respond( FUNC_ID_STORE_HOMEID, compl_workbuf, 1);
            break;

          case FUNC_ID_MEMORY_GET_BYTE:
          {
            addr.addres1byte = 0x00;
            addr.addres2byte = (BYTE)serFrameDataPtr[0];
            addr.addres3byte = (BYTE)serFrameDataPtr[1];
            compl_workbuf[0] = NVM_Get(addr);
            Respond(serFrameCmd, compl_workbuf, 1);
          }
          break;

          case FUNC_ID_MEMORY_PUT_BYTE:
            /* old eeprom, offset(MSB) | offset (LSB) | data */
          {
            BYTE val =  serFrameDataPtr[3];
            addr.addres1byte = (BYTE)serFrameDataPtr[0];
            addr.addres2byte = (BYTE)serFrameDataPtr[1];
            addr.addres3byte = (BYTE)serFrameDataPtr[2];
            NVM_Put(addr, val);
            compl_workbuf[0] = NVM_Get(addr);
            Respond(serFrameCmd, compl_workbuf, 1);
          }
          break;
          case FUNC_ID_MEMORY_GET_BUFFER:
          {
            /* old EEPROM, offset(MSB) | offset (LSB) | length */
            compl_workbuf[0] = serFrameDataPtr[0];
            compl_workbuf[1] = serFrameDataPtr[1];
            compl_workbuf[2] = serFrameDataPtr[2];
            offset.addres1byte = (BYTE)serFrameDataPtr[0];
            offset.addres2byte = (BYTE)serFrameDataPtr[1];
            offset.addres3byte = (BYTE)serFrameDataPtr[2];
            NVM_GetArray(offset, &compl_workbuf[3], serFrameDataPtr[3]);
            Respond(serFrameCmd, compl_workbuf, serFrameDataPtr[3] + 3);
          }
          break;

          case FUNC_ID_MEMORY_PUT_BUFFER:
          {
            /* old EEPROM, adr(MSB) | adr(LSB) | length | buffer[] */
            startAdr.addres1byte = (BYTE)serFrameDataPtr[0];
            startAdr.addres2byte = (BYTE)serFrameDataPtr[1];
            startAdr.addres3byte = (BYTE)serFrameDataPtr[2];
            WORD length = (BYTE)serFrameDataPtr[3];
            NVM_PutArray(startAdr, &serFrameDataPtr[4], length);
            compl_workbuf[0] = DONE;
            Respond(FUNC_ID_MEMORY_PUT_BUFFER, compl_workbuf, 1);
          }
          break;

          case FUNC_ID_ZW_GET_VERSION:
          {
            compl_workbuf[0] = 0x00;
            compl_workbuf[1] = ZWAVE_PROGRAMMER_VERSION;
            compl_workbuf[2] = ZWAVE_PROGRAMMER_REVISION;
            Respond(serFrameCmd, compl_workbuf, 3);
          }
          break;
#endif
          case FUNC_ID_EXIT_PROG_MODE:
            btSig[0] = 0;
            btSig[1] = 0;
            btSig[2] = 0;
            btSig[3] = 0;
            btSig[4] = 0;
            compl_workbuf[1] = DONE;
            Respond(serFrameCmd, compl_workbuf, 1);
            waitCmd = FUNC_ID_EXIT_PROG_MODE;
            break;

#ifdef ZWAVE_PROGRAMMER
            /*Handler of Programmer Enable*/
            /*REQ | Cmd */
            /*RES | Cmd | SYNC */
            /*  SYNC: number of synchronizations trials. If SYNC is 0xFF, then*/
            /* synchronization has failed.*/
          case FUNC_ID_ZW0x0x_PROG_ENABLE:
          {
            /*We assume that the chip is not in APM mode or it is not a 500 series chip*/
            isInAutoProgMode = FALSE;
            /*ready yhe reset_n signal*/
            PIN_OUT(RESET_N);
            Reset_N_High();
            /*if the chip is 500 then we need to check if the APM lock bit is set by checking the FSMBusy bit
              Fix TO*#5455
              Enable Programming mode function flow (ZW05x only)
              1. Set RESET_N low (Done in ZW0x0xProgEnable function)
              2. send Enable Interface command
              3. Send Read Sig command
              4. Send Read State Command
              5. If FSMBusy bit is set then set RESET_N high and got to step 2
             */
            if (ZW0x0xProgEnable())
            {
              ReadSignatureBits(0, &compl_workbuf[1]);
              /*we have seven sig bytes the last 3 bytes are the vendor id, chiptype id, and revision nr.*/
              /*500 series chiptype id = 0x04, current revision is 0x01*/
              if((compl_workbuf[5] == 0x1F) && (compl_workbuf[6] == 0x04))
              {
                 /*the FSMBusy bit is true which means that APM mode is enabled,
                    then we should set reset_n high and try to read to re_enable the interface again*/
                if (CheckState(ZW050x) == STATE_WRITE_BUSY )
                {
                  Reset_N_High();
                  isInAutoProgMode = TRUE;
                  ProgEnable(ZW050x);
                  ReadSignatureBits(0, &compl_workbuf[1]);
                /*
                  the FSMbusy bit should be false when interface enable command is send wihtout assreting the reset_n
                  if it still true then something is totally wrong and we need to indicate that enable interface command failed
                */
                  if (CheckState(ZW050x) == STATE_WRITE_BUSY)
                  {
                    compl_workbuf[0] = MAX_SYNC; /*indicate that the programming interface enable operation failed.*/
                  }
                }


              }
            }
            Respond(serFrameCmd, compl_workbuf, 1);
            Reset_N_Low(); /*?? don't know why reset_n is set low*/
          }
          break;

          case FUNC_ID_ZW0x0x_CHIP_ERASE:
          {
            compl_workbuf[0] = ChipErase(serFrameDataPtr[0], serFrameDataPtr[1]);
            Respond(serFrameCmd, compl_workbuf, 1);
          }
          break;


          case FUNC_ID_ZW0x0x_CHECK_STATE:
          {
            compl_workbuf[0] = CheckState(serFrameDataPtr[0]);
            Respond(serFrameCmd, compl_workbuf, 1);
          }
          break;

          case FUNC_ID_ZW050x_SET_NVR:
          {
            SPI_Exchange(0xFE);
            SPI_Exchange(0);
            SPI_Exchange(serFrameDataPtr[0]);
            SPI_Exchange(serFrameDataPtr[1]);
            compl_workbuf[0] = SUCCESS;
            Respond(serFrameCmd, compl_workbuf, 1);
          }
          break;

          case FUNC_ID_ZW050x_READ_NVR:
          {
            SPI_Exchange(0xF2);
            SPI_Exchange(serFrameDataPtr[0]);
            SPI_Exchange(serFrameDataPtr[1]);
            compl_workbuf[0] =  SPI_Exchange(0);
            Respond(serFrameCmd, compl_workbuf, 1);
          }
          break;

          case FUNC_ID_ZW050x_RESET:
          {
            SPI_Exchange(0xFF);
            SPI_Exchange(0xFF);
            SPI_Exchange(0xFF);
            SPI_Exchange(0xFF);
            compl_workbuf[0] = SUCCESS;
            Respond(serFrameCmd, compl_workbuf, 1);
          }
          break;
          case FUNC_ID_ZW050x_GET_PROG_MODE:
          {
            if(isInAutoProgMode==TRUE)
              compl_workbuf[0] = 0x01;
            else
              compl_workbuf[0] = 0x00;
            Respond(serFrameCmd, compl_workbuf, 1);
          }
          break;
          case FUNC_ID_ZW0x0x_READ_PAGE:
          {
            Respond(serFrameCmd,
                    compl_workbuf,
                    ReadPage(serFrameDataPtr[0], serFrameDataPtr[1], compl_workbuf)
                   );
          }
          break;

          case FUNC_ID_ZW0x0x_WRITE_PAGE:
          {
            Reset_N_Low();
            DelayMs(1);

            if(serBuf[4]==ZW050x){
                compl_workbuf[0] = WritePage(serBuf[4],  //   BYTE chipType,
                                             serBuf[5],  //    BYTE pageNo, /*The page address*/
                                             serBuf[6],  //    BYTE verify,
                                             ((((WORD)serBuf[7]) << 8) | serBuf[8]), //    WORD startAddr,
                                             ((((WORD)serBuf[9]) << 8) | serBuf[10]), //    WORD stopAddr,
                                             ((((WORD)serBuf[0]) << 8) | serBuf[1]) - 11, //        WORD dataLength,
                                             (serBuf + 11) //              BYTE *data /*Data to be ritten to page Addr*/
                                            );

            }
            else{

                compl_workbuf[0] = WritePage(serFrameDataPtr[0],  //   BYTE chipType,
                                             serFrameDataPtr[1],  //    BYTE pageNo, The page address
                                             serFrameDataPtr[2],  //    BYTE verify,
                                             0, //    WORD startAddr,
                                             0, //    WORD stopAddr,
                                             ((((WORD)serBuf[0]) << 8) | serBuf[1]) - 7, //        WORD dataLength,
                                             (serBuf + 7) //              BYTE *data Data to be ritten to page Addr
                                            );

            }
            Respond(serFrameCmd, compl_workbuf, 1);

          }
          break;

          /*Handler of SRAM page read command*/
          /*REQ | cmd | ChipType | PageNo */
          /*RES | cmd | SRAM data[] */
          case FUNC_ID_ZW0x0x_SRAM_READ_PAGE:
          {
            Respond(serFrameCmd,
                    compl_workbuf,
                    SRAMReadPage(serFrameDataPtr[0],
                                 serFrameDataPtr[1],
                                 compl_workbuf
                                )
                   );
            break;
          }

          /*Handler of SRAM page write command*/
          /*REQ | cmd | ChipType | PageNo | IsVerify | SRAM data[] */
          /*RES | cmd | Status */
          /*   Status = SUCCESS or FAIL*/
          case FUNC_ID_ZW0x0x_SRAM_WRITE_PAGE:
          {
            compl_workbuf[0] = SRAMWritePage(serFrameDataPtr[0],
                                             serFrameDataPtr[1],
                                             serFrameDataPtr[2],
                                             ((((WORD)serBuf[0]) << 8) | serBuf[1]) - 7, //data length
                                             &serFrameDataPtr[3]
                                            );
            Respond(serFrameCmd, compl_workbuf, 1);
            break;
          }

          /*Handler of Enable Exexute Out of SRAM mode*/
          /*REQ | cmd | ChipType */
          /*RES | cmd | Status */
          /*   Status = SUCCESS or FAIL*/
          case FUNC_ID_ZW0x0x_SRAM_EXECUTE:
          {
            if (serFrameDataPtr[0] == ZW050x)
            {
              compl_workbuf[0] = ExecuteOutOfSRAM_500(serFrameDataPtr[0]);
            }
            else
            {
              compl_workbuf[0] = ExecuteOutOfSRAM(serFrameDataPtr[0]);
            }
            Respond(serFrameCmd, compl_workbuf, 1);

            break;
          }

          /*Handler of Modem bit write*/
          /*REQ | cmd | ChipType */
          /*RES | cmd | Status */
          /*   Status = SUCCESS or FAIL*/
          case FUNC_ID_ZW0x0x_MODEM_BIT_WRITE:
          {
            compl_workbuf[0] = SetModemBits(serFrameDataPtr[0]);
            Respond(serFrameCmd, compl_workbuf, 1);
            break;
          }

          /*Handler of Status Byte Read*/
          /*REQ | cmd | ChipType */
          /*RES | cmd | StatusByte */
          /*   StatusByte = Byte wit bits STATE_CRC_BUSY, STATE_CRC_DONE, */
          /*     STATE_CRC_FAILED, STATE_WRITE_BUSY,STATE_WRITE_FAILED, */
          /*     STATE_CONTINUE_FEFUSED, STATE_DEV_MODE_ENABLED, STATE_EXEC_SRAM_MODE_ENABLED */
          case FUNC_ID_ZW0x0x_STATUS_READ:
          {
            compl_workbuf[0] = CheckState(serFrameDataPtr[0]);
            Respond(serFrameCmd, compl_workbuf, 1);
            break;
          }

          /*Handler of Write OTP Stats Read*/
          /*REQ | cmd | ChipType */
          /*RES | cmd | WriteOTPStats(MSB) | WriteOTPStats(LSB) */
          /*   WriteOTPStats: count of excessive writes */
          case FUNC_ID_ZW0x0x_WRITE_OTP_STATS_READ:
          {
            WORD num = ReadWriteOtpStats(serFrameDataPtr[0]);
            compl_workbuf[0] = (BYTE)(num >> 8);
            compl_workbuf[1] = (BYTE)(num >> 0);
            Respond(serFrameCmd, compl_workbuf, 2);
            break;
          }

          /*Handler of Enable Development mode*/
          /*REQ | cmd | ChipType */
          /*RES | cmd | Status */
          /*   Status = SUCCESS or FAIL*/
          case FUNC_ID_ZW0x0x_DEV_MODE_ENABLE:
          {
            compl_workbuf[0] = EnableDevMode(serFrameDataPtr[0]);
            Respond(serFrameCmd, compl_workbuf, 1);
            break;
          }

          /*Handler of CRC Check Run*/
          /*REQ | cmd | ChipType */
          /*RES | cmd | Status */
          /*   Status = SUCCESS or FAIL*/
          case FUNC_ID_ZW0x0x_RUN_CRC:
          {
            compl_workbuf[0] = CRCCheck(serFrameDataPtr[0]);
            Respond(serFrameCmd, compl_workbuf, 1);
            break;
          }

          case FUNC_ID_ZW0x0x_ERASE_PAGE:
          {
            compl_workbuf[0] = ErasePage(serFrameDataPtr[0], serFrameDataPtr[1]);
            Respond(serFrameCmd, compl_workbuf, 1);
          }
          break;

          /*Handler of Read Lock Bits*/
          /*REQ | cmd | ChipType */
          /*RES | cmd | LockBits */
          case FUNC_ID_ZW0x0x_READ_LOCKBITS:
          {
            compl_workbuf[0] = ReadLockBits(serFrameDataPtr[0]);
            Respond(serFrameCmd, compl_workbuf, 1);
            break;
          }

          case FUNC_ID_ZW050x_READ_LOCKBITS:
          {
            compl_workbuf[0] = ReadLockBits500(serFrameDataPtr[0],serFrameDataPtr[1]);
            Respond(serFrameCmd, compl_workbuf, 1);
            break;
          }

          /*Handler of Write Lock Bits*/
          /*REQ | cmd | ChipType | LockBits*/
          /*RES | cmd | Status */
          /*   Status = SUCCESS or FAIL*/
          case FUNC_ID_ZW0x0x_WRITE_LOCKBITS:
          {
            compl_workbuf[0] = SetLockBits(serFrameDataPtr[0], serFrameDataPtr[1]);
            Respond(serFrameCmd, compl_workbuf, 1);
            break;
          }

          case FUNC_ID_ZW050x_WRITE_LOCKBITS:
          {
            compl_workbuf[0] = SetLockBits500(serFrameDataPtr[0], serFrameDataPtr[1],serFrameDataPtr[2]);
            Respond(serFrameCmd, compl_workbuf, 1);
            break;
          }

          case FUNC_ID_ZW0x0x_SET_WRITE_CYCLE:
          {
            SendWriteCycleTime(serFrameDataPtr[0]);
            compl_workbuf[0] = DONE;
            Respond(serFrameCmd, compl_workbuf, 1);
          }
          break;

          case FUNC_ID_ZW0x0x_READ_SIG_BYTE:
          {
            ReadSignatureBits(serFrameDataPtr[0], compl_workbuf);
            Respond(serFrameCmd, compl_workbuf, 7);
          }
          break;

          /*Handler of Programmer Release*/
          /*REQ | cmd | ChipType */
          /*RES | cmd | DONE */
          /*   DONE = DONE constant*/
          case FUNC_ID_ZW0x0x_PROG_RELEASE:
           if(!isInAutoProgMode)
           {

             PIN_IN(MOSI, 0);
             PIN_IN(SCK, 0);
             PIN_IN(MISO, 0);

            /*we release the ASIC from the reset stat provided that the ASIC will not use the SPI signals as output*/
             switch (serFrameDataPtr[0])
             {
               case ZW010x:
                 PROG_VPP_HIGH;
                 Reset_N_Low();
                 DelayMs(10);
                 Reset_N_High();
                 PROG_VPP_LOW;
                 break;

               case ZW020x:
               case ZW030x:
                 PROG_VPP_LOW;
                 PIN_OUT(PROG_VPP);
                 Reset_N_High();
                 DelayMs(1);
                 DelayUS(400);
                 Reset_N_Low();
                 DelayMs(1);
                 DelayUS(400);
                 Reset_N_High();
                 break;
               case ZW040x:
                 PROG_VPP_LOW;
                 PIN_OUT(PROG_VPP);
                 Reset_N_High();
                 break;
             }

           }
           isInAutoProgMode=TRUE;
           PIN_IN(RESET_N, 0);
           //Disable EEPROM Interface if any:
           DisableFlashInterfaceButStayInProgrammingMode(FALSE);
           compl_workbuf[0] = DONE;
           Respond(serFrameCmd, compl_workbuf, 1);

           break;


          case FUNC_ID_CALIBRATION_START:
            /*we release the ASIC from the reset stat provided that the ASIC will not use the SPI signals as output*/
            switch (serFrameDataPtr[0])
            {
              case ZW040x:
              case ZW050x:
                  CAL_EN_LOW;
                break;
            }
            compl_workbuf[0] = DONE;
            Respond(serFrameCmd, compl_workbuf, 1);
            break;

          case FUNC_ID_CALIBRATION_STOP:
            /*we release the ASIC from the reset stat provided that the ASIC will not use the SPI signals as output*/
            switch (serFrameDataPtr[0])
            {
              case ZW050x:
                  CAL_EN_HIGH;
                break;
            }
            compl_workbuf[0] = DONE;
            Respond(serFrameCmd, compl_workbuf, 1);
            break;
#endif

          case FUNC_ID_PROGRAMMER_SET_LED:
          {
            if (serFrameDataPtr[0] == PASS_LED)/*LED name*/
            {
              PIN_ON(LEDB);
            }
            else if (serFrameDataPtr[0] == BUSY_LED)/*LED name*/
            {
              if (serFrameDataPtr[1])
              {
                PIN_ON(LEDC);
              }
              else
              {
                PIN_OFF(LEDC);
              }
            }
            else if (serFrameDataPtr[0] == ERR_LED)/*LED name*/
            {
              PIN_ON(LEDD);
            }

            state  = stateIdle;

          }
          break;
#ifdef ZWAVE_PROGRAMMER
          /*Handler of Toggle NVM Programming Interface*/
          /*REQ | cmd | IsEnable | ChipType */
          /*RES | cmd | BusType | ManufacturerID | DeviceID | SizeID */
          /*   BusType = ManufacturerID = DeviceID = SizeID = 0 if error*/
          /* WARNING! EEPROM interface should be possible to enable without
                   *  entering the programming mode!
                   *  !!! This is only for backward compatibility with older
                   *  Z-Wave Programmers (v2.21 and older) which
                   *  sends FUNC_ID_ZW0x0x_PROG_RELEASE before enabling EEPROM interface !!!
                   */
          case FUNC_ID_TOGGLE_EEPROM_IF:
          {
            DWORD nvm_type = NVMInvalid;

            if (serFrameDataPtr[0])/*enable EEPROM interface*/
            {
              SS_N_HIGH;
              MOSI_LOW;
              SCK_LOW;

              PIN_OUT(SS_N);
              PIN_OUT(MOSI);
              PIN_OUT(SCK);

              //  !!! This is only for backward compatibility with older
              //  Z-Wave Programmers (v2.21 and older) which
              //  sends FUNC_ID_ZW0x0x_PROG_RELEASE before enabling EEPROM interface !!!
              // In new Z-Wave Programmers, eeprom interface can only be enabled
              //  after entering the programming mode.
              PIN_OUT(RESET_N);
              Reset_N_Low();
              // end of code for compatibility.

              //Disable the flash interface:
              // Flash (OTP) interface is disabled by continuous toggling the
              // reset_n signal of the ZW0x0x chip at frequency about 3.6 KHz.
              // This lead to constantly reseting the ZW0x0x chip preventing him
              // to enter the programming mode and also to start executing
              // the firmware in ZW0x0x chip.
              DisableFlashInterfaceButStayInProgrammingMode(TRUE);


              //Initialization of the NVM interface:
              switch (serFrameDataPtr[1])
              {
                case ZW010x:
                  //Reset_N_Low();
                  nvm_type = NVM_Init(&SS_NPort, (1 << SS_N));
                  break;

                case ZW020x:
                case ZW030x:
                case ZW040x:
                case ZW050x:
                  nvm_type = NVM_Init(&SS_NPort, (1 << SS_N));
                  break;

                default:
                  nvm_type = NVMInvalid;
              }
            }
            else
            {
              /*SPI signals*/
              //!!WARNING do not remove SPI signals from the chip if chip in
              //  programming mode (reset=LOW), because this can produce
              //  glitches on SPI bus and we can lost synchronization with chip.
              //PIN_IN(MOSI,0);
              //PIN_IN(SCK,0);

              PIN_IN(SS_N, 1);
              DisableFlashInterfaceButStayInProgrammingMode(FALSE);

              PROG_VPP_LOW;
              PIN_OUT(PROG_VPP);
              nvm_type = NVMUnknown;
            }

            compl_workbuf[0] = (nvm_type >> 24) & 0xff;
            compl_workbuf[1] = (nvm_type >> 16) & 0xff;
            compl_workbuf[2] = (nvm_type >>  8) & 0xff;
            compl_workbuf[3] = (nvm_type >>  0) & 0xff;
            Respond(serFrameCmd, compl_workbuf, 4);
            break;
          }
#endif

#ifdef ZWAVE_PROGRAMMER
          /*Handler of MTP Interface toggle command*/
          /*REQ | cmd | IsEnable | ChipType */
          /*RES | cmd | Status */
          /*   Status = SUCCESS or FAIL*/
          case FUNC_ID_TOGGLE_MTP_IF:
          {
            BYTE result = FAIL;

            if (serFrameDataPtr[0])/*enable MTP interface*/
            {
              //Disable EEPROM Interface if any:
              DisableFlashInterfaceButStayInProgrammingMode(FALSE);

              //Disable Flash/OTP Interface Interface:
              mtpIfEn = FALSE;

              if (serFrameDataPtr[1] == ZW040x)
              {
                //Upload the MTP Handler to the ZW040x chip:
                //Go to the programming mode:
                if (ZW0x0xProgEnable())
                {
                  //Save current working mode:
                  MtpHandlerOldStateByte = CheckState(ZW040x);
                  //Save current contents of the SRAM:
                  SRAMReadWrite(TRUE, MtpHandlerOldSRAM, MTP_HANDLER_OLD_SRAM_SIZE, FALSE);

                  //Upload MTP Handler code to the SRAM of the ZW040x chip by pages:
                  if (SRAMReadWrite(FALSE, (BYTE*)MtpHandler, MTPHANDLER_LENGTH, TRUE))
                  {
                    //Set the "Execution out of SRAM" working mode of chip:
                    if (ExecuteOutOfSRAM(ZW040x) == SUCCESS)
                    {
                      //Disable Flash/OTP Interface:
                      Reset_N_Low();
                      PIN_OUT(RESET_N);
                      PROG_VPP_LOW;
                      PIN_OUT(PROG_VPP);
                      PIN_IN(MOSI, 0);
                      PIN_IN(SCK, 0);
                      PIN_IN(MISO, 0);


                      //Run chip in "Execution out of SRAM" working mode:
                      Reset_N_Low();
                      PIN_OUT(RESET_N);
                      DelayMs(10);
                      Reset_N_High();
                      DelayMs(10);

                      //Enabling MTP Interface:
                      MOSI_LOW;
                      SCK_LOW;
                      PIN_OUT(MOSI);
                      PIN_OUT(SCK);
                      PIN_IN(MISO, 0);
                      SPI_Init(&PORTD, &PIND, (1 << MOSI), (1 << MISO), (1 << SCK));

                      //Wait when MTP interface will be ready:
                      DelayUS(MTP_INTERFACE_READY_DELAY_US);

                      mtpIfEn = TRUE;
                      result = SUCCESS;
                    }
                  }
                }
              }
            }
            else
            {
              if (mtpIfEn && serFrameDataPtr[1] == ZW040x)
              {
                result = FAIL;
                //Enable Flash/OTP Interface and disable MTP Interface:
                Reset_N_Low();
                PIN_OUT(RESET_N);
                PROG_VPP_LOW;
                PIN_OUT(PROG_VPP);
                //!!WARNING do not remove SPI signals from the chip if chip in
                //  programming mode (reset=LOW), because this can produce
                //  glitches on SPI bus and we can lost synchronization with chip.
                //PIN_IN(MOSI, 0);
                //PIN_IN(SCK, 0);
                //PIN_IN(MISO, 0);
                //Go to the programming mode:
                if (ZW0x0xProgEnable())
                {
                  //Restore the old contents of the SRAM:
                  if (SRAMReadWrite(FALSE, MtpHandlerOldSRAM, MTP_HANDLER_OLD_SRAM_SIZE, TRUE))
                  {
                    //Restore the old working mode:
                    if (MtpHandlerOldStateByte & STATE_EXEC_SRAM_MODE_ENABLED)
                    {
                      if (ExecuteOutOfSRAM(ZW040x) == SUCCESS)
                      {
                        result = SUCCESS;
                      }
                    }
                    else if (MtpHandlerOldStateByte & STATE_DEV_MODE_ENABLED)
                    {
                      if (EnableDevMode(ZW040x) == SUCCESS)
                      {
                        result = SUCCESS;
                      }
                    }
                    else
                    {
                      //FAIL: Normal working mode can't be enabled (Dev or ExecOutOfSRAM mode can't be disabled) by SPI Commands - only by power cycling.
                    }
                  }
                }
              }
              else
              {
                result = SUCCESS;
              }

              //Disable Flash/OTP Interface and MTP Interface:
              Reset_N_Low();
              PIN_OUT(RESET_N);
              PROG_VPP_LOW;
              PIN_OUT(PROG_VPP);
              //!!WARNING do not remove SPI signals from the chip if chip in
              //  programming mode (reset=LOW), because this can produce
              //  glitches on SPI bus and we can lost synchronization with chip.
              //PIN_IN(MOSI, 0);
              //PIN_IN(SCK, 0);
              //PIN_IN(MISO, 0);

              mtpIfEn = FALSE;

              //Disable EEPROM Interface if any:
              PIN_IN(SS_N, 1);
              DisableFlashInterfaceButStayInProgrammingMode(FALSE);
            }

            compl_workbuf[0] = result;
            Respond(serFrameCmd, compl_workbuf, 1);
            break;
          }

          /*Handler of MTP fill by value command*/
          /*REQ | cmd | ChipType | FillValue */
          /*RES | cmd | Status */
          /*   Status = SUCCESS or FAIL*/
          case FUNC_ID_MTP_FILL:
          {
            if (mtpIfEn)
            {
              compl_workbuf[0] = MTPFill(serFrameDataPtr[0], serFrameDataPtr[1]);
            }
            else
            {
              compl_workbuf[0] = FAIL;
            }

            Respond(serFrameCmd, compl_workbuf, 1);
            break;
          }

          /*Handler of MTP page read command*/
          /*REQ | cmd | ChipType | PageNo */
          /*RES | cmd | MTP data[] */
          case FUNC_ID_MTP_READ_PAGE:
          {
            //MTP PageNo is not used.
            if (mtpIfEn)
            {
              Respond(serFrameCmd, compl_workbuf,
                      MTPRead(serFrameDataPtr[0], compl_workbuf));
            }
            else
            {
              Respond(serFrameCmd, NULL, 0);
            }

            break;
          }

          /*Handler of MTP page write command*/
          /*REQ | cmd | ChipType | PageNo | IsVerify | MTP data[] */
          /*RES | cmd | Status */
          /*   Status = SUCCESS or FAIL*/
          case FUNC_ID_MTP_WRITE_PAGE:
          {
            //MTP PageNo is not used.
            if (mtpIfEn)
            {
              compl_workbuf[0] = MTPWrite(serFrameDataPtr[0], serFrameDataPtr[2], &serFrameDataPtr[3]);
            }
            else
            {
              compl_workbuf[0] = FAIL;
            }

            Respond(serFrameCmd, compl_workbuf, 1);
            break;
          }
#endif

#ifdef BOOT_LOADER
          case FUNC_ID_M128_CHIP_ERASE:
          {
            EraseAtmel();
          }
          break;
          case FUNC_ID_M128_BLOCK_WRITE:
          {
            int pageNo = (serFrameDataPtr[0] << 8) | serFrameDataPtr[1];

            compl_workbuf[0] = BlockLoad(serFrameDataPtr[2], BLOCKSIZE, 'F', pageNo );
            Respond(serFrameCmd, compl_workbuf, 1);
          }
          break;

          case FUNC_ID_M128_BLOCK_READ:
          {
            int pageNo = (serFrameDataPtr[0] << 8) | serFrameDataPtr[1];
            BlockRead(BLOCKSIZE, 'F', pageNo); // Block read
          }
          break;

#if 0
          case FUNC_ID_M128_SET_LOCKBITS:
          {

            boot_lock_bits_set(serFrameDataPtr[0]);  // boot.h takes care of mask
            boot_spm_busy_wait();
            compl_workbuf[0] = DONE;
            Respond(serFrameCmd, compl_workbuf, 1);
          }
          break;
          case FUNC_ID_M128_GET_LOCKBITS:
          {
            boot_spm_busy_wait();
            compl_workbuf[0] = read_fuse_lock(0x0001, _BV(BLBSET) | _BV(SPMEN));
            Respond(serFrameCmd, compl_workbuf, 1);
          }
          break;

          case FUNC_ID_M128_GET_FUSEBITS_LOW:
          {
            boot_spm_busy_wait();
            compl_workbuf[0] = read_fuse_lock(0x0000, _BV(BLBSET) | _BV(SPMEN));
            Respond(serFrameCmd, compl_workbuf, 1);
          }
          break;

          case FUNC_ID_M128_GET_FUSEBITS_HIGH:
          {
            boot_spm_busy_wait();
            compl_workbuf[0] =  read_fuse_lock(0x0003, _BV(BLBSET) | _BV(SPMEN));
            Respond(serFrameCmd, compl_workbuf, 1);
          }
          break;
          case FUNC_ID_M128_GET_FUSEBITS_EXT:
          {
            boot_spm_busy_wait();
            compl_workbuf[0] = read_fuse_lock(0x0002, _BV(BLBSET) | _BV(SPMEN));
            Respond(serFrameCmd, compl_workbuf, 1);
          }
          break;
#endif
#endif

          default :
          {
            state = stateIdle;
          }
          break;
        }
      }
      break;
      case stateTxSerial :
      {
        /* Wait for ack on send respons. Retransmit as needed */
        BYTE conVal = ConUpdate(FALSE);   /* FALSE: Do not ack/nak frames received while waiting for ack */

        if (conVal != conIdle)
        {
          if (conVal == conFrameSent)
          {
            retry = 0;
            state = stateIdle;
          }
          else  /* Then it must be a timeout */
          {
            //if (conVal == conTxTimeout)
            {
              //if (retry++ < MAX_SERIAL_RETRY)
              //{
              //  ConTxFrame(0, REQUEST, NULL, 0);  // Retry...
              //}
              //else
              {
                retry = 0;
                state = stateIdle;
              }
            }
          }
        }
      }
      break;
    }
  } // flashProg aapl
} // end: main




#ifdef BOOT_LOADER
unsigned char BlockLoad(BYTE verify, unsigned int size, unsigned char mem, uint16_t pageNo)
{
  uint16_t data, i = 3;
  long addr = pageNo * size;
  WORD size_t = size;
  long tempaddress;
  BYTE ret = SUCCESS;
  BYTE tmp = 0xFF ;
  BYTE sreg;
  sreg = SREG;
  cli();

  tmp ^= btSig[0];
  tmp ^= btSig[1];
  tmp ^= btSig[2];
  tmp ^= btSig[3];
  tmp ^= btSig[4];

  if (tmp) // bootLoader signature is not correct abort tha ATMEL programming
  {
    return FAIL;
  }


  my_eeprom_busy_wait();
  // EEPROM memory type.
#if 0

  if (mem == 'E')
  {
    /* Fill buffer first, as EEPROM is too slow to copy with UART speed */

    /* Then program the EEPROM */
    for ( tempaddress = 0; tempaddress < size; tempaddress++)
    {
      EEARL = addr; // Setup EEPROM address
      EEARH = ((*addr) >> 8);
      EEDR = serFrameDataPtr[tempaddress + 3]; // Get byte.
      EECR |= (1 << EEMWE); // Write byte.
      EECR |= (1 << EEWE);

      while (EECR & (1 << EEWE)) // Wait for write operation to finish.
        ;

      (*addr)++; // Select next EEPROM byte
    }

    return '\r'; // Report programming OK
  }
  // Flash memory type.
  else if (mem == 'F')
#endif
  {
    // NOTE: For flash programming, 'address' is given in words.
    tempaddress = addr;  // Store address in page.
    //boot_page_erase (addr);
    //boot_spm_busy_wait ();
    do
    {
      data = serFrameDataPtr[i++ ];
      data += (serFrameDataPtr[i++ ] << 8);
      boot_page_fill(addr, data);
      addr += 2; // Select next word in memory.
      size_t -= 2; // Reduce number of bytes to write by two.
    }
    while (size_t); // Loop until all bytes written.

    boot_page_write(tempaddress);
    boot_spm_busy_wait();
    boot_rww_enable();        //Re-enable the RWW section

    if (verify)
    {
      addr = tempaddress;
      size_t = size;

      i = 3; // flash data start at offfset 3

      do
      {
        data = serFrameDataPtr[i++ ];
        data += (serFrameDataPtr[i++ ] << 8);

        if (data != pgm_read_word_far(addr))
        {
          ret = FAIL;
          break;
        }

        addr += 2; // Select next word in memory.
        size_t -= 2; // Subtract two bytes from number of bytes to read
      }
      while (size_t); // Repeat until all block has been read
    }
  }
  SREG = sreg;
  sei();
  return ret; // Report programming OK
}


void BlockRead(unsigned int size, unsigned char mem, uint16_t pageNo)
{
  // EEPROM memory type.
  int i = 0;
  uint16_t data;
  long addr = pageNo * size;

  my_eeprom_busy_wait();
#if 0

  if (mem == 'E') // Read EEPROM
  {

    do
    {
      EEARL = *addr; // Setup EEPROM address
      EEARH = ((*addr) >> 8);
      (*addr)++; // Select next EEPROM byte
      EECR |= (1 << EERE); // Read EEPROM
      compl_workbuf[i++] = EEDR; // Transmit EEPROM dat ato PC
      size--; // Decrease number of bytes to read
    }
    while (size); // Repeat until all block has been read
  }
  // Flash memory type.
  else if (mem == 'F')
#endif
  {

    do
    {
      data = pgm_read_word_far(addr);
      compl_workbuf[i++] =  (BYTE)data ;
      compl_workbuf[i++] =  (BYTE)(data >> 8);
      (addr) += 2; // Select next word in memory.
      size -= 2; // Subtract two bytes from number of bytes to read
    }
    while (size); // Repeat until all block has been read
  }
  Respond(serFrameCmd, compl_workbuf, i);

}
#endif

/* end of file */
