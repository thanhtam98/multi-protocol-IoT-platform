/**
  ******************************************************************************
  * @file    stm32f10x_it.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/20/2009
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"

#include "hw_config.h"
#include "uart_command.h"
#include "main.h"
#include "hw_config.h"
#include "Valve.h"


#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define xBTN1 1
#define xBTN2 2
#define xBTN3 3
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
T_CON_TYPE cmd_ready;
uint16_t capture = 0;
uint16_t flowCounter[4];
extern ValveHandles_t ValveHandles;

/**
 *	@brief  this uDataReady variable is set when the RF transceiver sends an interruption on the IRQout pad
 */

/* Private function prototypes -----------------------------------------------*/
extern void xPortSysTickHandler(void);
/* Private functions ---------------------------------------------------------*/


/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  /* Update the LocalTime by adding SYSTEMTICK_PERIOD_MS each SysTick interrupt */
	xPortSysTickHandler();
//  TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
void EXTI0_IRQHandler(void)
{
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	if (GPIO_ReadInputDataBit(MOTOR4_SA_GPIO_Port,MOTOR4_SA_Pin)==0)
	{
		Motor_Stop(3);
		Data_motor_t data;
		data.motor_num = 3;
		data.state = 0x00;
		xQueueSendFromISR(ValveHandles.xQueueReponse	,&data,&xHigherPriorityTaskWoken) ;

	}
	EXTI_ClearFlag(EXTI_Line0);
}
void EXTI1_IRQHandler(void)
{
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	if (GPIO_ReadInputDataBit(MOTOR4_SB_GPIO_Port,MOTOR4_SB_Pin)==0)
	{
		Motor_Stop(3);
		Data_motor_t data;
		data.motor_num = 3;
		data.state = 0xff;
		xQueueSendFromISR(ValveHandles.xQueueReponse,&data,&xHigherPriorityTaskWoken) ;

	}						
	EXTI_ClearFlag(EXTI_Line1);
}
void EXTI2_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	if (GPIO_ReadInputDataBit(MOTOR3_SA_GPIO_Port,MOTOR3_SA_Pin)==0)
	{
		Motor_Stop(2);
		Data_motor_t data;
		data.motor_num = 2;
		data.state = 0x00;
		xQueueSendFromISR(ValveHandles.xQueueReponse	,&data,&xHigherPriorityTaskWoken) ;
		
	}
	EXTI_ClearFlag(EXTI_Line2);
}
void EXTI3_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	if (GPIO_ReadInputDataBit(MOTOR3_SB_GPIO_Port,MOTOR3_SB_Pin)==0)
	{
		Motor_Stop(2);
		Data_motor_t data;
		data.motor_num = 2;
		data.state = 0xff;
		xQueueSendFromISR(ValveHandles.xQueueReponse,&data,&xHigherPriorityTaskWoken) ;
		
	}
	EXTI_ClearFlag(EXTI_Line3);
}
void EXTI4_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	//if (GPIO_ReadInputDataBit(MOTOR3_SA_GPIO_Port,MOTOR3_SA_Pin)==0)
	{
		Motor_Stop(1);
		Data_motor_t data;
		data.motor_num = 1;
		data.state = 0x00;
		xQueueSendFromISR(ValveHandles.xQueueReponse	,&data,&xHigherPriorityTaskWoken) ;
		EXTI_ClearFlag(EXTI_Line4);
	}
//	if (GPIO_ReadInputDataBit(BTN_1_GPIO_Port,BTN_1_Pin) ==0 )
//	{
//			Data_motor_t data ;
//			data.motor_num = 1;
//			data.state = 0;
//			xQueueSendFromISR(ValveHandles.xQueueControl,&data,&xHigherPriorityTaskWoken) ;
//			portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
//		
//		EXTI_ClearFlag(EXTI_Line4);
//		
	//}
	EXTI_ClearFlag(EXTI_Line4);
}
void EXTI9_5_IRQHandler(void)
{
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

		{
		Motor_Stop(1);
		Data_motor_t data;
		data.motor_num = 1;
		data.state = 0x00;
		xQueueSendFromISR(ValveHandles.xQueueReponse	,&data,&xHigherPriorityTaskWoken) ;
		EXTI_ClearFlag(EXTI_Line4);
	}
//	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

//		if (EXTI_GetITStatus(EXTI_Line5)){
//			uint8_t data = xBTN2;
//			xQueueSendFromISR(ValveHandles.xQueue,&data,&xHigherPriorityTaskWoken) ;
//			portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
//			EXTI_ClearFlag(EXTI_Line5);			
//		}
//	if (EXTI_GetITStatus(EXTI_Line6)){
//		
//		uint8_t data = xBTN3;
//			xQueueSendFromISR(ValveHandles.xQueue,&data,&xHigherPriorityTaskWoken) ;
//			portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
//			EXTI_ClearFlag(EXTI_Line6);
//		}

	
}

void EXTI15_10_IRQHandler(void) 
{
	if (EXTI_GetITStatus(EXTI_Line12))
	{
		flowCounter[0]++;
		EXTI_ClearFlag(EXTI_Line12);

		
	}
	if (EXTI_GetITStatus(EXTI_Line13))
	{
		flowCounter[1]++;
		EXTI_ClearFlag(EXTI_Line13);

	}
	if (EXTI_GetITStatus(EXTI_Line14))
	{
		flowCounter[2]++;
		EXTI_ClearFlag(EXTI_Line14);

		
	}
	if (EXTI_GetITStatus(EXTI_Line15))
	{
		flowCounter[3]++;
		EXTI_ClearFlag(EXTI_Line15);

		
	}
}

//void USART2_IRQHandler(void)
//{
//	if(USART_GetITStatus(DBG_UART, USART_IT_RXNE) != RESET)//enter interrupt when STM32 receice data.
//      {
//       //  GPIO_SetBits(LED1_GPIO_Port,LED1_Pin);
//      //printf("UASART2 Interrupt RX");
//			cmd_ready = Uart_update(TRUE);
//			USART_ClearITPendingBit(DBG_UART, USART_IT_RXNE);
//				// GPIO_ResetBits(LED1_GPIO_Port,LED1_Pin);
//      }
//    
//}


void UART5_IRQHandler(void)
{
	if(USART_GetITStatus(ZM_UART, USART_IT_RXNE) != RESET)//enter interrupt when STM32 receice data.
      {
       //  GPIO_SetBits(LED1_GPIO_Port,LED1_Pin);
      //printf("UASART2 Interrupt RX");
			cmd_ready = Uart_update(TRUE);
			USART_ClearITPendingBit(ZM_UART, USART_IT_RXNE);
				// GPIO_ResetBits(LED1_GPIO_Port,LED1_Pin);
      }
    
}
/**
 *	@brief  This function handles the RF transceiver interupts
 *	@brief  RFTRANS_IRQ_HANDLER							EXTI3_IRQHandlers
 */



/**
 * @brief  This function handles the timer interrupt.
 * @param  None
 * @retval None
 */

/**
 * @brief  This function handles the pulse timer interrupt.
 * @param  None
 * @retval None
 */


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
