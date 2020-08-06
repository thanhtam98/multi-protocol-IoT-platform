																			   /**
  ******************************************************************************
  * @file    main.h
  * @author  Vp
  * @version V0.0.1
  * @date    June 2019
  * @brief   This file contains all the functions prototypes for the main.c 
  *          file.
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"

#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include "eeprom.h"

#define USE_SEMAPHORE
#define DISPENSER_SERIAL
/* Exported types ------------------------------------------------------------*/
typedef struct {
	xQueueHandle xQueue;
	xQueueHandle xQueueControl;
	xQueueHandle xQueueReponse;
	xQueueHandle xQueueLedIndicate;
	//float Flowmeter[4];
} ValveHandles_t;
typedef struct {
    uint8_t ucCommand;
    uint8_t ucFrameType;
    struct {
        uint8_t ucLen;
        uint8_t uxData[30];
    } uxMessage;
} SerialMessage_t;
/* Exported constants --------------------------------------------------------*/

/* Exported function prototypes ----------------------------------------------*/
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
