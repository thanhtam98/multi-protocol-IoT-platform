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
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

extern uint8_t PRINTF_EN;


#define DBG(fmt,...) if (PRINTF_EN == 1) printf(fmt,##__VA_ARGS__)

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define userPHY 0x00
#define PHY_USER_NAME_PHY_ADDRESS 0x00
#define PC14_OSC32_IN_Pin GPIO_PIN_14
#define PC14_OSC32_IN_GPIO_Port GPIOC
#define PC15_OSC32_OUT_Pin GPIO_PIN_15
#define PC15_OSC32_OUT_GPIO_Port GPIOC
#define PH0_OSC_IN_Pin GPIO_PIN_0
#define PH0_OSC_IN_GPIO_Port GPIOH
#define PH1_OSC_OUT_Pin GPIO_PIN_1
#define PH1_OSC_OUT_GPIO_Port GPIOH
#define USART3_RDE_Pin GPIO_PIN_10
#define USART3_RDE_GPIO_Port GPIOD
#define USART3_LED_Pin GPIO_PIN_11
#define USART3_LED_GPIO_Port GPIOD
#define USART6_LED_Pin GPIO_PIN_15
#define USART6_LED_GPIO_Port GPIOD
#define USART6_RDE_Pin GPIO_PIN_8
#define USART6_RDE_GPIO_Port GPIOC
#define USART4_LED_Pin GPIO_PIN_9
#define USART4_LED_GPIO_Port GPIOC
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define UART4_RDE_Pin GPIO_PIN_12
#define UART4_RDE_GPIO_Port GPIOC
#define USART2_LED_Pin GPIO_PIN_3
#define USART2_LED_GPIO_Port GPIOD
#define USART2_RDE_Pin GPIO_PIN_4
#define USART2_RDE_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define GPIO_SPEAKER_Pin GPIO_PIN_4
#define GPIO_SPEAKER_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
typedef enum{
	TASK_NONE= 0,
	TASK_INIT,
	TASK_RUNNING,
	TASK_ERR_1,
	TASK_ERR_2,
	TASK_KILL
}xTaskState_t;

typedef struct{
	osThreadId xTask;
	xTaskState_t xState;
}xQueueControl_t;
typedef union
{
	uint16_t i16data;
	uint8_t  i8data[2];
}int16toint8;
typedef struct{
	uint8_t  PortID;
	uint8_t  NodeID;
	uint8_t  FunC;
	int16toint8 RegAdr;
	int16toint8 RegData;

}xQueueMbMqtt_t;
enum {
	SYS_START,SYS_MB_PROTOCOL, SYS_MB_APP, SYS_NET_TIME,SYS_CORE_DISCOV,SYS_MQTT,SYS_HTTP,SYS_DEFAULT
};
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
