@echo off
rem *******************************  mk.bat  *******************************
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
rem               Copyright (c) 2001
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
rem  Description: Make bat file for building Binary sensor
rem
rem  Author:   Peter Shorty
rem
rem  Last Changed By:  $Author: vvi $
rem  Revision:         $Revision: 17230 $
rem  Last Changed:     $Date: 2010-03-30 11:57:31 +0300 (Tue, 30 Mar 2010) $
rem
rem ****************************************************************************

if "%TOOLSDIR%"=="" goto usage_tools

set WINAVRPATH=C:\\winavr
set oldpath=%path%
set path=%TOOLSDIR%\Python;%path%

set target=%1
if "%1" == "" goto set_target
if "%1" == "all" goto set_target
if "%1" == "All" goto set_target
goto skip_set_target
:set_target
set target=ZW0x0x
:skip_set_target

if not exist src\mtp_ZW040x.h goto skip_del_mtp_zw
del /F /S /Q src\mtp_ZW040x.h >nul
:skip_del_mtp_zw
cmd /C make_mtp.bat %1 %2 %3
cd src
%TOOLSDIR%\Make\make %1 %2 %3
cd ..

if NOT "%target%" == "ZW0x0x" goto skip_make_fw
cmd /C make_firmware.bat

if NOT exist "%TOOLSDIR%\..\..\anicca\trunk\Tools\ZWaveProgrammer\ZWaveProgrammer\Resources" goto fw_skip_tools
copy /Y "build\ZWaveProgrammer_FW\ZWaveProgrammer_FW.hex" "%TOOLSDIR%\..\..\anicca\trunk\Tools\ZWaveProgrammer\ZWaveProgrammer\Resources\ZWaveProgrammer_FW.hex"
goto :fw_skip
:fw_skip_tools
if NOT exist "..\..\..\..\anicca\trunk\Tools\ZWaveProgrammer\ZWaveProgrammer\Resources" goto fw_skip_rel
copy /Y "build\ZWaveProgrammer_FW\ZWaveProgrammer_FW.hex" "..\..\..\..\anicca\trunk\Tools\ZWaveProgrammer\ZWaveProgrammer\Resources\ZWaveProgrammer_FW.hex"
goto :fw_skip
:fw_skip_rel
:fw_skip

:skip_make_fw



:done
set path=%oldpath%
set oldpath=
set target=
goto exit


:usage_tools
@echo Set TOOLSDIR to point to the location of the Z-Wave tools
@echo e.g c:\projects\zensys\devkit\tools
goto exit

:exit
set WINAVRPATH=
