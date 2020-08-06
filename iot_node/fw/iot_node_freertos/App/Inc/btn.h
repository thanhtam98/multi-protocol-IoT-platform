#ifndef __BTN_H__
#define __BTN_H__
#include "main.h"

enum {
	xBTN1,
	xBTN2,
	xBTN3
};
typedef enum {
	PRESSED = 0, RELEASED
} sw_t;
typedef enum {
	SHORT_PRESS = 0, LONG_PRESS, DBLONG_PRESS, DOUBLE_PRESS
} sw_action_t;
typedef enum {
	bWAITING = 0, bPREAMBLE, bSHORT, bLONG, bDBLONG, bLONG_CMP, bDOUBLE, bEND
} sw_state_t;
typedef struct {
	sw_state_t swState;
	uint16_t detectTimeInterval;
	uint16_t resetTimeInterval;
} sw_object_t;
typedef struct
{
    sw_action_t swData;
    uint8_t swTaskSource;
} sw_data_t;


#define BTN_MIN_WAITING_MS   1
#define BTN_MAX_WAITING_MS	 100
#define BTN_MAX_NUM			3


/* Function prototype*/
sw_t sw_state(uint8_t swNumber) ;
void vBtnTask(const void *arg);
//typedef void (*u_button_callback_function_t) (uint8_t button_num, uint8_t button_state);


#endif
