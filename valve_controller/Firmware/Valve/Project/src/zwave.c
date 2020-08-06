/**
  ******************************************************************************
  * @file    Project/User/zwave
  * @author  NTT - BKSTEK Team
  * @version V1.0
  * @date    15-11-2019
  * @brief   Zwave functions description
	*/
/*Standard library include*/
#include <stdio.h>
#include <string.h>

/*FreeROTS library include*/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

/*STM32 library include*/
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"
#include "stm32f10x_rtc.h"

/*Valve Controller libary include*/
#include "zwave.h"
#include "buttons.h"
#include "uart_command.h"
#include "Valve.h"
#include "main.h"

/* Private variables ---------------------------------------------------------*/
zwave_mode_t Zwave_mode = ZWAVE_IDLE;
extern T_CON_TYPE cmd_ready;
extern BYTE serBuf[SERBUF_MAX];

/**
  * @brief  FreeRTOS Zwave protocol communication Task
  * @param  pvParameters not used
  * @retval None
  */
void vTaskZmReceiver(void *pvParameters)
{
		  ValveHandles_t *pxValveHandles = (ValveHandles_t*) pvParameters;

	const portTickType xTicksToWait = 100 / portTICK_RATE_MS;
	portBASE_TYPE xStatus;
	Data_motor_t data_control;
	Data_motor_t data_reponse;
	ZW_UART_COMMAND uart_cmd;
	BOOL ZW_ready =FALSE;
  
  BOOL ZW_appversion_send = FALSE;
  BOOL ZW_getconnect_send = FALSE;
	uint8_t tryCounter = 0;
	sw_data_t swData;
	while (1)
	{
		switch(Zwave_mode)
		{
			case ZWAVE_START:
			{
				
				break;
			}
			case ZWAVE_RESET:
			{
				uart_cmd.zw_uartcommandset.length = 3;
        uart_cmd.zw_uartcommandset.cmd =COMMAND_ZWAVE_RESET;
        uart_cmd.zw_uartcommandset.type =  ZW_SET_RESET;
        if (Uart_send_command(uart_cmd)==0)
							{
									printf("\r\n Failed to send data to ZW \r\n");
									vTaskDelay(500);
								/*
								Reset Zwave device when cannot connect after 10 times trying to connect
								*/
									tryCounter++;
									if(tryCounter == 10) 
										Zwave_mode = ZWAVE_IDLE ;
							}		
					else
							{
								printf("\r\n ZWave communication OK: Sent EXCLUSION \r\n");
									Zwave_mode = ZWAVE_IDLE;
							}
        break;
			}
      case ZWAVE_EXC:
      {
        uart_cmd.zw_uartcommandset.length = 4;
        uart_cmd.zw_uartcommandset.cmd = COMMAND_ZW_CONNECT;
        uart_cmd.zw_uartcommandset.type = ZW_SET_CONNECT;
        uart_cmd.zw_uartcommandset.value1 = ZW_DISCONNECT;
        if (Uart_send_command(uart_cmd)==0)
							{
									printf("\r\n Failed to send data to ZW \r\n");
									vTaskDelay(500);
								/*
								Reset Zwave device when cannot connect after 10 times trying to connect
								*/
									tryCounter++;
									if(tryCounter == 10) 
										Zwave_mode = ZWAVE_IDLE ;
							}		
					else
							{
								printf("\r\n ZWave communication OK: Sent EXCLUSION \r\n");
									Zwave_mode = ZWAVE_IDLE;
							}
        break;
      }
			case ZWAVE_CONNECT:
			{
					uart_cmd.zw_uartcommandset.length = 4;
					uart_cmd.zw_uartcommandset.cmd = COMMAND_ZW_CONNECT;
					uart_cmd.zw_uartcommandset.type = ZW_SET_CONNECT;
					uart_cmd.zw_uartcommandset.value1 =ZW_CONNECT;
					if (Uart_send_command(uart_cmd)==0)
							{
									printf("\r\n Failed to send data to ZW \r\n");
									vTaskDelay(500);
								/*
								Reset Zwave device when cannot connect after 10 times trying to connect
								*/
									tryCounter++;
									if(tryCounter == 10) 
											Zwave_mode = ZWAVE_IDLE ;
							}		
					else
							{
									printf("\r\n Connect ZWave communication OK \r\n");
									Zwave_mode = ZWAVE_IDLE;
							}
							break;
			}
			case ZWAVE_IDLE:
			{
				/*Statement for sending message*/
				//printf("Starting or Handling Zwave device after RESET!");
				xStatus = xQueueReceive(pxValveHandles->xQueue,&swData,NULL );
				if (xStatus == pdPASS)
				{
//				if (swData.swTaskSource == xBTN2)
//				{
//					Zwave_mode = ZWAVE_CONNECT;
//				}
//				if (swData.swTaskSource == xBTN3)
//				{
//					Zwave_mode = ZWAVE_EXC;
//				}
					if (swData.swTaskSource == xBTN1)
					{
						if (swData.swData == DBLONG_PRESS)
						{
							Zwave_mode = ZWAVE_RESET;
						}
						else if ( swData.swData == LONG_PRESS)
						{
							Zwave_mode = ZWAVE_CONNECT;
						}
							
					}
					if (swData.swTaskSource == xBTN2)
					{
						if (swData.swData == LONG_PRESS)
						{
							// send data to queue for motor task 
								data_control.motor_num 	=4;
								data_control.state 			= 255;
								//uint8_t data;
								xStatus = xQueueSend( pxValveHandles->xQueueControl, &data_control, xTicksToWait );
                if( xStatus != pdPASS )
                {
                    printf("Could not send to the queue.\n" );
                }
						}
					}
					if (swData.swTaskSource == xBTN3)
					{
						if (swData.swData == LONG_PRESS)
						{
							// send data to queue for motor task 
								data_control.motor_num 	=4;
								data_control.state 			= 0;
								//uint8_t data;
								//xStatus = xQueueSend( pxValveHandles->xQueueLedIndicate, &ledData, xTicksToWait );
								xStatus = xQueueSend( pxValveHandles->xQueueControl, &data_control, xTicksToWait );
                if( xStatus != pdPASS )
                {
                    printf("Could not send to the queue.\n" );
                }
						}
					}
					
					
					break;					
				}	
				xStatus = xQueueReceive(pxValveHandles->xQueueReponse,&data_reponse,NULL);
					if (xStatus == pdPASS)
				{
					uart_cmd.zw_uartcommandreport.length = 5;
					uart_cmd.zw_uartcommandreport.cmd = COMMAND_SWITCHBINARY  ;
					uart_cmd.zw_uartcommandreport.type = ZW_SWITCHBINARY_REPORT  ;
					uart_cmd.zw_uartcommandreport.value1 =data_reponse.motor_num;
					uart_cmd.zw_uartcommandreport.value2 =data_reponse.state;
					if (Uart_send_command(uart_cmd)==0)
							{
									printf("\r\n Failed to send data to ZW \r\n");
									vTaskDelay(50);
								/*
								Reset Zwave device when cannot connect after 10 times trying to connect
								*/
									tryCounter++;
									if(tryCounter == 10) 
											Zwave_mode = ZWAVE_IDLE ;
							}		
					else
							{
								printf("\r\n Connect ZWave communication: Repones motor state OK  %d  \r\n",data_reponse.motor_num);
									Zwave_mode = ZWAVE_IDLE;
							}
							
					
					break;					
				}	
				/*Statement for receiving message*/
				if (cmd_ready == conFrameReceived)
				{	
					memcpy(&uart_cmd.zw_uartcommand.length,serBuf,sizeof(uart_cmd));
					cmd_ready = conIdle;
					switch (uart_cmd.zw_uartcommand.cmd)
					{
            case COMMAND_STATUS:
            {
              if (uart_cmd.zw_uartcommand.type == ZW_REPORT_STATUS )
              {
                if (uart_cmd.zw_uartcommandreport.value1 == ZW_CONNECT)
                {
									printf(" Zw reponse connected");
                  //ZW is ready  
                }
                else if (uart_cmd.zw_uartcommandreport.value1 == ZW_DISCONNECT)
                {
									printf(" Zw reponse not connected");

                  //ZW not ready
                }                
              }
              break;
            }
            case COMMAND_ZW_CONNECT:
            {
              if(uart_cmd.zw_uartcommand.type ==ZW_REPORT_STATUS)
              {
                if(uart_cmd.zw_uartcommandget.value == ZW_CONNECT)
                {
																		printf(" Zw reponse connected");

                  //ZW is connected
                }
                else if( uart_cmd.zw_uartcommandget.value == ZW_DISCONNECT)
                {
																		printf(" Zw reponse not connected");

                  //ZW is not connected
                }
              }

              break;
            }
            case COMMAND_SWITCHBINARY:
            {
							if (uart_cmd.zw_uartcommandset.type ==ZW_SWITCHBINARY_SET )
							{
								// send data to queue for motor task 
								data_control.motor_num 	=uart_cmd.zw_uartcommandset.value1;
								data_control.state 			= uart_cmd.zw_uartcommandset.value2;
								//uint8_t data;
								xStatus = xQueueSend( pxValveHandles->xQueueControl, &data_control, xTicksToWait );
                if( xStatus != pdPASS )
                {
                    printf("Could not send to the queue.\n" );
                }
								
							}
							

              break;
            }
						case COMMAND_ZWAVE_RESET:
            { 
							printf("Zwave reset.\n" );

              Zwave_mode = ZWAVE_IDLE;
              break;
            }
            
					}
				}
			}
     
				
			
				
		}
  }
}
	
/**
  * @brief  FreeRTOS Zwave Periodic Checking Task
  * @param  pvParameters not used
  * @retval None
  * 
  */
void vTaskZmPeriodic(void *pvParameters)
{
	ValveHandles_t *pxValveHandles = (ValveHandles_t*) pvParameters;

    ZW_UART_COMMAND uart_cmd;
    while (TRUE)
    {
   
//				for (uint8_t motor_num = 0; motor_num < MAX_MOTOR; motor_num ++)
//				{
//					uint8_t *pFloat = (uint8_t*) &pxValveHandles->Flowmeter[motor_num];
//					uart_cmd.zw_uartcommandreport.length = 7;
//					uart_cmd.zw_uartcommandreport.cmd    = COMMAND_METER;
//					uart_cmd.zw_uartcommandreport.type   = ZW_FLOW_REPORT;
//					uart_cmd.zw_uartcommandreport.value1 = motor_num; //ID
//					uart_cmd.zw_uartcommandreport.value2 =*(pFloat++);
//					uart_cmd.zw_uartcommandreport.value3 =*(pFloat++);
//					uart_cmd.zw_uartcommandreport.value4 =*(pFloat++);
//					uart_cmd.zw_uartcommandreport.value5 =*(pFloat);
//				if (Uart_send_command(uart_cmd)==0)
//				{
//					printf("\r\n Failed to send data to ZW: Flowmeter \r\n");
//				}
//				else
//				{
//						printf("\r\n ZWave communication OK: Sent Flowmeter \r\n");
//				}
//				
//				}
				
				 vTaskDelay(10000);
			}
     
}
    








