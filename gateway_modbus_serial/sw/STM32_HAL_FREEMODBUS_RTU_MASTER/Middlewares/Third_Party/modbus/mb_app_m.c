/*
 Brief: Optional functions callback when Modbus sequences are called in Master mode
 Author: NTT
 Date: 30-3-2020
 Version: 1.0
 */
/*Built-in C lib includes-----------------*/

/*Platform includes-----------------------*/

/*FreeRTOS includes-----------------------*/
#include "main.h";
#include "FreeRTOS.h"

/*Modbus platform includes----------------*/
#include "mb_app_m.h"
#include "mb_m.h"
#include "mbconfig.h"
#include "mb.h"

/*Private define -------------------------*/

#define M_REG_HOLDING_START           0
#define M_REG_HOLDING_NREGS           40
/*Shared variables------------------------*/
osMessageQId xQueueDownlinkHandle;
osMessageQId xQueueUplinkHandle;
/*Private variable------------------------*/

/*Start inplementation--------------------------*/

