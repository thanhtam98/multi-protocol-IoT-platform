/*
 * Brief: Default app folked from rtos default task
 *
 *
 *
 *
 * */

#include "app.h"
/**/
osThreadId modbusTaskHandle;
osThreadId masterMbHandle;
osThreadId loraTaskHandle;
osThreadId envTaskHandle;
osThreadId cmdTaskHandle;
osThreadId btnTaskHandle;
//osMessageQId btnQueueHandle;

void vAppDefault(void) {

	/*Struct information*/
	iot_node_t iot_node_handle;

	/*Queue initialization**/
	osMessageQDef(btnQueue, 1, sw_data_t);
	iot_node_handle.btnQueueHandle = osMessageCreate(osMessageQ(btnQueue),
			NULL);
	osMessageQDef(ledQueue, 1, led_control_t);
	iot_node_handle.ledQueueHandle = osMessageCreate(osMessageQ(ledQueue),
			NULL);
//	iot_node.envQueueHandle
	/*Task initialization*/
//
	osThreadDef(loraTaskHandle, vRakTask, osPriorityNormal, 0, 512);
	loraTaskHandle = osThreadCreate(osThread(loraTaskHandle),
			(void*) &iot_node_handle);
	DBG("\r\n MemFree: %d", xPortGetFreeHeapSize());
	osThreadDef(modbusTaskHandle, vModBusTask, osPriorityNormal, 0, 256);
	modbusTaskHandle = osThreadCreate(osThread(modbusTaskHandle), (void*)&iot_node_handle);
	DBG("\r\n MemFree: %d", xPortGetFreeHeapSize());
	osThreadDef(envTaskHandle, vEnvTask, osPriorityNormal, 0, 256);
	envTaskHandle = osThreadCreate(osThread(envTaskHandle),
			(void*) &iot_node_handle);
	DBG("\r\n MemFree: %d", xPortGetFreeHeapSize());
	osThreadDef(cmdTaskHandle, vCmdTask, osPriorityNormal, 0, 256);
	cmdTaskHandle = osThreadCreate(osThread(cmdTaskHandle), (void*)&iot_node_handle);
	DBG("\r\n MemFree: %d", xPortGetFreeHeapSize());

	osThreadDef(btnTaskHandle, vBtnTask, osPriorityNormal, 0, 256);
	btnTaskHandle = osThreadCreate(osThread(btnTaskHandle),
			(void*) &iot_node_handle);

	DBG("\r\n MemFree: %d", xPortGetFreeHeapSize());
//	osThreadDef(masterMbHandle, vTestModbusTask, osPriorityNormal, 0, 512 );
//	masterMbHandle = osThreadCreate(osThread(masterMbHandle),  (void*)&iot_node_handle);
//	DBG("\r\n MemFree: %d", xPortGetFreeHeapSize());


	float dtemp = 0;
	while (1) {
		/*---- Main task controller ------------*/

		osDelay(1000);
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
		osDelay(1000);
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
		osDelay(1000);
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
		osDelay(1000);
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);


//Temporary delay for another task done


	}
}

void vAppConfiguration(void) {

	while (1) {

	};
	/* Reset system to apply */
	u_mem_set(NODE_HAVE_PARAM_ADR, 1);

	NVIC_SystemReset();
}

void HAL_GPIO_WRITE_ODR(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) {
	/* Check the parameters */
	assert_param(IS_GPIO_PIN(GPIO_Pin));

	GPIOx->ODR |= GPIO_Pin;
}
void HAL_I2C_ClearBusyFlagErrata_2_14_7(I2C_HandleTypeDef *hi2c) {

	static uint8_t resetTried = 0;
	if (resetTried == 1) {
		return;
	}
	uint32_t SDA_PIN = GPIO_PIN_9;
	uint32_t SCL_PIN = GPIO_PIN_8;
	GPIO_InitTypeDef GPIO_InitStruct;

// 1
	__HAL_I2C_DISABLE(hi2c);

// 2
	GPIO_InitStruct.Pin = SDA_PIN | SCL_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WRITE_ODR(GPIOB, SDA_PIN);
	HAL_GPIO_WRITE_ODR(GPIOB, SCL_PIN);

// 3
	GPIO_PinState pinState;
	if (HAL_GPIO_ReadPin(GPIOB, SDA_PIN) == GPIO_PIN_RESET) {
		for (;;) {
		}
	}
	if (HAL_GPIO_ReadPin(GPIOB, SCL_PIN) == GPIO_PIN_RESET) {
		for (;;) {
		}
	}

// 4
	GPIO_InitStruct.Pin = SDA_PIN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_TogglePin(GPIOB, SDA_PIN);

// 5
	if (HAL_GPIO_ReadPin(GPIOB, SDA_PIN) == GPIO_PIN_SET) {
		for (;;) {
		}
	}

// 6
	GPIO_InitStruct.Pin = SCL_PIN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_TogglePin(GPIOB, SCL_PIN);

// 7
	if (HAL_GPIO_ReadPin(GPIOB, SCL_PIN) == GPIO_PIN_SET) {
		for (;;) {
		}
	}

// 8
	GPIO_InitStruct.Pin = SDA_PIN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WRITE_ODR(GPIOB, SDA_PIN);

// 9
	if (HAL_GPIO_ReadPin(GPIOB, SDA_PIN) == GPIO_PIN_RESET) {
		for (;;) {
		}
	}

// 10
	GPIO_InitStruct.Pin = SCL_PIN;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_GPIO_WRITE_ODR(GPIOB, SCL_PIN);

// 11
	if (HAL_GPIO_ReadPin(GPIOB, SCL_PIN) == GPIO_PIN_RESET) {
		for (;;) {
		}
	}

// 12
	GPIO_InitStruct.Pin = SDA_PIN | SCL_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
//    GPIO_InitStruct.Alternate = NUCLEO_I2C_EXPBD_SCL_SDA_AF;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

// 13
	hi2c->Instance->CR1 |= I2C_CR1_SWRST;

// 14
	hi2c->Instance->CR1 ^= I2C_CR1_SWRST;

// 15
	__HAL_I2C_ENABLE(hi2c);

	resetTried = 1;
}

