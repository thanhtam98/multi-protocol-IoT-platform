/**
  ******************************************************************************
  * @file    Project/User/main.c 
  * @author  NTT - BKSTEK Team
  * @version V1.0
  * @date    15-11-2019
  * @brief   Main program body
	*/
	

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"
#include "stm32f10x_rtc.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include "eeprom.h"
#include "hw_config.h"
#include "valve.h"
#include "buttons.h"
#include "main.h"
#include "zwave.h"

/* Private typedef -----------------------------------------------------------*/
enum
{
  VALVE_START=0,
  VALVE_ZW_CONNECT,
  VALVE_ON,
  VALVE_OFF,
  VALVE_FACTORYRESET,
  VALVE_IDE,
}VALVE_MODE;
/* Private define ------------------------------------------------------------*/
#define xBTN1 1
#define xBTN2 2
#define xBTN3 3
/*--------------- Tasks Priority -------------*/     

/* Private macro -------------------------------------------------------------*/
#define uxPriority 2
/* Private variables ---------------------------------------------------------*/
//led_object_t LedObjectProfile[3]
ValveHandles_t ValveHandles;
/* Declare a variable of type xQueueHandle.  This is used to store the queue
that is accessed by all three tasks. */



//motor_t MOTOR_INSTANT[MAX_MOTOR]; 
/* Private function prototypes -----------------------------------------------*/


// FreeRTOS tasks
static void vTaskLED(void *pvParameters);
static void vTaskUART(void *pvParameters);
static void vTaskZWAVE(void *pvParameters);

/* Main function -------------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
	int main(void) {

		SystemInit();
    /* Setup STM32F103 clock, PLL and Flash configuration) */
		
		
    /* Initialize STM32 peripherals (clocks, GPIO, NVIC) resources */
		STM32F1_HW_Init();
		printf("\r\n**STM32F103 HW init successful** \r\n")	;	
    /* Free RTOS *//////
		//ValveHandles.serialPortMutex = xSemaphoreCreateMutex();		
	//	ValveHandles.zwPortMutex		= xSemaphoreCreateMutex();
//    xTaskCreate(vMainTestTask, "TEST", configMINIMAL_STACK_SIZE*2, NULL, mainLED_TASK_PRIORITY + 1, NULL);

		ValveHandles.xQueue = xQueueCreate( 3, sizeof(sw_data_t)) ;
		ValveHandles.xQueueControl = xQueueCreate(3, 	sizeof( Data_motor_t ));
		ValveHandles.xQueueReponse = xQueueCreate(3, sizeof(Data_motor_t));
		ValveHandles.xQueueLedIndicate = xQueueCreate(3, sizeof(Data_led_t));
		
		xTaskCreate(vTaskButton1,"Task BTNs", configMINIMAL_STACK_SIZE, (void *)&ValveHandles,uxPriority,NULL);
    xTaskCreate(vTaskLED,"Task LED"      ,configMINIMAL_STACK_SIZE, (void *)&ValveHandles,uxPriority,NULL);
		//xTaskCreate(vTaskUART,"Task UART", configMINIMAL_STACK_SIZE,NULL,1,NULL);
		xTaskCreate(vTaskZmReceiver,"Task ZWAVE", configMINIMAL_STACK_SIZE*2,(void *)&ValveHandles,uxPriority,NULL);
		xTaskCreate(vTaskZmPeriodic,"Task ZWAVE sent period report", configMINIMAL_STACK_SIZE,(void *)&ValveHandles,uxPriority,NULL);
		xTaskCreate(vTaskControlMotor,"Task Control Motor", configMINIMAL_STACK_SIZE*8, (void *) &ValveHandles, uxPriority, 	NULL);
		xTaskCreate(vTaskStopSensorCheck,"Task Read stop sensor",configMINIMAL_STACK_SIZE, (void*)&ValveHandles,uxPriority,NULL);
		xTaskCreate(vTaskMeasure,"Task Measure flow of water", configMINIMAL_STACK_SIZE, (void *)&ValveHandles, uxPriority,   NULL);		
					

					/* Start scheduler */
		
		vTaskStartScheduler();
    /* We should never get here as control is now taken by the scheduler */
	
    for ( ; ; );
}

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  FreeRTOS RGB LED Task
  * @param  pvParameters not used
  * @retval None
  */

//void LEDs_Effect(uint8_t led, led_effect ef )
//{
//	
//}
/**
  * @brief  FreeRTOS RGB LED Task
  * @param  pvParameters not used
  * @retval None
  */
static void vTaskLED(void *pvParameters)
{
		ValveHandles_t *pxValveHandles = (ValveHandles_t*) pvParameters;
	const portTickType xTicksToWait = 100 / portTICK_RATE_MS;
	portBASE_TYPE xStatus;
	Data_led_t xData;
	uint16_t numFreqProfile[4] = {0,0,10,100};
	uint16_t numTimeProfile[4] = {0,0,10,10};
	led_object_t ledObject[MAX_LED];
//	ledObject[0].numFreq =  10;
//	ledObject[1].numFreq =  10;
//	ledObject[0].numTime = 10;
//	ledObject[1].numTime = 10;
	bool invert[MAX_LED]; 
	while(1)
	{
		//xStatus = xQueueSend( pxValveHandles->xQueueLedIndicate, &ledData, xTicksToWait );
		xStatus = xQueueReceive(pxValveHandles->xQueueLedIndicate,&xData,NULL);
		if (xStatus == pdPASS){
				ledObject[xData.ledNum].profileNum = BLK;
				ledObject[xData.ledNum].numFreq = numFreqProfile[xData.profileNum];
				ledObject[xData.ledNum].numTime = numTimeProfile[xData.profileNum];
//			ledObject[xData.ledNum].ledEffect = xData.ledEffect;
//			ledObject[xData.ledNum].timeEffect = 2*xData.timeEffect;
//			ledObject[xData.ledNum].numTime = xData.numTime;
		}	
		for (uint8_t led_num=0; led_num < MAX_LED; led_num ++)
		{
			switch (ledObject[led_num].profileNum)
			{
				case ON:
				{
					LED_Control(led_num, ON );
					break;
				}
				case OFF:
				{
					LED_Control(led_num, OFF );
					break;
				}
				case BLK:
				{
						ledObject[led_num].numCounter++;
					if ( ledObject[led_num].numCounter > ledObject[led_num].numFreq)
					{
						ledObject[led_num].numCounter =0;
						LED_Toggle(led_num);
						//LED_Control(led_num, invert[led_num] );					
						ledObject[led_num].numTime --;
						 if (ledObject[led_num].numTime ==0) 
							{
							ledObject[led_num].profileNum = OFF;
							break;
							}
					}
					
//					ledObject[led_num].timerCounter++;
//					if (ledObject[led_num].timerCounter > ledObject[led_num].timeEffect)
//					{
//						ledObject[led_num].timerCounter = 0;
//						LED_Toggle(led_num);
//						if (ledObject[led_num].numTime ==0) 
//							{
//							break;
//							}
//						ledObject[led_num].numTime --;
//						if (ledObject[led_num].numTime ==0 ) 
//							{
//							ledObject[led_num].ledEffect = OFF;
//							}
						break;
					}
					
					
				}
				
		}
			vTaskDelay(1); 
	}
			
}



/**
  * @brief  FreeRTOS UART Test Task
  * @param  pvParameters not used
  * @retval None
  */



static void vTaskUART(void *pvParameters)
{
	ValveHandles_t *pxValveHandles = (ValveHandles_t*) pvParameters;
	
	uint32_t THH = 0, TMM = 0, TSS = 0;
		char data= 0xff;
	uint32_t TimeVar;
	while (1)
	{		
		
		//printf("DBG_UART periodic task \r\n");
		TimeVar= RTC_GetCounter();
		  /* Compute  hours */
  THH = TimeVar / 3600;
  /* Compute minutes */
  TMM = (TimeVar % 3600) / 60;
  /* Compute seconds */
  TSS = (TimeVar % 3600) % 60;

		printf("Time: %0.2d:%0.2d:%0.2d\r", THH, TMM, TSS);
		
		vTaskDelay(5000);		
	}
}
/**
  * @brief  FreeRTOS Zwave protocol communication Task
  * @param  pvParameters not used
  * @retval None
  */

/**
  * @brief  FreeRTOS Zwave protocol communication Task
  * @param  pvParameters not used
  * @retval None
  */


void vApplicationIdleHook( void ) {
}

void vApplicationTickHook( void ) {

}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  printf("Wrong parameters value: file %s on line %d\r\n", file, line);
  /* Infinite loop */
  while (1)
  {}
}
#endif
void vApplicationStackOverflowHook(void)
{
	taskDISABLE_INTERRUPTS();
	printf("FreeRTOS freeHeapsize = %d\n",xPortGetFreeHeapSize());
	for( ;; );
}

void vApplicationMallocFailedHook( void )
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	printf("FreeRTOS freeHeapsize = %d\n",xPortGetFreeHeapSize());
	for( ;; );
}

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
	//xSemaphoreTake( serialPortMutex, portMAX_DELAY);
  USART_SendData(DBG_UART, (uint8_t) ch);
	//xSemaphoreGive(serialPortMutex);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(DBG_UART, USART_FLAG_TC) == RESET)
  {}

  return ch;
}
/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
