/**
  ******************************************************************************
  * @file    TIM2_PWM_DutyCycleConfiguration\main.c
  * @author  MCD Application Team
  * @version  V2.0.1
  * @date     18-November-2011
  * @brief   This file contains the main function for TIM2 PWM_DutyCycleConfiguration
  *          example.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "stm8s.h"
#include "main.h"
#include "hardware_define.h"    //define ports and pin  
#include "uart_command.h"
/**
  * @addtogroup TIM2_PWM_DutyCycleConfiguration
  * @{
  */

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

typedef enum
{
  CMD_CLASS_BIN_OFF = 0x00,
  CMD_CLASS_BIN_ON  = 0xFF
}
CMD_CLASS_BIN_SW_VAL;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern T_CON_TYPE cmd_ready;
extern BYTE serBuf[SERBUF_MAX];

uint32_t Meter_value = 0;
uint32_t Meter_value_pre = 0;
uint16_t Delta_time =0;

uint8_t Valve_mode = VALVE_IDE;
volatile BOOL Flow_status = FALSE;
volatile BOOL Zw_connect_Flag = FALSE;
volatile unsigned char Alarm_mode = 0x01;
/* Private function prototypes -----------------------------------------------*/
void GPIO_Config(void);
void UART_Config(void);

void TIM1_Config(void);
void TIM2_Config(void);
void Motor_control(uint8_t mode);
void Delay_us(uint32_t timer);

/* Private functions ---------------------------------------------------------*/
BOOL Zw_SendAlarm(BOOL Water_Status)
{
    ZW_UART_COMMAND uart_cmd;
    uart_cmd.zw_uartcommandreport.length = 6;
    uart_cmd.zw_uartcommandreport.cmd = COMMAND_ALARM;
    uart_cmd.zw_uartcommandreport.type = ZW_ALARM_REPORT;
    uart_cmd.zw_uartcommandreport.value1 = 0x05;
    if(Water_Status == TRUE)
    {
        uart_cmd.zw_uartcommandreport.value2 = 0x02;
        uart_cmd.zw_uartcommandreport.value3 = 0x02;
    }
    else
    {
        uart_cmd.zw_uartcommandreport.value2 = 0x00;
        uart_cmd.zw_uartcommandreport.value3 = 0x00;
    }
    return Uart_send_command(uart_cmd);
}

BOOL WaterSensor_check(void)
{
  if(GPIO_ReadInputPin(GPIO_EXT_SENSOR,PIN_EXT_SENSOR) == RESET)
  {
    Delay_us(500000);
    if(GPIO_ReadInputPin(GPIO_EXT_SENSOR,PIN_EXT_SENSOR) == RESET)
    {
      return TRUE;
    }
  }
  return FALSE;
}
/* Public functions ----------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  ZW_UART_COMMAND uart_cmd;
  
  BOOL ZW_ready =FALSE;
  
  BOOL ZW_appversion_send = FALSE;
  BOOL ZW_getconnect_send = FALSE;
  
  BOOL ZW_getflow_send = FALSE;
  BOOL ZW_getmeter_send = FALSE;
  BOOL led_status = FALSE;
  
  volatile BOOL Water_Detect = FALSE;
  BOOL CmdAlarmSend_status = FALSE;
  
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);      
  GPIO_Config();
  UART_Config();
  TIM1_Config();
  TIM2_Config();
  
  while (1)
  {
    switch(Valve_mode)
    {
    case VALVE_START:
        if(ZW_appversion_send == FALSE)
        {
          GPIO_WriteLow(GPIO_LED_SYSTEM,PIN_LED_SYSTEM);
          //define the frame
          uart_cmd.zw_uartcommandset.length = 5;
          uart_cmd.zw_uartcommandset.cmd = COMMAND_VERSION;
          uart_cmd.zw_uartcommandset.type = ZW_SET_VERSION;
          uart_cmd.zw_uartcommandset.value1 = APP_VERSION;
          uart_cmd.zw_uartcommandset.value2 = APP_SUB_VERSION;
          
          if(Uart_send_command(uart_cmd)==TRUE)                        //return TRUE when transmitt successful
          {
            ZW_ready = FALSE;
            ZW_appversion_send = TRUE;
          }
        }
        if(ZW_getconnect_send == FALSE)
        {
          if(ZW_ready == TRUE)
          {
            uart_cmd.zw_uartcommandget.length = 3;
            uart_cmd.zw_uartcommandget.cmd = COMMAND_ZW_CONNECT;
            uart_cmd.zw_uartcommandget.type =ZW_GET_CONNECT;
            if(Uart_send_command(uart_cmd)==TRUE)
            {
              ZW_getconnect_send = TRUE;
              ZW_ready = FALSE;
            }
          }
        }
        if(ZW_getflow_send == FALSE)
        {
          if(ZW_ready == TRUE)
          {
            uart_cmd.zw_uartcommandget.length = 3;
            uart_cmd.zw_uartcommandget.cmd = COMMAND_FLOW;
            uart_cmd.zw_uartcommandget.type =ZW_FLOW_GET;
            if(Uart_send_command(uart_cmd)==TRUE)
            {
              ZW_getflow_send = TRUE;
              ZW_ready = FALSE;
            }
          }
        }
        if(ZW_getmeter_send == FALSE)
        {
          if(ZW_ready == TRUE)
          {
            uart_cmd.zw_uartcommandget.length = 3;
            uart_cmd.zw_uartcommandget.cmd = COMMAND_METER;
            uart_cmd.zw_uartcommandget.type =ZW_METER_GET;
            if(Uart_send_command(uart_cmd)==TRUE)
            {
              ZW_getmeter_send = TRUE;
              ZW_ready = FALSE;
            }
          }
        }
        if(ZW_ready == TRUE)
        {
          uart_cmd.zw_uartcommandset.length = 3;
          uart_cmd.zw_uartcommandset.cmd = COMMAND_SWITCHBINARY;
          uart_cmd.zw_uartcommandset.type = ZW_SWITCHBINARY_GET;
          if(Uart_send_command(uart_cmd)==TRUE)
          {
            Valve_mode = VALVE_IDE;
          }
        }
      break;
    case VALVE_FACTORYRESET:
      GPIO_WriteHigh(GPIO_LED_SYSTEM,PIN_LED_SYSTEM);
      if(ZW_ready == TRUE)
      {
        uart_cmd.zw_uartcommandget.length = 3;
        uart_cmd.zw_uartcommandget.cmd = COMMAND_ZWAVE_RESET;
        uart_cmd.zw_uartcommandget.type =ZW_SET_RESET;
        if(Uart_send_command(uart_cmd)==TRUE)
        {
          Valve_mode = VALVE_IDE;
          Delay_us(1000000);
        }
      }
      break;
      
    case VALVE_ON:
      GPIO_WriteLow(GPIO_LED_VALVE_OFF,PIN_LED_VALVE_OFF);
      GPIO_WriteHigh(GPIO_LED_VALVE_ON,PIN_LED_VALVE_ON);
      if(GPIO_ReadInputPin(GPIO_MOTOR_SENSOR_1,PIN_MOTOR_SENSOR_1)==RESET)
      {
        Motor_control(2);
        while(GPIO_ReadInputPin(GPIO_MOTOR_SENSOR_1,PIN_MOTOR_SENSOR_1)==RESET);
        Motor_control(0);
      }
      Valve_mode = VALVE_IDE;
      break;
      
    case VALVE_OFF:
      GPIO_WriteLow(GPIO_LED_VALVE_ON,PIN_LED_VALVE_ON);
      GPIO_WriteHigh(GPIO_LED_VALVE_OFF,PIN_LED_VALVE_OFF);
      if(GPIO_ReadInputPin(GPIO_MOTOR_SENSOR_2,PIN_MOTOR_SENSOR_2)==RESET)
      {
        Motor_control(1);
        while(GPIO_ReadInputPin(GPIO_MOTOR_SENSOR_2,PIN_MOTOR_SENSOR_2)==RESET);
        Motor_control(0);
      }
      Valve_mode = VALVE_IDE;
      break;
      
    case VALVE_ZW_CONNECT:
      if(ZW_ready)
      {
          uart_cmd.zw_uartcommandset.length = 4;
          uart_cmd.zw_uartcommandset.cmd = COMMAND_ZW_CONNECT;
          uart_cmd.zw_uartcommandset.type = ZW_SET_CONNECT;
          if(Zw_connect_Flag == FALSE)
          {
            uart_cmd.zw_uartcommandset.value1 =ZW_CONNECT;
          }
          else
          {
            uart_cmd.zw_uartcommandset.value1 = ZW_DISCONNECT;
          }
          if(Uart_send_command(uart_cmd))
          {
              ZW_ready = FALSE;
          }
      }
      if(led_status == TRUE)
      {
        GPIO_WriteLow(GPIO_LED_ZWAVE,PIN_LED_ZWAVE);
        led_status = FALSE;
      }
      else
      {
        GPIO_WriteHigh(GPIO_LED_ZWAVE,PIN_LED_ZWAVE);
        led_status = TRUE;
      }
      Delay_us(500000);
      break;
      
    case VALVE_IDE:
      if(GPIO_ReadInputPin(GPIO_BUTTON_VALVE_ON,PIN_BUTTON_VALVE_ON)==RESET)    //RESET =0 
      {
        Delay_us(500000);
        if(GPIO_ReadInputPin(GPIO_BUTTON_VALVE_ON,PIN_BUTTON_VALVE_ON)==RESET)
        {
          Valve_mode = VALVE_ON;
          if(ZW_ready)
          {
            uart_cmd.zw_uartcommandset.length = 4;
            uart_cmd.zw_uartcommandset.cmd = COMMAND_SWITCHBINARY;
            uart_cmd.zw_uartcommandset.type = ZW_SWITCHBINARY_SET;
            uart_cmd.zw_uartcommandset.value1 = CMD_CLASS_BIN_ON;
            Uart_send_command(uart_cmd);
          }
        }
      }
      if(GPIO_ReadInputPin(GPIO_BUTTON_VALVE_OFF,PIN_BUTTON_VALVE_OFF)==RESET)
      {
        Delay_us(500000);
        if(GPIO_ReadInputPin(GPIO_BUTTON_VALVE_OFF,PIN_BUTTON_VALVE_OFF)==RESET)
        {
          Valve_mode = VALVE_OFF;
          if(ZW_ready)
          {
            uart_cmd.zw_uartcommandset.length = 4;
            uart_cmd.zw_uartcommandset.cmd = COMMAND_SWITCHBINARY;
            uart_cmd.zw_uartcommandset.type = ZW_SWITCHBINARY_SET;
            uart_cmd.zw_uartcommandset.value1 = CMD_CLASS_BIN_OFF;
            Uart_send_command(uart_cmd);
          }
        }
      }
      if(GPIO_ReadInputPin(GPIO_BUTTON_ZWAVE,PIN_BUTTON_ZWAVE)==RESET)
      {
        Delay_us(500000);
        if(GPIO_ReadInputPin(GPIO_BUTTON_ZWAVE,PIN_BUTTON_ZWAVE)==RESET)
        {
          Valve_mode = VALVE_ZW_CONNECT;
        }
      }
      if(GPIO_ReadInputPin(GPIO_BUTTON_SYSTEM,PIN_BUTTON_SYSTEM)==RESET)
      {
        Delay_us(500000);
        if(GPIO_ReadInputPin(GPIO_BUTTON_SYSTEM,PIN_BUTTON_SYSTEM)==RESET)
        {
          Valve_mode = VALVE_FACTORYRESET;
        }
      }
      if(GPIO_ReadInputPin(GPIO_BUTTON_FLOW,PIN_BUTTON_FLOW)==RESET)
      {
        Delay_us(500000);
        if(GPIO_ReadInputPin(GPIO_BUTTON_FLOW,PIN_BUTTON_FLOW)==RESET)
        {
          if(Flow_status==TRUE)
          {
            GPIO_WriteLow(GPIO_LED_FLOW,PIN_LED_FLOW);
            Flow_status = FALSE;
            TIM1_Cmd(DISABLE);
            TIM2_Cmd(DISABLE);
            TIM2_SetCounter(0);
            TIM1_SetCounter(0);
          }
          else
          {
            GPIO_WriteHigh(GPIO_LED_FLOW,PIN_LED_FLOW);
            Flow_status = TRUE;
            Delta_time = 0;
            Meter_value_pre = Meter_value;
            TIM1_Cmd(ENABLE);
            TIM2_Cmd(ENABLE);
            TIM1_SetCounter(0);
          }
          uart_cmd.zw_uartcommandset.length = 4;
          uart_cmd.zw_uartcommandset.cmd = COMMAND_FLOW;
          uart_cmd.zw_uartcommandset.type = ZW_FLOW_SET;
          uart_cmd.zw_uartcommandset.value1 = Flow_status;
          Uart_send_command(uart_cmd);
        }
      }
      Water_Detect = WaterSensor_check();
      if(Water_Detect != CmdAlarmSend_status)
      {
           if(Zw_connect_Flag == TRUE)
           {
               if(Alarm_mode !=0)
               {
                  if(Zw_SendAlarm(Water_Detect)==TRUE)
                  {
                    CmdAlarmSend_status = Water_Detect;
                  }
               }
           }
      }
      break;
    default:
      break;
    }
    
    //end of switch case
    if(cmd_ready==conFrameReceived) //switch case theo cmd nhan duoc tu ZM
    {
      memcpy(&uart_cmd.zw_uartcommand.length,serBuf,sizeof(uart_cmd));
      cmd_ready = con;
      switch(uart_cmd.zw_uartcommand.cmd)
      {
        case COMMAND_STATUS:
          if(uart_cmd.zw_uartcommand.type == ZW_REPORT_STATUS)
          {
            ZW_ready = TRUE;
          }
          break;
        case COMMAND_ZW_CONNECT:
          if(uart_cmd.zw_uartcommand.type == ZW_REPORT_CONNECT)
          {
            ZW_ready = TRUE;
            if(uart_cmd.zw_uartcommandreport.value1==ZW_CONNECT)
            {
              Zw_connect_Flag = TRUE;
              GPIO_WriteHigh(GPIO_LED_ZWAVE,PIN_LED_ZWAVE);
            }
            else
            {
              Zw_connect_Flag = FALSE;
              GPIO_WriteLow(GPIO_LED_ZWAVE,PIN_LED_ZWAVE);
            }
            if(Valve_mode == VALVE_ZW_CONNECT)
            {
              Valve_mode = VALVE_IDE;
            }
          }
          break;
	case COMMAND_SWITCHBINARY:
          if ((uart_cmd.zw_uartcommand.type == ZW_SWITCHBINARY_SET)||((uart_cmd.zw_uartcommand.type == ZW_SWITCHBINARY_REPORT)))
          {
            if(uart_cmd.zw_uartcommandset.value1 == CMD_CLASS_BIN_ON)
            {
              Valve_mode = VALVE_ON;
            }
            else
            {
              Valve_mode = VALVE_OFF;
            }
          }
          break;
        case COMMAND_ALARM:
          if(uart_cmd.zw_uartcommand.type == ZW_ALARM_SET)
          {
            Alarm_mode = uart_cmd.zw_uartcommandset.value2;
          }
          else if(uart_cmd.zw_uartcommand.type == ZW_ALARM_GET)
          {
            uart_cmd.zw_uartcommandreport.length = 7;
            uart_cmd.zw_uartcommandreport.cmd = COMMAND_ALARM;
            uart_cmd.zw_uartcommandreport.type = ZW_ALARM_REPORT;
            uart_cmd.zw_uartcommandreport.value1 = 0x05;
            uart_cmd.zw_uartcommandreport.value2 = 0x01;
            uart_cmd.zw_uartcommandreport.value3 = Water_Detect?0x01:0x03;
            uart_cmd.zw_uartcommandreport.value4 = 0x00;
            Uart_send_command(uart_cmd);
          }
          break;
        case COMMAND_FLOW:
          if(uart_cmd.zw_uartcommand.type==ZW_FLOW_SET)
          {
            Flow_status = uart_cmd.zw_uartcommandset.value1;
            if(Flow_status)
            {
              GPIO_WriteHigh(GPIO_LED_FLOW,PIN_LED_FLOW);
              TIM1_Cmd(ENABLE);
              TIM2_Cmd(ENABLE);
            }
            else
            {
              GPIO_WriteLow(GPIO_LED_FLOW,PIN_LED_FLOW);
              TIM1_Cmd(DISABLE);
              TIM2_Cmd(DISABLE);
            }
            ZW_ready = TRUE;
          }
          break;
        case COMMAND_METER:
          if(uart_cmd.zw_uartcommand.type==ZW_METER_RESET)
          {
            if(uart_cmd.zw_uartcommandset.value1 == 1)
            {
              Meter_value =0;
              Meter_value_pre =0;
              Delta_time =0;
            }
          }
          else if(uart_cmd.zw_uartcommand.type==ZW_METER_REPORT)
          {
            Meter_value = (uart_cmd.zw_uartcommandreport.value1<<24)|(uart_cmd.zw_uartcommandreport.value2<<16)|\
              (uart_cmd.zw_uartcommandreport.value3<<8)|uart_cmd.zw_uartcommandreport.value4;
            Meter_value_pre = Meter_value;
            ZW_ready = TRUE;
          }
          else if(uart_cmd.zw_uartcommand.type==ZW_METER_GET)
          {
              uart_cmd.zw_uartcommandreport.length = 16;
              uart_cmd.zw_uartcommandreport.cmd = COMMAND_METER;
              uart_cmd.zw_uartcommandreport.type = ZW_METER_REPORT;
              uart_cmd.zw_uartcommandreport.value1 = (0x01<<5)|0x03;
              uart_cmd.zw_uartcommandreport.value2 = (2<<5)|0x04;
              uart_cmd.zw_uartcommandreport.value3 = Meter_value>>24;
              uart_cmd.zw_uartcommandreport.value4 = (Meter_value>>16)&0xFF;
              uart_cmd.zw_uartcommandreport.value5 = (Meter_value>>8)&0xFF;
              uart_cmd.zw_uartcommandreport.value6 = Meter_value&0xFF;
              uart_cmd.zw_uartcommandreport.value7 = (Delta_time>>8)&0xFF;
              uart_cmd.zw_uartcommandreport.value8 = Delta_time&0xFF;
              uart_cmd.zw_uartcommandreport.value9 = Meter_value_pre>>24;
              uart_cmd.zw_uartcommandreport.value10 = (Meter_value_pre>>16)&0xFF;
              uart_cmd.zw_uartcommandreport.value11 = (Meter_value_pre>>8)&0xFF;
              uart_cmd.zw_uartcommandreport.value12 = Meter_value_pre&0xFF;
              uart_cmd.zw_uartcommandreport.value13 = 0;
              Uart_send_command(uart_cmd);
          }
          break;
        case COMMAND_VERSION:
          if(uart_cmd.zw_uartcommand.type == ZW_GET_VERSION)
          {
            uart_cmd.zw_uartcommandreport.length = 5;
            uart_cmd.zw_uartcommandreport.cmd = COMMAND_VERSION;
            uart_cmd.zw_uartcommandreport.type = ZW_REPORT_VERSION;
            uart_cmd.zw_uartcommandreport.value1 = APP_VERSION;
            uart_cmd.zw_uartcommandreport.value2 = APP_SUB_VERSION;
            Uart_send_command(uart_cmd);
          }
          break;
      case COMMAND_ZWAVE_RESET:
        if(uart_cmd.zw_uartcommand.type == ZW_REPORT_RESET)
        {
          if(uart_cmd.zw_uartcommandreport.value1 == 0x01)
          {
            ZW_appversion_send = FALSE;
            ZW_getconnect_send = FALSE;
            ZW_getflow_send = FALSE;
            ZW_getmeter_send = FALSE;
            Valve_mode = VALVE_START;
          }
        }
        break;
      }
    }
  }
}
/**
  * @brief  Configure GPIO peripheral input/output mode
  * @param  None
  * @retval None
  */
void GPIO_Config(void)
{
    /* Configure LED*/
    GPIO_DeInit(GPIO_LED_SYSTEM);
    GPIO_DeInit(GPIO_LED_VALVE_ON);
    GPIO_DeInit(GPIO_LED_VALVE_OFF);
    GPIO_DeInit(GPIO_LED_FLOW);
    GPIO_DeInit(GPIO_LED_LOW_BATTERY);
    GPIO_DeInit(GPIO_LED_CHARGE);
    GPIO_DeInit(GPIO_LED_ZWAVE);
    
    GPIO_DeInit(GPIO_ZWAVE_INT0);
    GPIO_DeInit(GPIO_ZWAVE_INT1);
    GPIO_DeInit(GPIO_ZWAVE_RESET);
    
    GPIO_DeInit(GPIO_BUTTON_SYSTEM);
    GPIO_DeInit(GPIO_BUTTON_FLOW);
    GPIO_DeInit(GPIO_BUTTON_VALVE_ON);
    GPIO_DeInit(GPIO_BUTTON_VALVE_OFF);
    GPIO_DeInit(GPIO_BUTTON_ZWAVE);
    
    GPIO_Init(GPIO_LED_SYSTEM,PIN_LED_SYSTEM,GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_WriteLow(GPIO_LED_SYSTEM,PIN_LED_SYSTEM);
    
    GPIO_Init(GPIO_LED_VALVE_ON,PIN_LED_VALVE_ON,GPIO_MODE_OUT_PP_LOW_FAST);   
    GPIO_WriteLow(GPIO_LED_VALVE_ON,PIN_LED_VALVE_ON);
    
    GPIO_Init(GPIO_LED_VALVE_OFF,PIN_LED_VALVE_OFF,GPIO_MODE_OUT_PP_LOW_FAST);   
    GPIO_WriteLow(GPIO_LED_VALVE_OFF,PIN_LED_VALVE_OFF);
    
    GPIO_Init(GPIO_LED_FLOW,PIN_LED_FLOW,GPIO_MODE_OUT_PP_LOW_FAST);   
    GPIO_WriteLow(GPIO_LED_FLOW,PIN_LED_FLOW);
    
    GPIO_Init(GPIO_LED_LOW_BATTERY,PIN_LED_LOW_BATTERY,GPIO_MODE_OUT_PP_LOW_FAST);   
    GPIO_WriteLow(GPIO_LED_LOW_BATTERY,PIN_LED_LOW_BATTERY);
    
    GPIO_Init(GPIO_LED_CHARGE,PIN_LED_CHARGE,GPIO_MODE_OUT_PP_LOW_FAST);   
    GPIO_WriteLow(GPIO_LED_CHARGE,PIN_LED_CHARGE);
    
    GPIO_Init(GPIO_LED_ZWAVE,PIN_LED_ZWAVE,GPIO_MODE_OUT_PP_LOW_FAST);   
    GPIO_WriteLow(GPIO_LED_ZWAVE,PIN_LED_ZWAVE);
    
    /* Configure BUTTON*/
    GPIO_Init(GPIO_BUTTON_SYSTEM,PIN_BUTTON_SYSTEM,GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(GPIO_BUTTON_FLOW,PIN_BUTTON_FLOW,GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(GPIO_BUTTON_VALVE_ON,PIN_BUTTON_VALVE_ON,GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(GPIO_BUTTON_VALVE_OFF,PIN_BUTTON_VALVE_OFF,GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(GPIO_BUTTON_ZWAVE,PIN_BUTTON_ZWAVE,GPIO_MODE_IN_PU_NO_IT);
    
    /* Configure ZWAVE*/
    GPIO_Init(GPIO_ZWAVE_RESET,PIN_ZWAVE_RESET,GPIO_MODE_OUT_PP_LOW_FAST);
    GPIO_WriteLow(GPIO_ZWAVE_RESET,PIN_ZWAVE_RESET);
    Delay_us(5000);
    GPIO_WriteHigh(GPIO_ZWAVE_RESET,PIN_ZWAVE_RESET);
    
    /* Configure Sensor*/
    GPIO_Init(GPIO_EXT_SENSOR,PIN_EXT_SENSOR,GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(GPIO_MOTOR_SENSOR_1,PIN_MOTOR_SENSOR_1,GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(GPIO_MOTOR_SENSOR_2,PIN_MOTOR_SENSOR_2,GPIO_MODE_IN_PU_NO_IT);
    
    /* Configure Motor*/
    GPIO_Init(GPIO_TURNR,PIN_TURNR,GPIO_MODE_OUT_PP_LOW_FAST);   
    GPIO_WriteLow(GPIO_TURNR,PIN_TURNR);
    
    GPIO_Init(GPIO_TURNL,PIN_TURNL,GPIO_MODE_OUT_PP_LOW_FAST);   
    GPIO_WriteLow(GPIO_TURNL,PIN_TURNL);
    
    GPIO_Init(GPIO_PWMR,PIN_PWMR,GPIO_MODE_OUT_PP_LOW_FAST);   
    GPIO_WriteHigh(GPIO_PWMR,PIN_PWMR);
    
    GPIO_Init(GPIO_PWML,PIN_PWML,GPIO_MODE_OUT_PP_LOW_FAST);   
    GPIO_WriteHigh(GPIO_PWML,PIN_PWML);
}

/**
  * @brief  Configure UART peripheral
  * @param  None
  * @retval None
  */
void UART_Config(void)
{
  UART1_DeInit();
  UART1_Init(115200,UART1_WORDLENGTH_8D,UART1_STOPBITS_1,UART1_PARITY_NO,UART1_SYNCMODE_CLOCK_DISABLE,UART1_MODE_TXRX_ENABLE);
  UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE);
  enableInterrupts();
  UART1_Cmd(ENABLE);
}

/**
  * @brief  Configure TIM1 peripheral in Captuer mode
  * @param  None
  * @retval None
  */
void TIM1_Config(void)
{
  //TIM1_PrescalerConfig(1,TIM1_PSCRELOADMODE_IMMEDIATE);
  TIM1_TimeBaseInit(0,TIM1_COUNTERMODE_UP,0xFFFF,0);
  TIM1_ETRClockMode2Config(TIM1_EXTTRGPSC_OFF,TIM1_EXTTRGPOLARITY_NONINVERTED,0);
  //TIM1_ITConfig(TIM1_IT_UPDATE,ENABLE);
  
  TIM1_SetCounter(0);
  /* Enable TIM1 */
  //TIM1_Cmd(ENABLE);
  enableInterrupts();
}
	/**
  * @brief  Configure TIM2 peripheral in PWM mode
  * @param  None
  * @retval None
  */

void TIM2_Config(void)
{
  TIM2_DeInit();
  /* Time base configuration */
  TIM2_TimeBaseInit(TIM2_PRESCALER_256,62500);
  TIM2_ARRPreloadConfig(ENABLE);
  TIM2_SetCounter(0);
  TIM2_ITConfig(TIM2_IT_UPDATE,ENABLE);
  TIM2_ClearFlag(TIM2_FLAG_UPDATE);
  /* TIM2 enable counter */
  TIM2_Cmd(DISABLE);
  enableInterrupts();
}



void Motor_control(uint8_t mode)
{
  switch(mode)
  {
  case 0:
    GPIO_WriteLow(GPIO_TURNR,PIN_TURNR);
    GPIO_WriteLow(GPIO_TURNL,PIN_TURNL);
    GPIO_WriteHigh(GPIO_PWMR,PIN_PWMR);
    GPIO_WriteHigh(GPIO_PWML,PIN_PWML);
    break;
  case 1:
    GPIO_WriteHigh(GPIO_TURNR,PIN_TURNR);
    GPIO_WriteLow(GPIO_PWML,PIN_PWML);
    break;
  case 2:
    GPIO_WriteHigh(GPIO_TURNL,PIN_TURNL);
    GPIO_WriteLow(GPIO_PWMR,PIN_PWMR);
    break;
  }
}
/**
  * @brief  Delay
  * @param  None
  * @retval None
  */
void Delay_us(uint32_t timer)
{
  uint32_t u8Number = timer/0xFF;
  timer -=u8Number*0xFF;
  
  if(u8Number>0)
      TIM4_TimeBaseInit(TIM4_PRESCALER_16, 0xFF);
  TIM4_ARRPreloadConfig(ENABLE);
  TIM4_SetCounter(0);
  /*TIM4 counter enable */
  TIM4_Cmd(ENABLE);
  while(u8Number)
  {
    u8Number--;
    while(TIM4_GetFlagStatus(TIM4_FLAG_UPDATE)==0);
    TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  };
  if(timer<0xFF)
    TIM4_TimeBaseInit(TIM4_PRESCALER_16, timer);
  while(TIM4_GetFlagStatus(TIM4_FLAG_UPDATE)==0);
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  TIM4_Cmd(DISABLE);
}


#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
