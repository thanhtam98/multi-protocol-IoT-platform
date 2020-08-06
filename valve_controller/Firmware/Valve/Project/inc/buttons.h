#ifndef _BUTTON_H__
#define _BUTTON_H__

#include "stdint.h"
/* Private define ------------------------------------------------------------*/

#define xBTN1 1
#define xBTN2 2
#define xBTN3 3

#define xMAX_BUTTON_NUM 3
#define MAX_WAITING_MS 100
#define MIN_WAITING_MS 1
/* Private typedef ------------------------------------------------------------*/


typedef     enum 
{PRESSED=0,
RELEASED}  sw_t;
typedef enum{
		SHORT_PRESS = 0,
		LONG_PRESS,
		DBLONG_PRESS,
		DOUBLE_PRESS
} sw_action_t;
typedef enum{
    WAITING = 0,
    PREAMBLE   ,
    SHORT,
    LONG,
		DBLONG,
		LONG_CMP,
    DOUBLE,
		END
} sw_state_t;
typedef struct {
    sw_state_t swState;
    uint16_t   detectTimeInterval;
    uint16_t   resetTimeInterval ;
} sw_object_t;

typedef struct
{
    sw_action_t swData;
    uint8_t swTaskSource;
} sw_data_t;


/* Private function prototype ------------------------------------------------------------*/
sw_t        switchState(int SWnumber);
void vTaskButton1(void *pvParameters);
void vTaskButton2(void *pvParameters);
void vTaskButton3(void *pvParameters);
#endif

