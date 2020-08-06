/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
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
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */
/* ----------------------- Platform includes ----------------------------------*/
#include "port.h"
#include "main.h"
#include "gpio.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
//static void prvvUARTTxReadyISR( void );
//static void prvvUARTRxISR( void );
/* -----------------------    variables     ---------------------------------*/
extern UART_HandleTypeDef huart3;

/* ----------------------- Start implementation -----------------------------*/
void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable) {
	/* If xRXEnable enable serial receive interrupts. If xTxENable enable
	 * transmitter empty interrupts.
	 */

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

BOOL xMBPortSerialInit(UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits,
		eMBParity eParity) {
	/*
	 Do nothing, Initialization is handled by MX_USART3_UART_Init()
	 Fixed port, baudrate, databit and parity
	 */
	return TRUE;
}

BOOL xMBPortSerialPutByte(CHAR ucByte) {
	/* Put a byte in the UARTs transmit buffer. This function is called
	 * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
	 * called. */
	return (HAL_OK == HAL_UART_Transmit(&huart3, (uint8_t*) &ucByte, 1, 10));
}

BOOL xMBPortSerialGetByte(CHAR * pucByte) {
	/* Return the byte in the UARTs receive buffer. This function is called
	 * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
	 */
//	HAL_GPIO_WritePin(USART3_LED_GPIO_Port, USART3_LED_Pin, 1);
	*pucByte = (uint8_t) (huart3.Instance->DR & (uint8_t) 0x00FF);
//	HAL_GPIO_WritePin(USART3_LED_GPIO_Port, USART3_LED_Pin, 0);
	return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call
 * xMBPortSerialPutByte( ) to send the character.
 
 static void prvvUARTTxReadyISR( void )
 {
 pxMBFrameCBTransmitterEmpty(  );
 }
 */

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 
 static void prvvUARTRxISR( void )
 {
 pxMBFrameCBByteReceived(  );
 }
 */
//void USART2_IRQHandler(void) {
//	/* USER CODE BEGIN USART2_IRQn 0 */
//	uint32_t tmp_flag = __HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE);
//	uint32_t tmp_it_source = __HAL_UART_GET_IT_SOURCE(&huart2, UART_IT_RXNE);
//
//	if ((tmp_flag != RESET) && (tmp_it_source != RESET)) {
//		pxMBFrameCBByteReceived();
//		__HAL_UART_CLEAR_PEFLAG(&huart2);
//		return;
//	}
//
//	if ((__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TXE) != RESET)
//			&& (__HAL_UART_GET_IT_SOURCE(&huart2, UART_IT_TXE) != RESET)) {
//		pxMBFrameCBTransmitterEmpty();
//		return;
//	}
//
//	/* USER CODE END USART2_IRQn 0 */
//	HAL_UART_IRQHandler(&huart2);
//	/* USER CODE BEGIN USART2_IRQn 1 */
//
//	/* USER CODE END USART2_IRQn 1 */
//}
