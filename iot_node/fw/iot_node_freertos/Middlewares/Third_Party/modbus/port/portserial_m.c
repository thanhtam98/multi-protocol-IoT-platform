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
 * File: $Id: portserial_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "main.h"
#include "mb_m.h"
#include "mbconfig.h"
#include "usart.h"



#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
/* ----------------------- Static variables ---------------------------------*/

/* ----------------------- Defines ------------------------------------------*/
/* serial transmit event */
#define EVENT_SERIAL_TRANS_START    (1<<0)

/* ----------------------- static functions ---------------------------------*/


extern UART_HandleTypeDef huart3;
/* ----------------------- Start implementation -----------------------------*/
BOOL xMBMasterPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits,
		eMBParity eParity)
{
	/*
		 Do nothing, Initialization is handled by MX_USART3_UART_Init()
		 Fixed port, baudrate, databit and parity
		 */
		return TRUE;
}

void vMBMasterPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
	if (xRxEnable) {
		HAL_GPIO_WritePin(MB_RDE_GPIO_Port, MB_RDE_Pin, 0);
		//			HAL_GPIO_WritePin(USART3_LED_GPIO_Port, USART3_LED_Pin, 0);
		__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);
	} else {
		__HAL_UART_DISABLE_IT(&huart3, UART_IT_RXNE);
	}

	if (xTxEnable) {
		//		HAL_GPIO_WritePin(USART3_LED_GPIO_Port, USART3_LED_Pin, 1);
		HAL_GPIO_WritePin(MB_RDE_GPIO_Port, MB_RDE_Pin, 1);
		__HAL_UART_ENABLE_IT(&huart3, UART_IT_TXE);
	} else {
		__HAL_UART_DISABLE_IT(&huart3, UART_IT_TXE);
	}
}

void vMBMasterPortClose(void)
{
//    serial->parent.close(&(serial->parent));
}

BOOL xMBMasterPortSerialPutByte(CHAR ucByte)
{
	return (HAL_OK == HAL_UART_Transmit(&huart3, (uint8_t *)&ucByte, 1, 10));

}

BOOL xMBMasterPortSerialGetByte(CHAR * pucByte)
{
	*pucByte = (uint8_t)((huart3).Instance->DR & (uint8_t)0x00FF);
	return TRUE;

}

/* 
 * Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
//void prvvUARTTxReadyISR(void)
//{
//	pxMBMasterFrameCBTransmitterEmpty();
//}

/* 
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
//void prvvUARTRxISR(void)
//{
//	pxMBMasterFrameCBByteReceived();
//}

/**
 * Software simulation serial transmit IRQ handler.
 *
 * @param parameter parameter
 */
//static void serial_soft_trans_irq(void* parameter) {
//	rt_uint32_t recved_event;
//	while (1)
//	{
//		/* waiting for serial transmit start */
//		rt_event_recv(&event_serial, EVENT_SERIAL_TRANS_START, RT_EVENT_FLAG_OR,
//				RT_WAITING_FOREVER, &recved_event);
//		/* execute modbus callback */
//		prvvUARTTxReadyISR();
//	}
//}

/**
 * This function is serial receive callback function
 *
 * @param dev the device of serial
 * @param size the data size that receive
 *
 * @return return RT_EOK
 */
//static rt_err_t serial_rx_ind(rt_device_t dev, rt_size_t size) {
//	prvvUARTRxISR();
//	return RT_EOK;
//}

#endif
