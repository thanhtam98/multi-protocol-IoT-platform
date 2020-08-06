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
rem  Description: Make bat file for building LED dimmer
rem
rem  Author:   Valeriy Vyshnyak
rem
rem  Last Changed By:  $Author: vvi $
rem  Revision:         $Revision: 0 $
rem  Last Changed:     $Date: 2009-11-11 $
rem
rem ****************************************************************************

if "%KEILPATH%"==""  goto usage_keil
if "%TOOLSDIR%"=="" goto usage_tools

if not exist %KEILPATH%\bin\c51.exe goto usage_keil
if not exist %KEILPATH%\bin\cx51.exe goto no_ext

set oldpath=%path%
set path=%KEILPATH%\bin;%TOOLSDIR%\Python;%path%

set target=%1
if "%1" == "" goto set_target
if "%1" == "all" goto set_target
if "%1" == "All" goto set_target
goto skip_set_target
:set_target
set target=ZW0x0x
:skip_set_target

REM %TOOLSDIR%\Make\make %1 %2 %3




if not "%target%" == "ZW0x0x" goto skip_make_ZW0x0x

if exist build\ goto skip_mk_build
mkdir build
:skip_mk_build

if exist build\ZW040x\ goto skip_mk_build_zw
mkdir build\ZW040x
:skip_mk_build_zw

if exist build\ZW040x\list\ goto skip_mk_build_zw_list
mkdir build\ZW040x\list
:skip_mk_build_zw_list

if exist build\ZW040x\Rels\ goto skip_mk_build_zw_rels
mkdir build\ZW040x\Rels
:skip_mk_build_zw_rels

%KEILPATH%\bin\C51.EXE .\src\mtp.c OMF2 ROM(COMPACT) WARNINGLEVEL(1) OPTIMIZE(0,SPEED) REGFILE(.\build\ZW040x\Rels\mtp_ZW040x.ORC) BROWSE NOINTPROMOTE NOAREGS NOINTVECTOR DB OE CD SB LC NOIP NOCO DEFINE(ZW040x,ZW0401,X) DEBUG PRINT(.\build\ZW040x\list\mtp.lst) TABS(2) OBJECT(.\build\ZW040x\Rels\mtp.obj)
%KEILPATH%\bin\LX51.EXE .\build\ZW040x\Rels\mtp.obj TO .\build\ZW040x\Rels\mtp_ZW040x.AOF REGFILE (.\build\ZW040x\Rels\mtp_ZW040x.ORC) PRINT (.\build\ZW040x\list\mtp_ZW040x.map) NOOL CLASSES (IDATA (D:0-D:0XFF), XDATA (X:0-X:0), CODE (C:0-C:0XFFF))
%KEILPATH%\bin\OHX51 .\build\ZW040x\Rels\mtp_ZW040x.AOF HEXFILE(.\build\ZW040x\mtp_ZW040x.hex)
%TOOLSDIR%\HexTools\srec_cat.exe -Output .\src\mtp_ZW040x.h -C-Array MtpHandler .\build\ZW040x\mtp_ZW040x.hex -Intel
goto done

:skip_make_ZW0x0x



if not "%target%" == "clean" goto skip_clean

if not exist build\ZW040x\ goto skip_clean_del
rmdir /S /Q build\ZW040x >nul
:skip_clean_del

if not exist src\mtp_ZW040x.h goto skip_clean_h
del /F /Q src\mtp_ZW040x.h >nul
:skip_clean_h

goto done

:skip_clean


@echo %0: Nothing to be done for '%target%'



:done
set path=%oldpath%
set oldpath=
set target=
goto exit

:usage_keil
@echo Set KEILPATH to point to the location of the Keil Compiler
@echo e.g c:\keil\c51
goto exit

:usage_tools
@echo Set TOOLSDIR to point to the location of the Z-Wave tools
@echo e.g c:\projects\zensys\devkit\tools
goto exit

:no_ext
@echo This developers kit requires the Keil PK51 Professional Developer's Kit

:exit
