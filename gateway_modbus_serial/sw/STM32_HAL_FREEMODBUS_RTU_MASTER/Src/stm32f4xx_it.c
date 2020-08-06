/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32f4xx_it.c
 * @brief   Interrupt Service Routines.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
#include "FreeRTOS.h"
#include "task.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#include "mb.h"
#include "mbport.h"

// extern uint16_t downcounter;
// extern TimerExpiredType TimerExpired[MB_RS485_MAX_PORT];
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ETH_HandleTypeDef heth;
extern TIM_HandleTypeDef htim7;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
extern TIM_HandleTypeDef htim1;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
 * @brief This function handles TIM1 update interrupt and TIM10 global interrupt.
 */
void TIM1_UP_TIM10_IRQHandler(void) {
	/* USER CODE BEGIN TIM1_UP_TIM10_IRQn 0 */

	/* USER CODE END TIM1_UP_TIM10_IRQn 0 */
	HAL_TIM_IRQHandler(&htim1);
	/* USER CODE BEGIN TIM1_UP_TIM10_IRQn 1 */

	/* USER CODE END TIM1_UP_TIM10_IRQn 1 */
}

/**
 * @brief This function handles USART1 global interrupt.
 */
void USART1_IRQHandler(void) {
	/* USER CODE BEGIN USART1_IRQn 0 */
//	uint32_t tmp_flag = __HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE);
//	uint32_t tmp_it_source = __HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_RXNE);
//
//	if ((tmp_flag != RESET) && (tmp_it_source != RESET)) {
//
//
//	}
//	    pxMBMasterFrameCBByteReceived(PORT2);
	UARTIntHandler();
	/* USER CODE END USART1_IRQn 0 */
	HAL_UART_IRQHandler(&huart1);
	/* USER CODE BEGIN USART1_IRQn 1 */
	__HAL_UART_CLEAR_PEFLAG(&huart1);
	/* USER CODE END USART1_IRQn 1 */
}

/**
 * @brief This function handles USART2 global interrupt.
 */
void USART2_IRQHandler(void) {
	/* USER CODE BEGIN USART2_IRQn 0 */
	uint32_t tmp_flag = __HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE);
	uint32_t tmp_it_source = __HAL_UART_GET_IT_SOURCE(&huart2, UART_IT_RXNE);

	if ((tmp_flag != RESET) && (tmp_it_source != RESET)) {
		pxMBMasterFrameCBByteReceived(PORT2);
		__HAL_UART_CLEAR_PEFLAG(&huart2);
		return;
	}

	if ((__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TXE) != RESET)
			&& (__HAL_UART_GET_IT_SOURCE(&huart2, UART_IT_TXE) != RESET)) {
		pxMBMasterFrameCBTransmitterEmpty(PORT2);
		return;
	}

	/* USER CODE END USART2_IRQn 0 */
	HAL_UART_IRQHandler(&huart2);
	/* USER CODE BEGIN USART2_IRQn 1 */

	/* USER CODE END USART2_IRQn 1 */
}

/**
 * @brief This function handles USART3 global interrupt.
 */
void USART3_IRQHandler(void) {
	/* USER CODE BEGIN USART3_IRQn 0 */
	/* USER CODE BEGIN USART2_IRQn 0 */

	uint32_t tmp_flag = __HAL_UART_GET_FLAG(&huart3, UART_FLAG_RXNE);
	uint32_t tmp_it_source = __HAL_UART_GET_IT_SOURCE(&huart3, UART_IT_RXNE);

	if ((tmp_flag != RESET) && (tmp_it_source != RESET)) {
		pxMBMasterFrameCBByteReceived(PORT1);
		__HAL_UART_CLEAR_PEFLAG(&huart3);
		return;
	}

	if ((__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TXE) != RESET)
			&& (__HAL_UART_GET_IT_SOURCE(&huart3, UART_IT_TXE) != RESET)) {
		pxMBMasterFrameCBTransmitterEmpty(PORT1);
		return;
	}

	/* USER CODE END USART2_IRQn 1 */
	/* USER CODE END USART3_IRQn 0 */
	HAL_UART_IRQHandler(&huart3);
	/* USER CODE BEGIN USART3_IRQn 1 */

	/* USER CODE END USART3_IRQn 1 */
}

/**
 * @brief This function handles UART4 global interrupt.
 */
void UART4_IRQHandler(void) {
	/* USER CODE BEGIN UART4_IRQn 0 */
	uint32_t tmp_flag = __HAL_UART_GET_FLAG(&huart4, UART_FLAG_RXNE);
	uint32_t tmp_it_source = __HAL_UART_GET_IT_SOURCE(&huart4, UART_IT_RXNE);

	if ((tmp_flag != RESET) && (tmp_it_source != RESET)) {
		pxMBMasterFrameCBByteReceived(PORT3);
		__HAL_UART_CLEAR_PEFLAG(&huart4);
		return;
	}

	if ((__HAL_UART_GET_FLAG(&huart4, UART_FLAG_TXE) != RESET)
			&& (__HAL_UART_GET_IT_SOURCE(&huart4, UART_IT_TXE) != RESET)) {
		pxMBMasterFrameCBTransmitterEmpty(PORT3);
		return;
	}

	/* USER CODE END UART4_IRQn 0 */
	HAL_UART_IRQHandler(&huart4);
	/* USER CODE BEGIN UART4_IRQn 1 */

	/* USER CODE END UART4_IRQn 1 */
}

/**
 * @brief This function handles TIM7 global interrupt.
 */
void TIM7_IRQHandler(void) {
	/* USER CODE BEGIN TIM7_IRQn 0 */

	if (__HAL_TIM_GET_FLAG(&htim7, TIM_FLAG_UPDATE) != RESET
			&& __HAL_TIM_GET_IT_SOURCE(&htim7, TIM_IT_UPDATE) != RESET) {
		__HAL_TIM_CLEAR_IT(&htim7, TIM_IT_UPDATE);
		// for (ucPort = 0; ucPort < MAX)
		// if (!--downcounter)

		 prvvMasterTIMERExpiredISR();
	}

	/* USER CODE END TIM7_IRQn 0 */
	HAL_TIM_IRQHandler(&htim7);
	/* USER CODE BEGIN TIM7_IRQn 1 */

	/* USER CODE END TIM7_IRQn 1 */
}

/**
 * @brief This function handles Ethernet global interrupt.
 */
void ETH_IRQHandler(void) {
	/* USER CODE BEGIN ETH_IRQn 0 */

	/* USER CODE END ETH_IRQn 0 */
	HAL_ETH_IRQHandler(&heth);
	/* USER CODE BEGIN ETH_IRQn 1 */

	/* USER CODE END ETH_IRQn 1 */
}

/**
 * @brief This function handles USART6 global interrupt.
 */
void USART6_IRQHandler(void) {
	/* USER CODE BEGIN USART6_IRQn 0 */
	uint32_t tmp_flag = __HAL_UART_GET_FLAG(&huart6, UART_FLAG_RXNE);
	uint32_t tmp_it_source = __HAL_UART_GET_IT_SOURCE(&huart6, UART_IT_RXNE);

	if ((tmp_flag != RESET) && (tmp_it_source != RESET)) {
		pxMBMasterFrameCBByteReceived(PORT4);
		__HAL_UART_CLEAR_PEFLAG(&huart6);
		return;
	}

	if ((__HAL_UART_GET_FLAG(&huart6, UART_FLAG_TXE) != RESET)
			&& (__HAL_UART_GET_IT_SOURCE(&huart6, UART_IT_TXE) != RESET)) {
		pxMBMasterFrameCBTransmitterEmpty(PORT4);
		return;
	}
	/* USER CODE END USART6_IRQn 0 */
	HAL_UART_IRQHandler(&huart6);
	/* USER CODE BEGIN USART6_IRQn 1 */

	/* USER CODE END USART6_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
