/**
  ******************************************************************************
  * @file    Project/User/hw_config
  * @author  NTT - BKSTEK Team
  * @version V1.0
  * @date    15-11-2019
  * @brief   Button functions
	*/

#include "main.h"
#include "hw_config.h"
#include "buttons.h"
#include "Valve.h"
#include "stm32f10x_exti.h"


#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rtc.h"


extern void STM32F1_HW_Init(void)
{
	
	UARTs_Init();
	BTNs_Init();
	LEDs_Init();
	Motor_Init();
	EEPROM_Init();
	//RTC_Init();
}

static void BTNs_Init(void)
{
	
	  /* GPIOD Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	 /* 
		**BUTTON GPIO CONFIG USING GPIO PERIPH**
	 
    */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin  = BTN_1_Pin|BTN_2_Pin|BTN_3_Pin;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	
	GPIO_Init(BTN_GPIO_Port,&GPIO_InitStructure);
	/* 
		**EXTI CONFIG FOR  BUTTONs **
	 
    */
	EXTI_InitTypeDef   EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource4);
	//GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
	///GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);


//	
	EXTI_InitStructure.EXTI_Line = EXTI_Line4|EXTI_Line5|EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  //EXTI_Init(&EXTI_InitStructure);
	
//	  /* Enable and set EXTI Interrupt to the lowest priority */
	NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x08;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x08;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_Init(&NVIC_InitStructure);
}

static void UARTs_Init(void) {
	
	
	//RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5 , ENABLE);
	RCC_APB1PeriphClockCmd(DBG_UART_CLK , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
	

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
			USART_InitTypeDef USART_InitStructure;
			NVIC_InitTypeDef NVIC_InitStructure;
			GPIO_InitTypeDef GPIO_InitStructure;
  /* 
		**ZWAVE UART CONFIG USING UART5**
	 USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
				- Uart Recive interrupt enabled
    */
			/* Configure DBG Rx as input floating */
		  /* Enable the USART2 Pins Software Remapping */
		//GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE); 
		 
		  /* Configure ZM_UART Tx as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = ZM_UART_TxPin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(ZM_UART_TxGPIO, &GPIO_InitStructure);
		
			GPIO_InitStructure.GPIO_Pin = ZM_UART_RxPin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(ZM_UART_RxGPIO, &GPIO_InitStructure);
			
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(ZM_UART, &USART_InitStructure);
	  /* Enable ZM_UART Receive interrupts */
		USART_ClearFlag(ZM_UART,USART_IT_RXNE);
		USART_ITConfig(ZM_UART, USART_IT_RXNE, ENABLE);
		/* Enable ZM_UART Receive interrupts */
		USART_Cmd(ZM_UART, ENABLE);
    /* NVIC configuation for ZM_UART   */
	

		/* Configure the NVIC Preemption Priority Bits */  
		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
		NVIC_InitStructure.NVIC_IRQChannel = ZM_UART_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		 /* 
		**ZWAVE UART CONFIG USING UART5**
	 USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
				- Uart Recive interrupt enabled
    */
		/* Configure DBG Rx as input floating */
		  /* Enable the USART2 Pins Software Remapping */
		//GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE); 
		 USART_InitTypeDef USART_InitStructure2;
		
			GPIO_InitStructure.GPIO_Pin = DBG_UART_RxPin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(DBG_UART_GPIO, &GPIO_InitStructure);
		  /* Configure DBG Tx as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = DBG_UART_TxPin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(DBG_UART_GPIO, &GPIO_InitStructure);
	
		USART_InitStructure2.USART_BaudRate = 115200;
    USART_InitStructure2.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure2.USART_StopBits = USART_StopBits_1;
    USART_InitStructure2.USART_Parity = USART_Parity_No;
    USART_InitStructure2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure2.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(DBG_UART, &USART_InitStructure2);
	  /* Enable DBG_UART Receive interrupts */
		USART_ITConfig(DBG_UART, USART_IT_RXNE, DISABLE);
		/* Enable DBG_UART   */
		USART_Cmd(DBG_UART, ENABLE);
    /* NVIC configuation for DBG_UART   */
		

		/* Configure the NVIC Preemption Priority Bits */  
	//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
		NVIC_InitStructure.NVIC_IRQChannel = DBG_UART_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
			
		
}
static void LEDs_Init(void)
{
	  /* GPIOD Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	 /* 
		**LED GPIO CONFIG USING GPIO PERIPH**
    */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin  = LED2_Pin|LED1_Pin;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_10MHz;	
	GPIO_Init(LED_GPIO_Port,&GPIO_InitStructure);
	GPIO_SetBits(LED_GPIO_Port,LED2_Pin|LED1_Pin);
}
void LED_Control(uint8_t led, uint8_t state)
{
	
  switch (led)
  {
  case /* constant-expression */ 0:
  {
    /* code */
    GPIO_WriteBit(LED1_GPIO_Port,LED1_Pin,(BitAction)state);
    break;
  }
  case 1:
  {
    GPIO_WriteBit(LED1_GPIO_Port,LED2_Pin,(BitAction)state);
    break;
  }
  }
}
void LED_Toggle(uint8_t led)
{
  switch (led)
  {
  case /* constant-expression */ 0:
  {
    /* code */
    GPIO_WriteBit(LED1_GPIO_Port, LED1_Pin, (BitAction)(1 - GPIO_ReadOutputDataBit(LED1_GPIO_Port, LED1_Pin)));

    break;
  }
  case 1:
  {
    GPIO_WriteBit(LED2_GPIO_Port, LED2_Pin, (BitAction)(1 - GPIO_ReadOutputDataBit(LED2_GPIO_Port, LED2_Pin)));

    break;
  }
  }
}

static void RTC_Init(void)
{
	
  if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
  {
    /* Backup data register value is not correct or not yet programmed (when
       the first time the program is executed) */

    printf("\r\n\n RTC not yet configured....");

    /* RTC Configuration */
    RTC_Configuration();

    printf("\r\n RTC configured....");

    /* Adjust time by values entered by the user on the hyperterminal */
    Time_Adjust();

    BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
  }
  else
  {
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      printf("\r\n\n Power On Reset occurred....");
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      printf("\r\n\n External Reset occurred....");
    }

    printf("\r\n No need to configure RTC....");
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
  }
	if(switchState(1)==0)
	{
		Time_Adjust();
	}
	  /* Clear reset flags */
  RCC_ClearFlag();

  /* Display time in infinite loop */
  //Time_Show();
}
static void RTC_Configuration(void)
{
	
	 /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  BKP_DeInit();

  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}


 uint8_t USART_Scanf(uint32_t value)
{
  uint32_t index = 0;
  uint32_t tmp[2] = {0, 0};

  while (index < 2)
  {
    /* Loop until RXNE = 1 */
    while (USART_GetFlagStatus(DBG_UART, USART_FLAG_RXNE) == RESET)
    {}
    tmp[index++] = (USART_ReceiveData(DBG_UART));
    if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
    {
      printf("\n\rPlease enter valid number between 0 and 9");
      index--;
    }
  }
  /* Calculate the Corresponding value */
  index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
  /* Checks */
  if (index > value)
  {
    printf("\n\rPlease enter valid number between 0 and %d", value);
    return 0xFF;
  }
  return index;
}
static uint32_t Time_Regulate(void)
{
  uint32_t Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;

  printf("\r\n==============Time Settings=====================================");
  printf("\r\n  Please Set Hours");

  while (Tmp_HH == 0xFF)
  {
    Tmp_HH = USART_Scanf(23);
  }
  printf(":  %d", Tmp_HH);
  printf("\r\n  Please Set Minutes");
  while (Tmp_MM == 0xFF)
  {
    Tmp_MM = USART_Scanf(59);
  }
  printf(":  %d", Tmp_MM);
  printf("\r\n  Please Set Seconds");
  while (Tmp_SS == 0xFF)
  {
    Tmp_SS = USART_Scanf(59);
  }
  printf(":  %d", Tmp_SS);

  /* Return the value to store in RTC counter register */
  return((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS));
}

static void Time_Adjust(void)
{
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  /* Change the current time */
  RTC_SetCounter(Time_Regulate());
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}
