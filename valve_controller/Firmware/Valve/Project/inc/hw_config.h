#ifndef HW_CONFIG__H_
#define HW_CONFIG__H_

#include "stdint.h"
/****************************************************************************/
/*                      PUBLIC TYPES and DEFINITIONS                       */
/****************************************************************************/
#define MAX_LED 2
enum{
	OFF=0,
	ON,
	BLK,
	LONG_BLK
};
//typedef struct {
//	uint8_t numTime;
//	uint8_t timeEffect;
//	uint8_t timerCounter;
//	uint8_t ledEffect;
//} led_object_t;
typedef struct{
	uint8_t  numFreq;
	uint16_t numCounter;
	uint16_t numTime;
	uint8_t  profileNum;
}led_object_t;

typedef struct {
	uint8_t ledNum;
	uint8_t profileNum;
} Data_led_t;
//typedef enum
//{ Bit_RESET = 0,
//  Bit_SET
//}BitAction;
/* -------------------------------------------------------------------------- 
** Delay TIMER configuration
** -------------------------------------------------------------------------- */

/****************************************************************************/
/*                      PRIVATE TYPES and DEFINITIONS                       */
/****************************************************************************/


/* -------------------------------------------------------------------------- 
** Delay TIMER configuration
** -------------------------------------------------------------------------- */

#define TIMER_DELAY                                     TIM4
#define TIMER_DELAY_CLOCK                               RCC_APB1Periph_TIM4
/* Delay ms */
#define TIMER_DELAY_PERIOD                              71
#define TIMER_DELAY_PRESCALER                           1000
/* Delay us */
#define TIMER_US_DELAY_PERIOD                           35
#define TIMER_US_DELAY_PRESCALER                        1

	/* Define ZWAVE communication */
	
	#define ZM_UART                   	UART5
  #define ZM_UART_RxGPIO              GPIOD
	#define ZM_UART_TxGPIO              GPIOC
	
  #define ZM_UART_CLK               RCC_APB1Periph_UART5
  #define ZM_UART_GPIO_TxCLK        RCC_APB2Periph_GPIOC
	#define ZM_UART_GPIO_RxCLK        RCC_APB2Periph_GPIOD
  #define ZM_UART_RxPin             GPIO_Pin_2
  #define ZM_UART_TxPin             GPIO_Pin_12
  #define ZM_UART_IRQn              UART5_IRQn
  #define ZM_UART_IRQHandler        UART5_IRQHandler
	/* Define DBG communication */
	
	#define DBG_UART                   USART2
  #define DBG_UART_GPIO              GPIOA
  #define DBG_UART_CLK               RCC_APB1Periph_USART2
  #define DBG_UART_GPIO_CLK          RCC_APB2Periph_GPIOA
  #define DBG_UART_RxPin             GPIO_Pin_3
  #define DBG_UART_TxPin             GPIO_Pin_2
  #define DBG_UART_IRQn              USART2_IRQn
  #define DBG_UART_IRQHandler        USART2_IRQHandler
	/* Define Buttons input */
	
#define BTN_1_Pin GPIO_Pin_4
#define BTN_1_GPIO_Port GPIOA
#define BTN_2_Pin GPIO_Pin_5
#define BTN_2_GPIO_Port GPIOA
#define BTN_3_Pin GPIO_Pin_6
#define BTN_3_GPIO_Port GPIOA
#define BTN_GPIO_Port   GPIOA
	/* Define LEDs output */
#define LED2_Pin GPIO_Pin_7
#define LED2_GPIO_Port GPIOC
#define LED1_Pin GPIO_Pin_8
#define LED1_GPIO_Port GPIOC
#define LED_GPIO_Port GPIOC

	/* Define Motor driver */




#define MOTOR3_SB_Pin GPIO_Pin_0
#define MOTOR3_SB_GPIO_Port GPIOA
#define MOTOR3_SA_Pin GPIO_Pin_1
#define MOTOR3_SA_GPIO_Port GPIOA
#define MOTOR3_A_Pin GPIO_Pin_1
#define MOTOR3_A_GPIO_Port GPIOB
#define MOTOR3_B_Pin GPIO_Pin_0
#define MOTOR3_B_GPIO_Port GPIOB
#define MOTOR3_ENC_GPIO_Port GPIOB
#define MOTOR3_ENC_Pin GPIO_Pin_13

#define MOTOR4_SA_Pin GPIO_Pin_4
#define MOTOR4_SA_GPIO_Port GPIOC
#define MOTOR4_SB_Pin GPIO_Pin_5
#define MOTOR4_SB_GPIO_Port GPIOC
#define MOTOR4_ENC_Pin GPIO_Pin_12
#define MOTOR4_ENC_GPIO_Port GPIOB
#define MOTOR4_A_Pin GPIO_Pin_4
#define MOTOR4_A_GPIO_Port GPIOB
#define MOTOR4_B_Pin GPIO_Pin_5
#define MOTOR4_B_GPIO_Port GPIOB


#define MOTOR2_SA_Pin GPIO_Pin_2
#define MOTOR2_SA_GPIO_Port GPIOC
#define MOTOR2_SB_Pin GPIO_Pin_3
#define MOTOR2_SB_GPIO_Port GPIOC
#define MOTOR2_ENC_Pin GPIO_Pin_15
#define MOTOR2_ENC_GPIO_Port GPIOB
#define MOTOR2_A_Pin GPIO_Pin_6
#define MOTOR2_A_GPIO_Port GPIOB
#define MOTOR2_B_Pin GPIO_Pin_7
#define MOTOR2_B_GPIO_Port GPIOB

#define MOTOR1_ENC_Pin GPIO_Pin_14
#define MOTOR1_ENC_GPIO_Port GPIOB
#define MOTOR1_B_Pin GPIO_Pin_9
#define MOTOR1_B_GPIO_Port GPIOB
#define MOTOR1_A_Pin GPIO_Pin_8
#define MOTOR1_A_GPIO_Port GPIOB
#define MOTOR1_SA_Pin GPIO_Pin_0
#define MOTOR1_SA_GPIO_Port GPIOC
#define MOTOR1_SB_Pin GPIO_Pin_1
#define MOTOR1_SB_GPIO_Port GPIOC
/****************************************************************************/
/*                      PUBLIC FUNCTION PROTOTYPES                    */
/****************************************************************************/
extern void STM32F1_HW_Init(void);

/****************************************************************************/
/*                      PRIVATE FUNCTION PROTOTYPE   S                    */
/****************************************************************************/
static void UARTs_Init(void) ;
static void BTNs_Init(void);
static void LEDs_Init(void);
static void RTC_Configuration(void);
static void RTC_Init(void);
static void Time_Adjust(void);


void LED_Control(uint8_t led, uint8_t state);
void LED_Toggle(uint8_t led);
#endif