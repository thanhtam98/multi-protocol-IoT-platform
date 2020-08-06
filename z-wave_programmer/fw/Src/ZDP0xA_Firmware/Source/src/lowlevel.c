/*************************  lowlevel.c  *******************************
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

//
//  Low-level routines to read lock and fuse-bytes
//
//  Copyright (C) 2/2005 Martin Thomas, Kaiserslautern, Germany
//

#include "lowlevel.h"

unsigned char read_fuse_lock(unsigned short addr, unsigned char mode)
{
	unsigned char retval;

	asm volatile
	(
		"movw r30, %3\n\t" /* Z to addr */ \
		"sts 0x68, %2\n\t" /*"sts %0, %2\n\t"*/ /* set mode in SPM_REG */ \
		"lpm\n\t"  /* load fuse/lock value into r0 */ \
		"mov %1,r0\n\t" /* save return value */ \
		: "=m" (SPM_REG),
		  "=r" (retval)
		: "r" (mode),
		  "r" (addr)
		: "r30", "r31", "r0"
	);
	return retval;
}




