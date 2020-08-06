/*
 * FreeModbus Libary: RT-Thread Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttimer_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbport.h"


/* ----------------------- Variables ----------------------------------------*/

extern TIM_HandleTypeDef htim7;

uint16_t timeout = 0;

TimerExpiredType TimerExpired[MB_RS485_MAX_PORT] = {MB_TMODE_NONE,MB_TMODE_NONE,MB_TMODE_NONE,MB_TMODE_NONE};
// uint16_t downcounter = 
/* ----------------------- static functions ---------------------------------*/
//static void prvvTIMERExpiredISR(void);

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBMasterPortTimersInit(USHORT usTimeOut50us)
{
   TimerExpired[0].Event = MB_TMODE_NONE;
    TimerExpired[1].Event = MB_TMODE_NONE;
     TimerExpired[2].Event = MB_TMODE_NONE;
      TimerExpired[3].Event = MB_TMODE_NONE;

  /*No thing to do 
   User software timer intead
  */
      TIM_MasterConfigTypeDef sMasterConfig;

   htim7.Instance = TIM7;
   htim7.Init.Prescaler = (HAL_RCC_GetPCLK1Freq() / 1000000) - 1;
   htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
   htim7.Init.Period = 50 - 1;

   timeout = usTimeOut50us;
//      timeout = 3;
   if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
   {
     return FALSE;
   }

   sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
   sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
   if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
   {
     return FALSE;
   }
//  HAL_TIM_Base_Start_IT(&htim7);
	
  return TRUE;
}

void vMBMasterPortTimersT35Enable(UCHAR ucPort)
{
	  vMBMasterSetCurTimerMode(ucPort,MB_TMODE_T35);
    TimerExpired[ucPort].TimeOut = timeout;
		TimerExpired[ucPort].Event = MB_TMODE_T35;
}

void vMBMasterPortTimersConvertDelayEnable(UCHAR ucPort)
{
   vMBMasterSetCurTimerMode(ucPort,MB_TMODE_CONVERT_DELAY);
		 TimerExpired[ucPort].TimeOut = timeout*3;
		TimerExpired[ucPort].Event = MB_TMODE_CONVERT_DELAY;

}

void vMBMasterPortTimersRespondTimeoutEnable(UCHAR ucPort)
{
		vMBMasterSetCurTimerMode(ucPort,MB_TMODE_RESPOND_TIMEOUT);
		TimerExpired[ucPort].TimeOut = timeout*1000;
		TimerExpired[ucPort].Event = MB_TMODE_RESPOND_TIMEOUT;

}

void vMBMasterPortTimersDisable(UCHAR ucPort)
{
   /* Disable any pending timers. */

  TimerExpired[ucPort].Event = MB_TMODE_NONE;
  /* Timer will always run ports */
//  HAL_TIM_Base_Stop_IT(&htim7);
}

void prvvMasterTIMERExpiredISR(void)
{
  
  UCHAR ucPort = 0;
  for (ucPort = 0; ucPort < MB_RS485_MAX_PORT; ucPort++)
  {
    if ((TimerExpired[ucPort].Event != MB_TMODE_NONE ))
    {
      if ((--TimerExpired[ucPort].TimeOut) == 0)
        {
          TimerExpired[ucPort].Event = MB_TMODE_NONE;
					if (pxMBMasterPortCBTimerExpired != NULL)
					{
          (void) pxMBMasterPortCBTimerExpired(ucPort);
					}
        }
    }
  }

}


