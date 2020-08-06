/**
  ******************************************************************************
  * @file    Project/User/Zware
  * @author  NTT - BKSTEK Team
  * @version V1.0
  * @date    15-11-2019
  * @brief   include zware function
	*/

#ifndef _ZWAVE__H_
#define _ZWAVE__H_

/* Private typedef -----------------------------------------------------------*/

typedef enum {
	ZWAVE_START = 0,
	ZWAVE_CONNECT,
	ZWAVE_IDLE,
  ZWAVE_RESET,
  ZWAVE_EXC      // zwave exclusion mode
} zwave_mode_t;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private functions prototype ---------------------------------------------------------*/
void vTaskZmReceiver(void *pvParameters);
void vTaskZmPeriodic(void *pvParameters);
#endif 