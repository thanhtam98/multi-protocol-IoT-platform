/*******************************  mtp.h  **************************************
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

#ifndef MTP_H
#define MTP_H

#define MTP_INTERFACE_READY_DELAY_US        170
#define MTP_SIZE                            64

/* Read MTP Reg file byte. Read data from MTP Reg file at address Addr0(3_BYTE) */
/* MOSI    07h   00h   Addr0    00h     */
/* MISO    XXh   70h    00h    DataOut  */
#define READ_MTP_REG_1_BYTE   					    0x07
#define READ_MTP_REG_2_BYTE   					    0x00
#define READ_MTP_REG_2_BYTE_OUT					    0x70
#define READ_MTP_REG_3_BYTE_OUT					    0x00
#define READ_MTP_REG_DURATION_US            2       // wait here specified time before sending next bytes
#define READ_MTP_REG_4_BYTE   					    0x00

/* Continue Read MTP. The Command can follow a “Read MTP” command. */
/* The command will read the next three memory locations from the MTP. */
/* MOSI    B5h   00h    00h     00h     */
/* MISO    XXh  Data0  Data1   Data2    */
#define CONTINUE_READ_MTP_REG_1_BYTE				0xB5
#define CONTINUE_READ_MTP_REG_DURATION_US   6       // wait here specified time before sending next bytes
#define CONTINUE_READ_MTP_REG_2_BYTE				0x00
#define CONTINUE_READ_MTP_REG_3_BYTE				0x00
#define CONTINUE_READ_MTP_REG_4_BYTE				0x00

/* Write MTP Reg file byte. Write data Data to MTP Reg file at address Addr0(3_BYTE) */
/* MOSI    4Dh   00h   Addr0   Data     */
/* MISO    XXh   D8h    00h    Addr0    */
#define WRITE_MTP_REG_1_BYTE  					    0x4D
#define WRITE_MTP_REG_2_BYTE  					    0x00
#define WRITE_MTP_REG_2_BYTE_OUT				    0xD8
#define WRITE_MTP_REG_3_BYTE_OUT				    0x00
#define WRITE_MTP_REG_DURATION_US           2       // wait here specified time before sending next bytes

/* Continue Write MTP Reg file byte. The command can follow a “Write MTP Reg file” command and it */
/* writes three bytes to the next memory locations in MTP Reg file. */
/* MOSI    56h  Data0  Data1   Data2    */
/* MISO    XXh   67h   Data0   Data1    */
#define CONTINUE_WRITE_MTP_REG_1_BYTE				0x56
#define CONTINUE_WRITE_MTP_REG_2_BYTE_OUT		0x67
#define CONTINUE_WRITE_MTP_REG_DURATION_US  6       // wait here specified time before sending next bytes

/* Read MTP to MTP Reg file. Reads the data from the */
/*  MTP Memory to the MTP Reg file. */
/* MOSI    9Ah   00h    00h     00h     */
/* MISO    XXh   A8h    00h     00h     */
#define READ_MTP_1_BYTE                     0x9A
#define READ_MTP_2_BYTE                     0x00
#define READ_MTP_2_BYTE_OUT                 0xA8
#define READ_MTP_3_BYTE                     0x00
#define READ_MTP_3_BYTE_OUT                 0x00
#define READ_MTP_4_BYTE                     0x00
#define READ_MTP_4_BYTE_OUT                 0x00
#define READ_MTP_DURATION_US                13      /* Maximum Time of MTP Read duration*/ // wait here specified time before sending next bytes

/* Write MTP from MTP Reg file. Write the data written to the */
/*  MTP Reg file to the MTP Memory. */
/* MOSI    2Eh   00h    00h     00h     */
/* MISO    XXh   E6h    00h     00h     */
#define WRITE_MTP_1_BYTE                    0x2E
#define WRITE_MTP_2_BYTE                    0x00
#define WRITE_MTP_2_BYTE_OUT                0xE6
#define WRITE_MTP_3_BYTE                    0x00
#define WRITE_MTP_3_BYTE_OUT                0x00
#define WRITE_MTP_4_BYTE                    0x00
#define WRITE_MTP_4_BYTE_OUT                0x00
#define WRITE_MTP_DURATION_US               20000      /* Maximum Time of MTP Write duration*/ // wait here specified time before sending next bytes

/* Fill MTP by specified value. Write specified byte to the whole MTP memory (erase MTP) */
/* MOSI    39h   00h    00h    Data     */
/* MISO    XXh   92h    00h     00h     */
#define FILL_MTP_1_BYTE  					          0x39
#define FILL_MTP_2_BYTE  				        	  0x00
#define FILL_MTP_2_BYTE_OUT			        	  0x92
#define FILL_MTP_3_BYTE  				        	  0x00
#define FILL_MTP_3_BYTE_OUT			        	  0x00
#define FILL_MTP_4_BYTE_OUT				          0x00
#define FILL_MTP_DURATION_US                21000      /* Maximum Time of MTP Write duration*/ // wait here specified time before sending next bytes


#endif /*MTP_H*/
