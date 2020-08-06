@echo off
rem **************************  make_firmware.bat  ***************************
rem            #######
rem            ##  ##
rem            #  ##    ####   #####    #####  ##  ##   #####
rem              ##    ##  ##  ##  ##  ##      ##  ##  ##
rem             ##  #  ######  ##  ##   ####   ##  ##   ####
rem            ##  ##  ##      ##  ##      ##   #####      ##
rem           #######   ####   ##  ##  #####       ##  #####
rem                                            #####
rem           Z-Wave, the wireless language.
rem
rem               Copyright (c) 2008
rem               Zensys A/S
rem               Denmark
rem
rem               All Rights Reserved
rem
rem     This source file is subject to the terms and conditions of the
rem     Zensys Software License Agreement which restricts the manner
rem     in which it may be used.
rem
rem ---------------------------------------------------------------------------
rem
rem  Description: DOS Batchfile make Programmer firmware for uploading it
rem               to the ATMega128 via AVR ISP Mk II
rem
rem  Author:   Valeriy Vyshnyak
rem  Last Changed By:  $Author: vvi $
rem  Revision:         $Revision: 10080 $
rem  Last Changed:     $Date: 2008-03-19 14:39:52 +0200 $
rem
rem ****************************************************************************
@echo on

@if not exist build\BootLoader\BootLoader.hex goto eclipse
@if not exist build\ZWaveProgrammer_FW\ZWaveProgrammer_FW.hex goto eclipse
copy build\BootLoader\BootLoader.hex b.hex
copy build\ZWaveProgrammer_FW\ZWaveProgrammer_FW.hex z.hex
SET OUTPUT_DIR=build\ZWaveProgrammer_FW
@goto join

:eclipse
@if not exist "BootLoader\BootLoader.hex" goto eclipse_dbg
@if not exist "ZWaveProgrammer_FW\ZWaveProgrammer_FW.hex" goto eclipse_dbg
copy "BootLoader\BootLoader.hex" b.hex
copy "ZWaveProgrammer_FW\ZWaveProgrammer_FW.hex" z.hex
SET OUTPUT_DIR=ZWaveProgrammer_FW
@goto join

:eclipse_dbg
@if not exist "BootLoader Debug\BootLoader.hex" goto error
@if not exist "ZWaveProgrammer_FW Debug\ZWaveProgrammer_FW.hex" goto error
copy "BootLoader Debug\BootLoader.hex" b.hex
copy "ZWaveProgrammer_FW Debug\ZWaveProgrammer_FW.hex" z.hex
SET OUTPUT_DIR=ZWaveProgrammer_FW Debug
@goto join

@goto NoFW

:join

HexTools\srec_cat.exe z.hex -Intel b.hex -Intel -Output fw.hex -Intel -address-length=2  -DO -Line_Length 44
@if ERRORLEVEL 1 goto NoFW
@echo :00000001FF>>fw.hex
del b.hex
del z.hex
@del ATMega128_Firmware.hex
move /Y fw.hex "%OUTPUT_DIR%\ATMega128_Firmware.hex"
@goto exit

:NoFW
del b.hex
del z.hex
:error
@echo.
@echo ERROR!!!
@echo.
@pause
:exit