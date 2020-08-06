/**
 ***************************************************
 *
 *@brief 		Sensor humi and temp DHT11 using HAL
 *@data 			2018.04
 *@author		cuong vuong.
 *@file 			bsp_dht11.c
 *
 *Porting to FreeRTOS by NTT.
 *
 ***************************************************
 **/
#include <stdio.h>
/* Platform includes ---------------------------------*/
#include "dht.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tim.h"
/* */
/* Private variables ----------------------------------*/
unsigned int T_H, T_L, H_H, H_L, Check;

/* Start implementation ---------------------------------*/
void DHT_Set_Output(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = DHT11_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}
void DHT_Set_Input(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = DHT11_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
}

int DH11_ReadByte(void) {
	unsigned int data = 0, i, cout;

	for (i = 0; i < 8; i++) {
		cout = 1;
		while (!DHT_ReadPin() && cout++) {
			delay_us(1);
			if (cout > 100)
				break;
		}

		delay_us(40);
		data = data << 1;
		if (DHT_ReadPin() == GPIO_PIN_SET) {

			data |= 1;
		}

		cout = 1;
		while (DHT_ReadPin() && cout++) {
			delay_us(1);
			if (cout > 100)
				break;
		}

	}
	return data;
}

void DHT11_ReadData(DHT11_DATA_TypeDef *DHT11_DATA) {

	unsigned int cout = 1;
	HAL_TIM_Base_Start(&htim4);
	DHT_Set_Output();

	DHT_LowPin();
	HAL_Delay(20);

	DHT_HignPin();
	delay_us(80);

	DHT_Set_Input();
	taskENTER_CRITICAL();
	if (DHT_ReadPin() == 0) {
		cout = 1;
		while (!DHT_ReadPin() && cout++) {
			delay_us(1);
			if (cout > 100)
				break;
		}

		cout = 1;
		while (DHT_ReadPin() && cout++) {
			delay_us(1);
			if (cout > 100)
				break;
		}

		DHT11_DATA->humi_int = DH11_ReadByte();
		DHT11_DATA->humi_deci = DH11_ReadByte();
		DHT11_DATA->temp_int = DH11_ReadByte();
		DHT11_DATA->temp_deci = DH11_ReadByte();
		DHT11_DATA->check_sum = DH11_ReadByte();
		if (DHT11_DATA->check_sum
				== (DHT11_DATA->humi_int + DHT11_DATA->humi_deci
						+ DHT11_DATA->temp_int + DHT11_DATA->temp_deci)) {
//			DBG("\r\n Get ok: Humid %d,%d Temp %d,%d ", DHT11_DATA->humi_int,
//					DHT11_DATA->humi_deci, DHT11_DATA->temp_int,
//					DHT11_DATA->temp_deci);
		} else {
			DHT11_DATA->check_sum = 00;
//			DBG(
//					"\r\n Get fail:  HumidH %d HumidL %d TempH %d TempL %d Check %d ",
//					DHT11_DATA->humi_int, DHT11_DATA->humi_deci,
//					DHT11_DATA->temp_int, DHT11_DATA->temp_deci,
//					DHT11_DATA->check_sum);
		}
		DHT_Set_Output();
		DHT_HignPin();
	} else {
//		DBG("\r\n Fail to start DHT");
		DHT_Set_Output();
		DHT_HignPin();
	}
	taskEXIT_CRITICAL();
	HAL_TIM_Base_Stop(&htim4);
}

