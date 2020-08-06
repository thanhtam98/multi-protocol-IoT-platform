/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "cmsis_os.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "param.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t PRINTF_EN = 1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
void v_led_start(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
//	HAL_I2C_MspInit(&hi2c1);
//	v_led_start();

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, RESET);
	HAL_Delay(10);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, SET);
//while(1);
	/*Force to enable periphral manually */
	char *p = "NTT \r\n";
//	HAL_TIM_Base_Start(&htim4);  //delay_us => move to DHT when it is called to reduce power consuming
//
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
//	 HAL_TIM_Base_Start(&htim3);
	/* Run the ADC calibration */
//	if (HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK) {
//		/* Calibration Error */
//		Error_Handler();
//	}
//  HAL_Delay(101);
  HAL_UART_Transmit(&huart1,p,6,100);
  HAL_UART_Transmit(&huart3,p,6,100);
//  HAL_UART_Transmit(&huart2,p,6,100);
	printf("---------- IOT Node ----------  \r\n");

	/*Load pre-paramter from eepROm*/
	printf(" Loading pre-configuration from EEPROM !!  \r\n");
	v_epr_load(PARAM_LOAD_ALL);
	if ((PARAM[NODE_HAVE_PARAM_ADR] == EEP_PARAM)||(PARAM[NODE_HAVE_PARAM_ADR] == FLASH_PARAM)) {

		/* Found pre-param from eeprom and apply it*/
		printf(" pre-configuration from EEPROM: %d  \r\n",
				PARAM[NODE_HAVE_PARAM_ADR]);
		USER_USART3_UART_Init(); // Modbus re-config
		/*Todo: LORAWAN*/

	} else
	{
		printf(
				" Fail to find pre-configuration. Load defaul config instead.  \r\n");
		/* Load default config**/
		PARAM[NODE_HAVE_PARAM_ADR] = FLASH_PARAM;
		PARAM[NODE_ID_ADR] = NODE_ID_ADR_DEFAULT;
		PARAM[NODE_MB_ID_ADR] = NODE_MB_ID_DEFAULT;

//		PARAM[15] = 69;
//		PARAM[16] = 96;
		PARAM[NODE_LRWAN_DATARATE_ADR] = NODE_LRWAN_DATARATE_DEFAULT;
		PARAM[NODE_LRWAN_CLASS_ADR] = NODE_LRWAN_CLASS_DEFAULT;
		PARAM[NODE_LRWAN_FREQ_ADR] = NODE_LRWAN_FREQ_DEFAULT;
		PARAM[NODE_LRWAN_CONFIRM_ADR] = NODE_LRWAN_CONFIRM_DEFAULT;
		PARAM[NODE_LRWAN_MODE_ADR] = NODE_LRWAN_MODE_DEFAULT;
		PARAM[NODE_LRWAN_TX_POWER_ADR] =NODE_LRWAN_TX_POWER_DEFAULT ;

		PARAM[NODE_IO_PORT_0_ADR] = NODE_IO_PORT_0_DEFAULT;
		PARAM[NODE_IO_PORT_0_ADR + 1] = 0x00;
		PARAM[NODE_IO_PORT_1_ADR] = NODE_IO_PORT_1_DEFAULT;
		PARAM[NODE_IO_PORT_1_ADR + 1] = 0x01;
		PARAM[NODE_IO_PORT_2_ADR] = NODE_IO_PORT_2_DEFAULT;
		PARAM[NODE_IO_PORT_2_ADR + 1] = 0x02;
		PARAM[NODE_IO_PORT_3_ADR] = NODE_IO_PORT_3_DEFAULT;
		PARAM[NODE_IO_PORT_3_ADR + 1] = 0x03;
		PARAM[NODE_IO_PORT_4_ADR] = NODE_IO_PORT_4_DEFAULT;
		PARAM[NODE_IO_PORT_4_ADR + 1] = 0x04;
		PARAM[NODE_IO_PORT_5_ADR] = NODE_IO_PORT_5_DEFAULT;
		PARAM[NODE_IO_PORT_5_ADR + 1] = 0x05;
		v_epr_save(PARAM_LOAD_ALL);
//		memset(PARAM,0,120);
///
		/*Notify to the Led and jump in to vAppConfiguationTask !!**/

	}

	/* Print the configuration before jump into RTOS*/
	printf("Param type   %d \r\n",PARAM[NODE_HAVE_PARAM_ADR]  );
	printf("Node ID	     %d \r\n",PARAM[NODE_ID_ADR]  );
	printf("Modbus ID    %d \r\n",PARAM[NODE_MB_ID_ADR]  );
	printf("LRWAN DR     %d \r\n",PARAM[NODE_LRWAN_DATARATE_ADR]  );
	printf("LRWAN Class  %d \r\n",PARAM[NODE_LRWAN_CLASS_ADR]  );
	printf("LRWAN cf     %d \r\n",PARAM[NODE_LRWAN_CONFIRM_ADR]  );
	printf("LRWAN tx     %d \r\n",PARAM[NODE_LRWAN_TX_POWER_ADR]  );
	printf("LRWAN mode   %d \r\n",PARAM[NODE_LRWAN_MODE_ADR]  );
	printf("Port 1 type  %d \r\n",PARAM[NODE_IO_PORT_0_ADR]  );
	printf("Port 2 type  %d \r\n",PARAM[NODE_IO_PORT_1_ADR]  );
	printf("Port 3 type  %d \r\n",PARAM[NODE_IO_PORT_2_ADR]  );
	printf("Port 4 type  %d \r\n",PARAM[NODE_IO_PORT_3_ADR]  );
	printf("Port 5 type  %d \r\n",PARAM[NODE_IO_PORT_4_ADR]  );
	printf("Port 6 type  %d \r\n",PARAM[NODE_IO_PORT_5_ADR]  );


//	v_epr_load(PARAM_LOAD_ALL);
//	for (uint8_t paramIndex = 0; paramIndex < 120; paramIndex++) {
//		DBG("ParamLoaded %d: %d \r\n", paramIndex, PARAM[paramIndex]);
//		HAL_Delay(10);
//	}
	printf("Booting to RTOS   \r\n");

  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init(); 
  /* Start scheduler */
  osKernelStart();
 
  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void delay_us(uint32_t time) {
	/* */
	__HAL_TIM_SET_COUNTER(&htim4, 0);
	while (__HAL_TIM_GET_COUNTER(&htim4) < time);
}

/* USER CODE BEGIN 4 */
PUTCHAR_PROTOTYPE {
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
//  USART_SendData(EVAL_COM1, (uint8_t) ch);
	HAL_UART_Transmit(&huart2, (uint8_t *) &ch, 1, 0xFFFF);

	/* Loop until the end of transmission */
//  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
//  {}
	return ch;
}

void v_led_start(void) {

	for (uint8_t num = 0; num < 15; num++) {
		HAL_GPIO_TogglePin(LED_0_GPIO_Port, LED_3_Pin);
		HAL_Delay(50);
		HAL_GPIO_TogglePin(LED_0_GPIO_Port, LED_2_Pin);
		HAL_Delay(50);
		HAL_GPIO_TogglePin(LED_0_GPIO_Port, LED_1_Pin);
		HAL_Delay(50);
		HAL_GPIO_TogglePin(LED_0_GPIO_Port, LED_0_Pin);
		HAL_Delay(50);

	}
}
/* USER CODE END 4 */

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
	 tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
