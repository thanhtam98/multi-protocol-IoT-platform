##******************************* Makefile *********************************
##           #######
##           ##  ##
##           #  ##    ####   #####    #####  ##  ##   #####
##             ##    ##  ##  ##  ##  ##      ##  ##  ##
##            ##  #  ######  ##  ##   ####   ##  ##   ####
##           ##  ##  ##      ##  ##      ##   #####      ##
##          #######   ####   ##  ##  #####       ##  #####
##                                           #####
##          Z-Wave, the wireless language.
##
##              Copyright (c) 2001
##              Zensys A/S
##              Denmark
##
##              All Rights Reserved
##
##    This source file is subject to the terms and conditions of the
##    Zensys Software License Agreement which restricts the manner
##    in which it may be used.
##
##---------------------------------------------------------------------------
##
## Description:
##    Makefile for the ZW0x0x development kit sinffer
##    This makefile defines the targets built
##
## Author:   Samer Seoud
##
## Last Changed By:  $Author: efh $
## Revision:         $Revision: 12369 $
## Last Changed:     $Date: 2009-01-07 12:05:22 +0200 (Ср, 07 Січ 2009) $
##
##***************************************************************************/

# Application name definition
APPNAME:=ATMega_ZWaveProgFW

# Find the current directory
# used as a base for all other paths.
BASEDIR:=$(shell cd)

# Find the product name from the subdirectoryname
BASEDIRWORDLIST=$(subst \, ,$(subst \src,,$(BASEDIR)))
BASEDIRLASTWORDINDEX=$(words $(BASEDIRWORDLIST))
PRODUCTDIR=$(word $(BASEDIRLASTWORDINDEX),$(BASEDIRWORDLIST))

# Find the Z-Wave library location (if not forced by setting the environment variable ZWLIBROOT)
ifndef ZWLIBROOT
ZWLIBROOT:=$(subst \$(PRODUCTDIR)\src,,$(subst \Product,\Z-Wave,$(BASEDIR)))
# ZWLIBROOT:=D:\My\Zensys\Work\mantra\trunk\Z-Wave
endif

# Library location when making release of application
ifdef SVNLIBRELEASEDIRBIN
ZWLIBROOT:=$(SVNLIBRELEASEDIRBIN)
endif

# Export the variables declared above to the other makefiles.
export APPNAME BASEDIR ZWLIBROOT

BDIRS=build

default: all

# Standard make names (to be compatible with release system)
ZW0x0x: all
ZW0x0x_tf:

all: ATM128_ZWaveProgFW ATM128_BootLoader

# Common targets
ATM128_ZWaveProgFW: _ATMega128_ZWaveProgFW
ATM128_BootLoader: _ATM128_BootLoader


##***************************************************************************/

# Include test targets (if any)

dirs: $(BDIRS)

MAKEOPTS=--no-print-directory -C

##***************************************************************************/


depend_ATMega128_ZWaveProgFW:
	$(MAKE) depend $(MAKEOPTS) . CHIP=atmega128 MT_BOOTLOADER_ADDRESS=00000 -f Makefile.ATMega_ZWaveProgFW_common

_ATMega128_ZWaveProgFW: depend_ATMega128_ZWaveProgFW
	$(MAKE) $(MAKEOPTS) . CHIP=atmega128 MT_BOOTLOADER_ADDRESS=00000 -f Makefile.ATMega_ZWaveProgFW_common


depend_ATM128_BootLoader:
	$(MAKE) depend $(MAKEOPTS) . CHIP=atmega128  MT_BOOTLOADER_ADDRESS=1F000 -f Makefile.ATMega_BootLoader_common

_ATM128_BootLoader: depend_ATM128_BootLoader
	$(MAKE) $(MAKEOPTS) . CHIP=atmega128  MT_BOOTLOADER_ADDRESS=1F000 -f Makefile.ATMega_BootLoader_common


##***************************************************************************/

# Deletes all generated files (relocatables, downloadables,
# dependency files and the generated C and headerfiles from ZUIB)
clean:
ifeq ($(OS), Windows_NT)
	@cmd /c "if exist ..\build\NUL rd ..\build /s /q"
else
	@deltree /Y $(subst \,\\,..\build)
endif

##***************************************************************************/

# Catch all targets which we do not handle and tell world that
%:
	@echo Could not find rule for target '$@'
	@echo Some of the known targets are :
	@echo   ATM128_ZWaveProgFW        : ATMega 128 Z-Wave flash programmer
	@echo   ATM128_BootLoader         : ATMega 128 boot loader 


# All targets that shouldn't result in a file/directory
# with the same name as the target should be listed here.
# Otherwise the above rule will make a directory with
# the targetname
.PHONY: dirs clean depend all default ATM128_ZWaveProgFW ATM128_BootLoader Makefile ZW0x0x_tf ZW0x0x