/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern uint8_t PRINTF_EN;


#define DBG(fmt,...) if (PRINTF_EN == 1) printf(fmt,##__VA_ARGS__)


#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void delay_us(uint32_t time);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SW_1_Pin GPIO_PIN_6
#define SW_1_GPIO_Port GPIOA
#define SW_2_Pin GPIO_PIN_7
#define SW_2_GPIO_Port GPIOA
#define OneWire_Pin GPIO_PIN_1
#define OneWire_GPIO_Port GPIOB
#define MB_RDE_Pin GPIO_PIN_2
#define MB_RDE_GPIO_Port GPIOB
#define BTN_1_Pin GPIO_PIN_12
#define BTN_1_GPIO_Port GPIOB
#define BTN_2_Pin GPIO_PIN_13
#define BTN_2_GPIO_Port GPIOB
#define BTN_3_Pin GPIO_PIN_14
#define BTN_3_GPIO_Port GPIOB
#define RAK_RST_Pin GPIO_PIN_8
#define RAK_RST_GPIO_Port GPIOA
#define RAK_TX_Pin GPIO_PIN_9
#define RAK_TX_GPIO_Port GPIOA
#define RAK_RX_Pin GPIO_PIN_10
#define RAK_RX_GPIO_Port GPIOA
#define LED_0_Pin GPIO_PIN_4
#define LED_0_GPIO_Port GPIOB
#define LED_1_Pin GPIO_PIN_5
#define LED_1_GPIO_Port GPIOB
#define LED_2_Pin GPIO_PIN_6
#define LED_2_GPIO_Port GPIOB
#define LED_3_Pin GPIO_PIN_7
#define LED_3_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
