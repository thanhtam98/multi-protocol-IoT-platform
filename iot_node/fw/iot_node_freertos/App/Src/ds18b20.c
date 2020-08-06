#include  "ds18b20.h"
#include "main.h"
#include "cmsis_os.h"
#include "sharedmem.h"
#define DS18B20_PORT GPIOB
#define DS18B20_PIN GPIO_PIN_1

/* Start implementation -------------------*/

void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

uint8_t DS18B20_Start(void) {
	uint8_t Response = 0;
	Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);   // set the pin as output
	HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET); // pull the pin low
	delay_us(480);   // delay_us according to datasheet

	Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);    // set the pin as input
	delay_us(80);    // delay_us according to datasheet

	if (!(HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN)))
		Response = 1;    // if the pin is low i.e the presence pulse is detected
	else
		Response = -1;

	delay_us(400); // 480 us delay_us totally.

	return Response;
}

void DS18B20_Write(uint8_t data) {
	Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);  // set as output

	for (int i = 0; i < 8; i++) {

		if ((data & (1 << i)) != 0)  // if the bit is high
				{
			// write 1

			Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);  // set as output
			HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET); // pull the pin LOW
			delay_us(1);  // wait for 1 us

			Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);  // set as input
			delay_us(50);  // wait for 60 us
		}

		else  // if the bit is low
		{
			// write 0

			Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);
			HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET); // pull the pin LOW
			delay_us(50);  // wait for 60 us

			Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);
		}
	}
}

uint8_t DS18B20_Read(void) {
	uint8_t value = 0;

	Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);

	for (int i = 0; i < 8; i++) {
		Set_Pin_Output(DS18B20_PORT, DS18B20_PIN);   // set as output

		HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET); // pull the data pin LOW
		delay_us(1);  // wait for > 1us

		Set_Pin_Input(DS18B20_PORT, DS18B20_PIN);  // set as input
		if (HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN))  // if the pin is HIGH
				{
			value |= 1 << i;  // read = 1
		}
		delay_us(50);  // wait for 60 us
	}
	return value;
}

float f_ds18b20(void) {
	uint8_t Temp_byte1, Temp_byte2;
	uint16_t Temp;
	float Temp_out;
	taskENTER_CRITICAL();
	DS18B20_Start();
	HAL_Delay(1);
	DS18B20_Write(0xCC);  // skip ROM
	DS18B20_Write(0x44);  // convert t
	taskEXIT_CRITICAL();
//	HAL_Delay(800);
	osDelay(800);
	taskENTER_CRITICAL();
	DS18B20_Start();
	HAL_Delay(1);
	DS18B20_Write(0xCC);  // skip ROM
	DS18B20_Write(0xBE);  // Read Scratch-pad
	taskEXIT_CRITICAL();
	Temp_byte1 = DS18B20_Read();
	Temp_byte2 = DS18B20_Read();
	Temp = (Temp_byte2 << 8) | Temp_byte1;
	Temp_out = (float) Temp / 16;
	printf("Temp Value: %d\r\n",(uint16_t)Temp_out);
	uiMemSet(PORT_ONE_WIRE,Temp);
	return  Temp_out;

}

