/*************************  lowlevel.h  *******************************
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
 * Description: Low-level routines to read lock and fuse-bytes,
 *              timed delays, etc.
 *
 * Author:      Valeriy Vyshnyak
 *
 * Last Changed By:  $Author: vvi $
 * Revision:         $Revision: 0000 $
 * Last Changed:     $Date: 2009-11-11$
 *
 ****************************************************************************/

#ifndef LOWLEVEL_H
#define LOWLEVEL_H

#include <avr/io.h>


#define _NOP_   asm volatile ("nop\n\t"::)

#define DELAY_1_CLK    _NOP_ //delay_loop(1) /*the function use 3 cpu clk for each iteration, thus 2*3 @ 8 MHz clk = 6 */
#define DELAY_9_CLK    delay_loop(3) //delay_loop(18)
#define DELAY_36_CLK   delay_loop(12) //delay_loop(72)
#define DELAY_50_US    delay_loop(123)
#define DELAY_10_US    delay_loop(25)

/* Make delay with maximum time: */
#define DELAY_LOOP_MAX					delay_loop(0)

/* Maximum delay in uS */
#define DELAY_LOOP_MAX_US				(DWORD)(1000000.0*256.0*3.0/(double)F_CPU)

/* Calculate delay_loop counter value from uS */
#define DELAY_LOOP_FROM_US(_US)	(DWORD)((_US)*(F_CPU/1000000)/3)

/* Make delay with help of delay_loop. Take a uS as argument */
#define DELAY_LOOP_US(_US_)			{BYTE cnt = DELAY_LOOP_FROM_US(_US_); if(cnt != 0) delay_loop(cnt);}


/* Check for SPM Control Register in processor. */
#if defined (SPMCSR)
#  define SPM_REG    SPMCSR
#elif defined (SPMCR)
#  define SPM_REG    SPMCR
#else
#  error AVR processor does not provide bootloader support!
#endif

unsigned char read_fuse_lock(unsigned short addr, unsigned char mode);

/** \ingroup avr_delay

    Delay loop using an 8-bit counter \c __count, so up to 256
    iterations are possible.  (The value 256 would have to be passed
    as 0.)  The loop executes three CPU cycles per iteration, not
    including the overhead the compiler needs to setup the counter
    register.

    Thus, at a CPU speed of 1 MHz, delays of up to 768 microseconds
    can be achieved.
*/
static __inline__ void
delay_loop(uint8_t __count)
{
	__asm__ volatile (
		"1: dec %0" "\n\t"
		"brne 1b"
		: "=r" (__count)
		: "0" (__count)
	);
}

#endif
