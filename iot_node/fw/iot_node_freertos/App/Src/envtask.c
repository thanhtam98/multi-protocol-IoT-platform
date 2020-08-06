/* Built-in C library includes ----------------------*/

#include <stdio.h>
#include <stdint.h>
#include "cmsis_os.h"
/* Platform includes --------------------------------*/
#include "main.h"
#include "sharedmem.h"
#include "envtask.h"
#include "dht.h"
#include  "adc.h"
#include "gpio.h"
#include "param.h"
#include "ds18b20.h"
#include "tim.h"
#include "timers.h"
/*Timer */
#define ENV_TIME_INTERVAL  10
osTimerId envTimerHandler;

/* Shared variables ---------------------------------------------------------*/
//extern ADC_HandleTypeDef hadc1;
io_port_t ioPort[IO_MAX_PORT];
uint8_t ubSequenceCompleted = RESET;
__IO uint16_t aADCxConvertedValues[2];
/* Start Implementation -----------------------------*/
/**/

void vPortRawRead(uint32_t *portData) {

	/* Port1 ------------*/
	if (ioPort[PORT1].timeCouter == 0)
		portData[PORT1] = HAL_GPIO_ReadPin(SW_1_GPIO_Port, SW_1_Pin);
	/* Port2 ------------*/
	if (ioPort[PORT2].timeCouter == 0)
		portData[PORT3] = HAL_GPIO_ReadPin(SW_2_GPIO_Port, SW_2_Pin);
	/* Port3 ------------*/

	/* Port 4 5 ------------*/
	uint32_t adcRaw[2];
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100);
	adcRaw[0] = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100);
	adcRaw[1] = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	if (ioPort[PORT4].timeCouter == 0)
		portData[PORT4] = adcRaw[0];
	if (ioPort[PORT5].timeCouter == 0)
		portData[PORT5] = adcRaw[1];

	/* Port6 7----------------**/
	if (ioPort[PORT6].timeCouter == 0)
	{
		DHT11_DATA_TypeDef DHT_DATA;
		DHT11_ReadData(&DHT_DATA);
		if ((DHT_DATA.check_sum != 00)&&(DHT_DATA.temp_int !=0))
		{
			/* Ensure DHT driver return the correct values*/
			portData[PORT6] = (uint32_t) (DHT_DATA.temp_int << 8 * 3)
					| (DHT_DATA.temp_deci << 8 * 2)
					| (DHT_DATA.humi_int << 8 * 1)
					| (DHT_DATA.humi_deci << 8 * 0);
		}

	}

}
void vPortProcess(uint32_t *portData) {
	uiMemSet(PORT_IO_SW_1, (uint16_t) portData[PORT1]);
	uiMemSet(PORT_IO_SW_2, (uint16_t) portData[PORT2]);
	uiMemSet(PORT_PULSE, (uint16_t) portData[PORT3]);
	float f_data_temp = portData[PORT4] * 100 / 4096;
	uint16_t u_int_part = (uint16_t) portData[PORT4] * 100 / 4096;
	uint16_t u_dec_part = (uint16_t) 10
			* (float) (u_int_part - (uint16_t) portData[PORT4] * 100 / 4096);
	uiMemSet(PORT_ADC_1, (uint16_t) (u_int_part << 8) | (u_dec_part));
	f_data_temp = portData[PORT5] * 100 / 4096;
	u_int_part = (uint16_t) portData[PORT5] * 100 / 4096;
	u_dec_part = (uint16_t) 10
			* (float) (u_int_part - (uint16_t) portData[PORT5] * 100 / 4096);
	uiMemSet(PORT_ADC_2, (uint16_t) (u_int_part << 8) | (u_dec_part));

	uiMemSet(PORT_ONE_WIRE, (uint16_t) portData[PORT6]);
	uiMemSet(PORT_ONE_WIRE + 1, (uint16_t) (portData[PORT6] >> 16));

	 // profile processing -- Todo.
	/*
	for (uint8_t portIndex = 0; portIndex < IO_MAX_PORT; portIndex++) {
		switch (ioPort[portIndex].profile) {
		case IO_SW_NC:
			uiMemSet(portIndex + 1,
					(portData[portIndex] == RESET) ? SET : RESET);
			break;
		case IO_SW_NO:
			uiMemSet(portIndex + 1,
					(portData[portIndex] == RESET) ? RESET : SET);
			break;
		case IO_PUL_PER_SEC:
			uiMemSet(portIndex + 1,
					(uint16_t) ((portData[portIndex] / ENV_TIME_INTERVAL) << 8)
							| 0x0000);
			break;
		case IO_PUL_PER_MIN:
			uiMemSet(portIndex + 1,
					(uint16_t) ((60 * portData[portIndex] / ENV_TIME_INTERVAL)
							<< 8) | 0x0000);
			break;
		case IO_PUL_PER_HOUR:
			uiMemSet(portIndex + 1,
					(uint16_t) ((60 * 60 * portData[portIndex]
							/ ENV_TIME_INTERVAL) << 8) | 0x0000);
			break;
		case IO_ADC_LIGHT:
			uiMemSet(portIndex + 1, (uint16_t) portData[portIndex]);

			break;
		case IO_ADC_TEMP:
			uiMemSet(portIndex + 1, (uint16_t) portData[portIndex]);
			break;
		case IO_ADC_HUMID:
			uiMemSet(portIndex + 1, (uint16_t) portData[portIndex]);
			break;
		case IO_ONEWIRE_DHT11:
			uiMemSet(portIndex + 1, (uint16_t) portData[portIndex]);

			break;
		case IO_ONEWIRE_DHT22:
			uiMemSet(portIndex + 1, (uint16_t) portData[portIndex]);
			break;
		case IO_ONWWIRE_DS18B20:
			uiMemSet(portIndex + 1, (uint16_t) portData[portIndex]);

			break;
		}

	}
	*/
}
void vPortTimeCounter(uint32_t *portData) {
	for (uint8_t portIdex = 0; portIdex < IO_MAX_PORT; portIdex++) {
		ioPort[portIdex].timeCouter++;
		if (ioPort[portIdex].timeCouter == ioPort[portIdex].timeInterval) {
			ioPort[portIdex].timeCouter = 0;
		}

		if (ioPort[portIdex].timeCouter == 255)
			ioPort[portIdex].timeCouter = 1;

	}
}
void vEnvTimerCallback(void const *arg) {
	uint32_t *pxArg = (uint32_t) arg;

	vPortRawRead(pxArg);
	vPortProcess(pxArg);

}
void vEnvTask(void const *arg) {
	/*
	 * Initial ports if configured
	 * **/

	if (PARAM[NODE_HAVE_PARAM_ADR] == EEP_PARAM) {
		for (uint8_t portIndex = 0; portIndex < IO_MAX_PORT; portIndex++) {
			ioPort[portIndex].profile = u_mem_get(NODE_IO_BASE + portIndex * 2);
			ioPort[portIndex].timeInterval = u_mem_get(
			NODE_IO_BASE + portIndex * 2 + 1);
		}
	} else {

		/*Us*/
	}

	uint32_t temperature;
	uint32_t u32portRawData[IO_MAX_PORT];
	HAL_ADC_Start(&hadc1);

//	envTimerHandler = osTimerCreate(vEnvTimerCallback, osTimerPeriodic,
//			u32portRawData);
//	osTimerStart(envTimerHandler, ENV_TIME_INTERVAL * 1000);
	while (1) {
		vEnvTimerCallback(u32portRawData);
		osDelay(1000);
		/* */

	}

}
void ControlTask(void const *arg) {
	uint8_t temp;
	while (1) {
		temp = uiMemGet(FUNC_WRITE_SPEK_ADR);
		//	HAL_GPIO_WritePin(GPIO_SPEAKER_GPIO_Port, GPIO_SPEAKER_Pin, temp);
		osDelay(10);
	}
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle) {
	/* Report to main program that ADC sequencer has reached its end */
	ubSequenceCompleted = SET;
}
