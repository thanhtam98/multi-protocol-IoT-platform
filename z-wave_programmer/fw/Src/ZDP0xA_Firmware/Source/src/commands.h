/*************************  commands.h  *******************************
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
 * Description: This header file contains definitions of the commands of the
 *              Z-Wave Programmer Communication Protocol.
 *
 * Author:      Valeriy Vyshnyak
 *
 * Last Changed By:  $Author: vvi $
 * Revision:         $Revision: 0000 $
 * Last Changed:     $Date: 2009-11-11 $
 *
 ****************************************************************************/

#ifndef	_COMMANDS_H_

#define TIME_3_SEC                                  300

#define MAX_SERIAL_RETRY                            3

#define RESPONSE                                    0x01
#define REQUEST                                     0x00

#define    FUNC_ID_BLOCK_SET_EEP                    0x05
#define    FUNC_ID_ZW_GET_VERSION                   0x06
#define    FUNC_ID_MEMORY_GET_ID                    0x07
#define    FUNC_ID_MEMORY_GET_BYTE                  0x08
#define    FUNC_ID_MEMORY_PUT_BYTE                  0x09
#define    FUNC_ID_MEMORY_GET_BUFFER                0x10
#define    FUNC_ID_MEMORY_PUT_BUFFER                0x11
#define    FUNC_ID_STORE_HOMEID                     0x13

#define    FUNC_ID_M128_CHIP_ERASE                  0x16
#define    FUNC_ID_M128_GET_BLOCK_SIZE              0x17
#define    FUNC_ID_M128_BLOCK_WRITE                 0x18
#define    FUNC_ID_M128_BLOCK_READ                  0x19
#define    FUNC_ID_M128_GET_LOCKBITS                0x20
#define    FUNC_ID_M128_GET_FUSEBITS_LOW            0x21
#define    FUNC_ID_M128_GET_FUSEBITS_HIGH           0x22
#define    FUNC_ID_M128_GET_FUSEBITS_EXT            0x23
#define    FUNC_ID_EXIT_PROG_MODE                   0x24
#define    FUNC_ID_CONNECT_PROGRAMMER               0x25
#define    FUNC_ID_M128_GET_SW_VER                  0x26
#define    FUNC_ID_M128_ENTER_PROG_MODE             0x27

#define    FUNC_ID_ZW0x0x_PROG_ENABLE               0x40
#define    FUNC_ID_ZW0x0x_CHIP_ERASE                0x41
#define    FUNC_ID_ZW0x0x_READ_PAGE                 0x42
#define    FUNC_ID_ZW0x0x_WRITE_PAGE                0x43
#define    FUNC_ID_ZW0x0x_READ_LOCKBITS             0x44
#define    FUNC_ID_ZW0x0x_WRITE_LOCKBITS            0x45
#define    FUNC_ID_ZW0x0x_SET_WRITE_CYCLE           0x46
#define    FUNC_ID_ZW0x0x_READ_SIG_BYTE             0x47
#define    FUNC_ID_ZW0x0x_PROG_RELEASE              0x48
#define    FUNC_ID_TOGGLE_EEPROM_IF                 0x51
#define    FUNC_ID_ZW0x0x_ERASE_PAGE                0x52
#define    FUNC_ID_PROGRAMMER_SET_LED               0x53
#define    FUNC_ID_ZW050x_WRITE_LOCKBITS 0x54
#define    FUNC_ID_ZW050x_READ_LOCKBITS 0x55


/* ZW040x specific commands definitions: */
#define FUNC_ID_ZW0x0x_SRAM_READ_PAGE               0x60
#define FUNC_ID_ZW0x0x_SRAM_WRITE_PAGE              0x61
#define FUNC_ID_ZW0x0x_SRAM_EXECUTE                 0x62
#define FUNC_ID_ZW0x0x_MODEM_BIT_WRITE              0x63
#define FUNC_ID_ZW0x0x_STATUS_READ                  0x64
#define FUNC_ID_ZW0x0x_DEV_MODE_ENABLE              0x65
#define FUNC_ID_ZW0x0x_RUN_CRC                    0x66

/* button state: */
#define FUNC_ID_BUTTON_PRESSED                      0x67

/* ZW040x specific commands definitions for MTP memory: */
#define FUNC_ID_TOGGLE_MTP_IF                       0x68
#define FUNC_ID_MTP_FILL                            0x69
#define FUNC_ID_MTP_READ_PAGE                       0x6A
#define FUNC_ID_MTP_WRITE_PAGE                      0x6B

/* ZW040x specific commands definitions 2: */
#define FUNC_ID_ZW0x0x_WRITE_OTP_STATS_READ         0x6C

/* ZW040x specific commands definitions for Calibrations: */
#define FUNC_ID_TOGGLE_CALIBRATION_IF               0x6D
#define FUNC_ID_CALIBRATION_RUN                     0x6E
#define FUNC_ID_CALIBRATION_GET_STATUS              0x6F
#define FUNC_ID_CALIBRATION_READ_VALUE              0x70

#define FUNC_ID_CALIBRATION_START					0x71
#define FUNC_ID_CALIBRATION_STOP					0x72

#define    FUNC_ID_ZW050x_GET_PROG_MODE    0xFF - 4
#define    FUNC_ID_ZW0x0x_CHECK_STATE    0xFF - 3
#define    FUNC_ID_ZW050x_SET_NVR    0xFF - 2
#define    FUNC_ID_ZW050x_READ_NVR   0xFF - 1
#define    FUNC_ID_ZW050x_RESET      0xFF

#define    DONE                                     '\r'
#define    SUCCESS                                  '1'
#define    FAIL                                     '0'



/* indicate that preprocessor result is included */
#define	_COMMANDS_H_
#endif
