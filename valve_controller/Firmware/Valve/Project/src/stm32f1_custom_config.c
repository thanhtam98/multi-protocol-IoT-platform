/**
  ******************************************************************************
  * @file    stm32f1_custom_config.c
  * @author  Vu Pham
  * @version V0.0.1
  * @date    03/18/2019
  * @brief   STM32F1_CUSTOM_BOARD hardware configuration
  ******************************************************************************
  * @copy
**/
/* Includes ------------------------------------------------------------------*/
#include "stm32f1_custom_config.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



void STM32F1_Peripheral_Init(void)
{
  /* Configure the interrupts */
	Interrupts_Config();

	/* Configure the timers  */
	Timer_Config( );
  
  /* ST 95HF HW Init */
  ConfigManager_HWInit();
  
  /* Initialize USARTs */
	UARTstdio_Configuration(USART2, 9600);   // DEBUG
//  UARTstdio_Configuration(USART3, 115200);   // RS485

  /* Initialize STM32F103-CUSTOM's LEDs */
  STM_EVAL_LEDInit(LEDR1);
  STM_EVAL_LEDInit(LEDG1);
  STM_EVAL_LEDInit(LEDB1);
  STM_EVAL_LEDInit(LEDR2);
  STM_EVAL_LEDInit(LEDG2);
  STM_EVAL_LEDInit(LEDB2);
  STM_EVAL_LEDInit(LEDR3);
  STM_EVAL_LEDInit(LEDG3);
  STM_EVAL_LEDInit(LEDB3);
  /* Turn off leds available on STM32F103-CUSTOM */
  STM_EVAL_LEDOff(LEDR1);
  STM_EVAL_LEDOff(LEDG1);
  STM_EVAL_LEDOff(LEDB1);
  STM_EVAL_LEDOff(LEDR2);
  STM_EVAL_LEDOff(LEDG2);
  STM_EVAL_LEDOff(LEDB2);
  STM_EVAL_LEDOff(LEDR3);
  STM_EVAL_LEDOff(LEDG3);
  STM_EVAL_LEDOff(LEDB3);
  
  /* Initialize STM32F103-CUSTOM's Buzzer */
  STM_EVAL_LEDInit(BUZZR);
  STM_EVAL_GPIOLow(BUZZR);
  
  /* Initialize STM32F103-CUSTOM's Tamper */
  STM_EVAL_PBInit(Button_TAMPER, BUTTON_MODE_GPIO);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */

/******************* (C) COPYRIGHT 2019 Vu Pham *****END OF FILE****/
