
  
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
 * File: $Id: portevent_m.c v 1.60 2013/08/13 15:07:05 Armink add Master Functions$
 */
/*------------------------Built-in C library includes -----------------------*/
#include <stdio.h>
#include <string.h>

/*------------------------ Platform includes ------------------------*/
#include "main.h"

/*------------------------FreeRTOS Platform includes ------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbport.h"
#include "port.h"

/* ----------------------- Defines ------------------------------------------*/
enum
{
    RELEASE = 0,
    TAKE
};
/* ----------------------- Variables ----------------------------------------*/
static eMBMasterEventType eMasterQueuedEvent[MB_RS485_MAX_PORT];
static BOOL xMasterEventInQueue[MB_RS485_MAX_PORT];
static BOOL xMasterRunRes[MB_RS485_MAX_PORT];
/* ----------------------- Start implementation -----------------------------*/
BOOL xMBMasterPortEventInit(void)
{
    UCHAR ucPort;
    for (ucPort = 0; ucPort < MB_RS485_MAX_PORT; ucPort++)
    {
        xMasterEventInQueue[ucPort] = FALSE;
    }

    return TRUE;
}

BOOL xMBMasterPortEventPost(UCHAR ucPort, eMBMasterEventType eEvent)
{
    xMasterEventInQueue[ucPort] = TRUE;
    eMasterQueuedEvent[ucPort] = eEvent;
    return TRUE;
}

BOOL xMBMasterPortEventGet(UCHAR ucPort, eMBMasterEventType *eEvent)
{
    BOOL xEventHappened = FALSE;

    if (xMasterEventInQueue[ucPort])
    {
			switch (eMasterQueuedEvent[ucPort])
         {
         case EV_MASTER_READY:
             *eEvent = EV_MASTER_READY;
             break;
         case EV_MASTER_FRAME_RECEIVED:
             *eEvent = EV_MASTER_FRAME_RECEIVED;
             break;
         case EV_MASTER_EXECUTE:
             *eEvent = EV_MASTER_EXECUTE;
             break;
         case EV_MASTER_FRAME_SENT:
             *eEvent = EV_MASTER_FRAME_SENT;
             break;
         case EV_MASTER_ERROR_PROCESS:
             *eEvent = EV_MASTER_ERROR_PROCESS;
             break;
         default:
             vTaskDelay(1); //??
             return FALSE;
             break;
         }
//        *eEvent = eMasterQueuedEvent[ucPort];
        xMasterEventInQueue[ucPort] = FALSE;
        xEventHappened = TRUE;
    }
    return xEventHappened;
}
/**
 * This function is initialize the OS resource for modbus master.
 * Note:The resource is define by OS.If you not use OS this function can be empty.
 *
 */
void vMBMasterOsResInit(void)
{
    BOOL vNULL = 0;
    memcpy(xMasterRunRes, &vNULL, MB_RS485_MAX_PORT);
    //rt_sem_init(&xMasterRunRes, "master res", 0x01 , RT_IPC_FLAG_PRIO);
}

/**
 * This function is take Mobus Master running resource.
 * Note:The resource is define by Operating System.If you not use OS this function can be just return TRUE.
 *
 * @param lTimeOut the waiting time.
 *
 * @return resource taked result
 */
BOOL xMBMasterRunResTake(UCHAR ucPort, LONG lTimeOut)
{
    /*If waiting time is -1 .It will wait forever */
    if (xMasterRunRes[ucPort] == RELEASE)
    {
        xMasterRunRes[ucPort] = TAKE;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    //return rt_sem_take(&xMasterRunRes, lTimeOut) ? FALSE : TRUE ;
}

/**
 * This function is release Mobus Master running resource.
 * Note:The resource is define by Operating System.If you not use OS this function can be empty.
 *
 */
void vMBMasterRunResRelease(UCHAR ucPort)
{
    /* release resource */
    xMasterRunRes[ucPort] = RELEASE;
    //  rt_sem_release(&xMasterRunRes);
}

/**
 * This is modbus master respond timeout error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 * @param ucDestAddress destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMBMasterErrorCBRespondTimeout(UCHAR ucPort, UCHAR ucDestAddress, const UCHAR *pucPDUData,
                                    USHORT ucPDULength)
{
    /**
     * @note This code is use OS's event mechanism for modbus master protocol stack.
     * If you don't use OS, you can change it.
     */
	HAL_GPIO_WritePin(GPIO_SPEAKER_GPIO_Port,GPIO_SPEAKER_Pin,TRUE);
	
	HAL_Delay(10);
		HAL_GPIO_TogglePin(GPIO_SPEAKER_GPIO_Port,GPIO_SPEAKER_Pin);
		printf("\r\n vMBMasterErrorCBRespondTimeout: sent data to queue");
    xMBMasterPortEventPost(ucPort, EV_MASTER_ERROR_RESPOND_TIMEOUT);
    //rt_event_send(&xMasterOsEvent, EV_MASTER_ERROR_RESPOND_TIMEOUT);

    /* You can add your code under here. */
}

/**
 * This is modbus master receive data error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 * @param ucDestAddress destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMBMasterErrorCBReceiveData(UCHAR ucPort, UCHAR ucDestAddress, const UCHAR *pucPDUData,
                                 USHORT ucPDULength)
{
    /**
     * @note This code is use OS's event mechanism for modbus master protocol stack.
     * If you don't use OS, you can change it.
     */
	printf("\r\n vMBMasterErrorCBReceiveData ");
    xMBMasterPortEventPost(ucPort, EV_MASTER_ERROR_RESPOND_TIMEOUT);
    //rt_event_send(&xMasterOsEvent, EV_MASTER_ERROR_RECEIVE_DATA);

    /* You can add your code under here. */
}

/**
 * This is modbus master execute function error process callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 * @param ucDestAddress destination salve address
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMBMasterErrorCBExecuteFunction(UCHAR ucPort, UCHAR ucDestAddress, const UCHAR *pucPDUData,
                                     USHORT ucPDULength)
{
    /**
     * @note This code is use OS's event mechanism for modbus master protocol stack.
     * If you don't use OS, you can change it.
     */
	printf("\r\n vMBMasterErrorCBExecuteFunction ");
    xMBMasterPortEventPost(ucPort, EV_MASTER_ERROR_EXECUTE_FUNCTION);
    // rt_event_send(&xMasterOsEvent, EV_MASTER_ERROR_EXECUTE_FUNCTION);

    /* You can add your code under here. */
}

/**
 * This is modbus master request process success callback function.
 * @note There functions will block modbus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 */
void vMBMasterCBRequestScuuess(UCHAR ucPort)
{
    /**
     * @note This code is use OS's event mechanism for modbus master protocol stack.
     * If you don't use OS, you can change it.
     */
    //  rt_event_send(&xMasterOsEvent, EV_MASTER_PROCESS_SUCESS);

    xMBMasterPortEventPost(ucPort, EV_MASTER_PROCESS_SUCESS);
    /* You can add your code under here. */
}

/**
 * This function is wait for modbus master request finish and return result.
 * Waiting result include request process success, request respond timeout,
 * receive data error and execute function error.You can use the above callback function.
 * @note If you are use OS, you can use OS's event mechanism. Otherwise you have to run
 * much user custom delay for waiting.
 *
 * @return request error code
 */
eMBMasterReqErrCode eMBMasterWaitRequestFinish(UCHAR ucPort)
{
    eMBMasterReqErrCode eErrStatus = MB_MRE_NO_ERR;
		eMBMasterEventType  eEvent;
    // rt_uint32_t recvedEvent;
waiting_point:
		//if(xMBMasterPortEventGet( ucPort,&eEvent) == TRUE)
    if (xMasterEventInQueue[ucPort] == TRUE)
    {
        switch (eMasterQueuedEvent[ucPort])
        {
        case EV_MASTER_PROCESS_SUCESS:
            break;
        case EV_MASTER_ERROR_RESPOND_TIMEOUT:
        {
            eErrStatus = MB_MRE_TIMEDOUT;
            break;
        }
        case EV_MASTER_ERROR_RECEIVE_DATA:
        {
            eErrStatus = MB_MRE_REV_DATA;
            break;
        }
        case EV_MASTER_ERROR_EXECUTE_FUNCTION:
        {
            eErrStatus = MB_MRE_EXE_FUN;
            break;
        }
				default:
					//xMBMasterPortEventPost( ucPort,eEvent);
					vTaskDelay(1);
					goto waiting_point;
        }
				xMasterEventInQueue[ucPort] = FALSE;
        return eErrStatus;
    }
    else {
        vTaskDelay(20);
        goto waiting_point;
    }
}
