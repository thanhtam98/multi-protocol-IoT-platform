/* Built-in C library includes ----------------------*/

#include <stdio.h>
#include <stdint.h>
#include "cmsis_os.h"
/* Platform includes --------------------------------*/
#include "main.h"
#include "sharedmem.h"
#include "envtask.h"
#include "dht.h"

/* Shared variables ---------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;

/* Start Implementation -----------------------------*/

void EnvTask(void const *arg)
{
	uint32_t adcRead1;
	uint32_t adcRead2;
	uint32_t temperature;
	float vsense = 3.3/1023;
	float 	temp;
	HAL_ADC_Start(&hadc1);
	DHT11_DATA_TypeDef DHT_DATA;
	while(1)
	{
//		delay_us(1000000);

		HAL_ADC_Start(&hadc1);
		osDelay(1200);
		HAL_ADC_PollForConversion(&hadc1, 100);
		adcRead1 = HAL_ADC_GetValue(&hadc1);
		HAL_ADC_Stop (&hadc1);
		DHT11_ReadData(&DHT_DATA);
//		222222222222printf("\r\n ADC 1 channel 1: %d \r\n", adcRead1 );
//
//		if(Read_TempAndHumidity ( & DHT11_DATA)==SUCCESS)
//				{
//					// HIEN THI GIA TRI
//					printf("\r\nDHT11!\r\n\r\n %d.%d %RH , %d.%d *C \r\n",DHT11_DATA.humi_int,DHT11_DATA.humi_deci,DHT11_DATA.temp_int,DHT11_DATA.temp_deci);
//				}
//				else
//					printf("\r\n??DHT11??\r\n") ;


//		HAL_ADC_PollForConversion(&hadc1, 100);
//		adcRead2 = HAL_ADC_GetValue(&hadc1);
////		printf("\r\n ADC 1 channel 2: %d", adcRead2 );
//
//		HAL_ADC_PollForConversion(&hadc1, 100);
//		temperature = HAL_ADC_GetValue(&hadc1);
//		temp =(((temperature*vsense)-.76)/.0025)+25;
////		printf("\r\n ADC 1 temperature: %d", (int)temperature);
		uiMemSet(FUNC_READ_LIGH_ADR, adcRead1);
		uiMemSet(FUNC_READ_TEMP_ADR, DHT_DATA.temp_int);
		uiMemSet(FUNC_READ_HUMD_ADR, DHT_DATA.humi_int);




	}

}
void ControlTask(void const *arg)
{
	uint8_t temp;
	while(1)
	{
		temp = uiMemGet(FUNC_WRITE_SPEK_ADR);
		HAL_GPIO_WritePin(GPIO_SPEAKER_GPIO_Port, GPIO_SPEAKER_Pin, temp);
		osDelay(10);
	}
}
