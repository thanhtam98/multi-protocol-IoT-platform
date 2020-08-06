#include "btn.h"
#include "main.h"
#include  "app.h"
//void v_button_process_callback(uint8_t button_num, uint8_t button_state) {
//
//}

sw_t sw_state(uint8_t swNumber) {

	switch (swNumber) {
	case 0:
		if (HAL_GPIO_ReadPin(BTN_1_GPIO_Port, BTN_1_Pin) == 0)
			return PRESSED;
		else
			return RELEASED;
	case 1:
		if (HAL_GPIO_ReadPin(BTN_2_GPIO_Port, BTN_2_Pin) == 0)
			return PRESSED;
		else
			return RELEASED;
	case 2:
		if (HAL_GPIO_ReadPin(BTN_3_GPIO_Port, BTN_3_Pin) == 0)
			return PRESSED;
		else
			return RELEASED;
	}

}
void vBtnTask(const void *arg) {
	/* Global struct mapping */
	iot_node_t *px_iot_node_handle = (iot_node_t*) arg;
	/*HW init*/

	sw_data_t swData[3];
	swData[0].swTaskSource = xBTN1;
	swData[1].swTaskSource = xBTN2;
	swData[2].swTaskSource = xBTN3;
	/*HW mapping **/

	sw_object_t swObject[3] = { { .swState = bWAITING, .detectTimeInterval = 0,
			.resetTimeInterval = 0 }, { .swState = bWAITING,
			.detectTimeInterval = 0, .resetTimeInterval = 0 }, { .swState =
			bWAITING, .detectTimeInterval = 0, .resetTimeInterval = 0 }, };
//	u_button_callback_function_t u_button_callback_function;
	while (1) {

		osDelay(BTN_MIN_WAITING_MS);
		for (uint8_t btn_num = 0; btn_num < BTN_MAX_NUM; btn_num++)
		{

			switch (swObject[btn_num].swState) {
			case bWAITING: {
				if (sw_state(btn_num) == PRESSED) {
					swObject[btn_num].detectTimeInterval++;
					if (swObject[btn_num].detectTimeInterval
							>= BTN_MIN_WAITING_MS) {
						swObject[btn_num].swState = bPREAMBLE;
						swObject[btn_num].detectTimeInterval = 0;
					}
				}
				break;
			}
			case bPREAMBLE:
				/*
				 this case to decide what button do
				 */
			{
				if (sw_state(btn_num) == PRESSED) {
					swObject[btn_num].detectTimeInterval++;
					if (swObject[btn_num].detectTimeInterval
							>= 10 * BTN_MAX_WAITING_MS) {
//						ledData.profileNum = BLK;
//						ledData.ledNum = 0;
//						xStatus = xQueueSend(pxValveHandles->xQueueLedIndicate,
//								&ledData, xTicksToWait);
						/* ------------------------*/
						swObject[btn_num].swState = bLONG_CMP;
						swObject[btn_num].detectTimeInterval = 0;
						swObject[btn_num].resetTimeInterval = 0;
					}
				} else if (sw_state(btn_num) == RELEASED) {
					swObject[btn_num].resetTimeInterval++;
					if (swObject[btn_num].resetTimeInterval
							>= BTN_MAX_WAITING_MS) {
						swObject[btn_num].swState = bSHORT;
						swObject[btn_num].resetTimeInterval = 0;
//						ledData.profileNum = BLK;
//						ledData.ledNum = 0;
//
//						xStatus = xQueueSend(pxValveHandles->xQueueLedIndicate,
//								&ledData, xTicksToWait);
					}
				}

				break;
			}
			case bSHORT: {

				swData[btn_num].swData = SHORT_PRESS;
//				osMessagePut(px_iot_node_handle->btnQueueHandle, &swData[btn_num],100);
//				xStatus = xQueueSend(pxValveHandles->xQueue, &swData[btn_num],
//						xTicksToWait);
				swObject[btn_num].swState = bEND;
				DBG("Send Short");
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);

				break;
			}
			case bLONG_CMP: {

				if (sw_state(btn_num) == PRESSED) {
					swObject[btn_num].detectTimeInterval++;
					if (swObject[btn_num].detectTimeInterval
							>= 15 * BTN_MAX_WAITING_MS) {
						swObject[btn_num].swState = bDBLONG;
						swObject[btn_num].detectTimeInterval = 0;
//						ledData.profileNum = LONG_BLK;
//						ledData.ledNum = 0;
//						xStatus = xQueueSend(pxValveHandles->xQueueLedIndicate,
//								&ledData, xTicksToWait);
					}
				} else if (sw_state(btn_num) == RELEASED) {
					swObject[btn_num].resetTimeInterval++;
					if (swObject[btn_num].resetTimeInterval
							>= BTN_MAX_WAITING_MS) {
						swObject[btn_num].swState = bLONG;
						swObject[btn_num].resetTimeInterval = 0;
					}
				}
				break;
			}
			case bDBLONG: {
				swData[btn_num].swData = DBLONG_PRESS;
//				osMessagePut(px_iot_node_handle->btnQueueHandle, &swData[btn_num],100);
//				xStatus = xQueueSend(pxValveHandles->xQueue, &swData[btn_num],
//						xTicksToWait);
				swObject[btn_num].swState = bEND;
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);

				DBG("Send DBLong");
				break;
			}
			case bLONG: {

				swData[btn_num].swData = LONG_PRESS;
//				osMessagePut(px_iot_node_handle->btnQueueHandle, &swData[btn_num],100);
//				xStatus = xQueueSend(pxValveHandles->xQueue, &swData[btn_num],
//						xTicksToWait);
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);

				swObject[btn_num].swState = bEND;
				DBG("Send Long");

				break;
			}
			case bDOUBLE: {
				break;
			}
			case bEND: {
				if (sw_state(btn_num) == RELEASED) {
					swObject[btn_num].swState = bWAITING;
				}
				break;
			}
			default:
				break;
			}
		}
	}
}

