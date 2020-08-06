#ifndef __HARDWARE
#define __HARDWARE
#include "stm8s.h"
/*define LED SYSTEM*/
#define GPIO_LED_SYSTEM	        GPIOC
#define	PIN_LED_SYSTEM	        GPIO_PIN_1
/*define LED VALVE*/
#define GPIO_LED_VALVE_ON	GPIOB
#define	PIN_LED_VALVE_ON        GPIO_PIN_1
#define GPIO_LED_VALVE_OFF	GPIOB
#define	PIN_LED_VALVE_OFF       GPIO_PIN_0
/*define LED FLOW*/
#define GPIO_LED_FLOW   	GPIOB
#define	PIN_LED_FLOW            GPIO_PIN_2
/*define LED LOW BATTERY*/
#define GPIO_LED_LOW_BATTERY  	GPIOB
#define	PIN_LED_LOW_BATTERY     GPIO_PIN_6
/*define LED CHARGE*/
#define GPIO_LED_CHARGE  	GPIOB
#define	PIN_LED_CHARGE          GPIO_PIN_7
/*define LED ZWAVE*/
#define GPIO_LED_ZWAVE  	GPIOA
#define	PIN_LED_ZWAVE           GPIO_PIN_3

/*define BUTTON SYSTEM*/
#define GPIO_BUTTON_SYSTEM      GPIOE
#define PIN_BUTTON_SYSTEM	GPIO_PIN_5
/*define BUTTON FLOW*/
#define GPIO_BUTTON_FLOW	GPIOB
#define PIN_BUTTON_FLOW         GPIO_PIN_5
/*define BUTTON VALVE ON*/
#define GPIO_BUTTON_VALVE_ON    GPIOB
#define PIN_BUTTON_VALVE_ON     GPIO_PIN_4
/*define BUTTON VALVE OFF*/
#define GPIO_BUTTON_VALVE_OFF   GPIOC
#define PIN_BUTTON_VALVE_OFF    GPIO_PIN_2
/*define BUTTON ZWAVE*/
#define GPIO_BUTTON_ZWAVE	GPIOF
#define PIN_BUTTON_ZWAVE	GPIO_PIN_4

/*define sensor*/
#define GPIO_EXT_SENSOR         GPIOD
#define PIN_EXT_SENSOR          GPIO_PIN_0
#define GPIO_MOTOR_SENSOR_1     GPIOC
#define PIN_MOTOR_SENSOR_1      GPIO_PIN_4
#define GPIO_MOTOR_SENSOR_2     GPIOC
#define PIN_MOTOR_SENSOR_2      GPIO_PIN_3

/*define MOTOR*/
#define GPIO_TURNR              GPIOD
#define PIN_TURNR               GPIO_PIN_7
#define GPIO_TURNL              GPIOD
#define PIN_TURNL               GPIO_PIN_2
#define GPIO_PWMR               GPIOD
#define PIN_PWMR                GPIO_PIN_3
#define GPIO_PWML               GPIOD
#define PIN_PWML                GPIO_PIN_4

/*define Zwave I/O*/
#define GPIO_ZWAVE_INT0         GPIOC
#define PIN_ZWAVE_INT0          GPIO_PIN_6
#define GPIO_ZWAVE_INT1         GPIOC
#define PIN_ZWAVE_INT1          GPIO_PIN_7
#define GPIO_ZWAVE_RESET        GPIOC
#define PIN_ZWAVE_RESET         GPIO_PIN_5
#endif