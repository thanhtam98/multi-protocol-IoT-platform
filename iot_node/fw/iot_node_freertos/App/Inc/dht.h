#ifndef __DHT11_H
#define __DHT11_H

#include "main.h"

#define DHT11_GPIO_Port GPIOB
#define DHT11_Pin       GPIO_PIN_1
#define DHT_HignPin()	HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET)
#define DHT_LowPin()	HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET)
#define DHT_ReadPin()	HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)

typedef struct
{
	uint8_t temp_int ;		//so nguyen nhiet do
	uint8_t temp_deci ; 	//so thap phan
	uint8_t humi_int ; 	//So nguyen cua do am
	uint8_t humi_deci ;	//sau so thap phan
	uint8_t check_sum ;  // sum tong
} DHT11_DATA_TypeDef ;

void DHT11_ReadData(DHT11_DATA_TypeDef *DHT11_DATA);

#endif
