#ifndef _VAVLVE_H__
#define _VAVLVE_H__

#include "hw_config.h"
#include "uart_command.h"
/* Private typedef -----------------------------------------------------------*/
#define MAX_MOTOR 4 
#define FLOWMETER_TIME_SAMPLING 1000 //100ms = 1s
//enum
//{
//  VALVE_START=0,
//  VALVE_ZW_CONNECT,
//  VALVE_ON,
//  VALVE_OFF,
//  VALVE_FACTORYRESET,
//  VALVE_IDE,
//}VALVE_MODE;
typedef enum {
	MOTOR_AT_LEFT = 0,
	MOTOR_AT_RIGHT,
	MOTOR_RUN_LEFT,
	MOTOR_RUN_RIGHT
}state_t;
typedef enum
{
	MOTOR_TYPE_0 = 0,  //no stop sensor - control 	
	MOTOR_TYPE_1       // stop sensor 
}motorType_t;
typedef struct{
	uint8_t motor_num;
	uint8_t state;
} Data_motor_t;

typedef struct {
	state_t STATE;
	uint16_t FlowMeter;
	
}motor_t;
typedef struct{
	uint16_t preVal;
	uint16_t curVal;
	float flowVal;	
} Flow_meter_t;



typedef enum
{
  CMD_CLASS_BIN_OFF = 0x00,
  CMD_CLASS_BIN_ON  = 0xFF
}
CMD_CLASS_BIN_SW_VAL;

/* Public functions ---------------------------------------------------------*/

void Motor_Init(void);
void vTaskControlMotor(void *pvParameters);
/* Private functions ---------------------------------------------------------*/

static void Motor_Run_Right(uint8_t motor_num);
static void Motor_Run_Left(uint8_t motor_num);
static void Motor_Home(uint8_t motor_num);
void Motor_Stop(uint8_t motor_num);
void vTaskControlMotor(void *pvParameters);
void vTaskMeasure(void* pvParameters);
void vTaskStopSensorCheck(void *pvParameters);
#endif