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

/*------------------------ Built-in C library includes----------------*/
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
/*------------------------ Platform includes----------------*/

#include "port.h"
#include "main.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Static variables ---------------------------------*/

/* ----------------------- Defines ------------------------------------------*/
/* serial transmit event */
#define EVENT_SERIAL_TRANS_START (1 << 0)

/* ----------------------- static functions ---------------------------------*/
UART_HandleTypeDef UARTHandleGet(UCHAR ucPORT);
void RS485SetDir(UCHAR ucPort, ACTION action);
UART_HandleTypeDef *pUARTHandleGet(UCHAR ucPORT);
void UARTHandleSwitch(UCHAR ucPort);
void RS485LedInd(UCHAR ucPort, BOOL bState);
/* -----------------------    variables     ---------------------------------*/
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart6;

UART_HandleTypeDef *phuart;

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

void vMBMasterPortSerialEnable(UCHAR ucPort, BOOL xRxEnable, BOOL xTxEnable)
{
  /* If xRXEnable enable serial receive interrupts. If xTxENable enable
  * transmitter empty interrupts.
  */
  UARTHandleSwitch(ucPort);
  if (xRxEnable)
  {
    RS485SetDir(ucPort, MB_RS485_IN);
    RS485LedInd(ucPort, FALSE);
    //HAL_GPIO_WritePin(USART3_RDE_GPIO_Port, USART3_RDE_Pin, 0);
    __HAL_UART_ENABLE_IT(phuart, UART_IT_RXNE);
  }
  else
  {
    __HAL_UART_DISABLE_IT(phuart, UART_IT_RXNE);
  }

  if (xTxEnable)
  {
    RS485SetDir(ucPort, MB_RS485_OUT);
    RS485LedInd(ucPort, TRUE);
    //HAL_GPIO_WritePin(USART3_RDE_GPIO_Port, USART3_RDE_Pin, 1);
    __HAL_UART_ENABLE_IT(phuart, UART_IT_TXE);
  }
  else
  {
    __HAL_UART_DISABLE_IT(phuart, UART_IT_TXE);
  }
}

void vMBMasterPortClose(UCHAR ucPort)
{
  // serial->parent.close(&(serial->parent));
}

BOOL xMBMasterPortSerialPutByte(UCHAR ucPort, CHAR ucByte)
{
  /* Put a byte in the UARTs transmit buffer. This function is called
  * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
  * called. */
  UARTHandleSwitch(ucPort);
  return (HAL_OK == HAL_UART_Transmit(phuart, (uint8_t *)&ucByte, 1, 10));
}

BOOL xMBMasterPortSerialGetByte(UCHAR ucPort, CHAR *pucByte)
{
  /* Return the byte in the UARTs receive buffer. This function is called
  * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
  */
  UARTHandleSwitch(ucPort);

  *pucByte = (uint8_t)((*(phuart)).Instance->DR & (uint8_t)0x00FF);
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
//    pxMBMasterFrameCBTransmitterEmpty();
//}

/* 
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
//void prvvUARTRxISR(void)
//{
//    pxMBMasterFrameCBByteReceived();
//}

/**
 * Software simulation serial transmit IRQ handler.
 *
 * @param parameter parameter
 */
//static void serial_soft_trans_irq(void* parameter) {
//    rt_uint32_t recved_event;
//    while (1)
//    {
//        /* waiting for serial transmit start */
//        rt_event_recv(&event_serial, EVENT_SERIAL_TRANS_START, RT_EVENT_FLAG_OR,
//                RT_WAITING_FOREVER, &recved_event);
//        /* execute modbus callback */
//        prvvUARTTxReadyISR();
//    }
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
//    prvvUARTRxISR();
//    return RT_EOK;
//}

/*HW Function Porting -------------------------------------------*/
/*
Brief: Led indicate when TX actived
Todo: 
*/
void RS485LedInd(UCHAR ucPort, BOOL bState)
{
  switch (ucPort)
  {
  case PORT1:
    HAL_GPIO_WritePin(USART3_LED_GPIO_Port, USART3_LED_Pin, bState);
    break;
  case PORT2:
    HAL_GPIO_WritePin(USART2_LED_GPIO_Port, USART2_LED_Pin, bState);
    break;
  case PORT3:
    HAL_GPIO_WritePin(USART4_LED_GPIO_Port, USART4_LED_Pin, bState);
    break;
  case PORT4:
    HAL_GPIO_WritePin(USART6_LED_GPIO_Port, USART6_LED_Pin, bState);
    break;
  }
}
/*
Brief: Change Direction of MAX485 port
Todo: Add HAL control GPIO HW
*/
void RS485SetDir(UCHAR ucPort, ACTION action)
{

  switch (ucPort)
  {
  case PORT1:
    HAL_GPIO_WritePin(USART3_RDE_GPIO_Port, USART3_RDE_Pin, (GPIO_PinState)action);
    break;
  case PORT2:
    HAL_GPIO_WritePin(USART2_RDE_GPIO_Port, USART2_RDE_Pin, (GPIO_PinState)action);
    break;
  case PORT3:
    HAL_GPIO_WritePin(UART4_RDE_GPIO_Port, UART4_RDE_Pin, (GPIO_PinState)action);
    break;
  case PORT4:
    HAL_GPIO_WritePin(USART6_RDE_GPIO_Port, USART6_RDE_Pin, (GPIO_PinState)action);
    break;
  default:
    break;
  }
}
/*
Brief: Return uart handle 
*/
void UARTHandleSwitch(UCHAR ucPort)
{
  switch (ucPort)
  {
  case PORT1:
    phuart = &huart3;
    //memcpy(phuart, &huart3, sizeof(UART_HandleTypeDef));
    break;
  case PORT2:
    phuart = &huart2;
    //memcpy(phuart, &huart2, sizeof(UART_HandleTypeDef));
    break;
  case PORT3:
    phuart = &huart4;
    //memcpy(phuart, &huart4, sizeof(UART_HandleTypeDef));
    break;
  case PORT4:
    phuart = &huart6;
    // memcpy(phuart, &huart6, sizeof(UART_HandleTypeDef));
    break;
  default:
    break;
  }
}