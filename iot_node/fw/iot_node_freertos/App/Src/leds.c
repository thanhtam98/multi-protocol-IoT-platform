#include "leds.h"
#include "main.h"
#include "cmsis_os.h"
#include "app.h"

void v_led_controller(uint8_t led_num, uint8_t led_state) {
	/*--Add mapping array if the led total is more than 4*/
	if (led_state == LED_BLYNK) {
		switch (led_num) {
		case 0:
			HAL_GPIO_TogglePin(LED_0_GPIO_Port, LED_0_Pin);
			break;
		case 1:
			HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
			break;
		case 2:
			HAL_GPIO_TogglePin(LED_2_GPIO_Port, LED_2_Pin);
			break;
		case 3:
			HAL_GPIO_TogglePin(LED_3_GPIO_Port, LED_3_Pin);
			break;
		default:
			HAL_GPIO_TogglePin(LED_0_GPIO_Port,
			LED_3_Pin | LED_2_Pin | LED_1_Pin | LED_0_Pin);

		}
	}

	else {
		switch (led_num) {
		case 0:
			HAL_GPIO_WritePin(LED_0_GPIO_Port, LED_0_Pin,
					(FlagStatus) led_state);
			break;
		case 1:
			HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin,
					(FlagStatus) led_state);
			break;
		case 2:
			HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin,
					(FlagStatus) led_state);
			break;
		case 3:
			HAL_GPIO_WritePin(LED_3_GPIO_Port, LED_3_Pin,
					(FlagStatus) led_state);
			break;
		default:
			HAL_GPIO_WritePin(LED_0_GPIO_Port,
			LED_3_Pin | LED_2_Pin | LED_1_Pin | LED_0_Pin,
					(FlagStatus) led_state);

		}
	}

}

/*
 * Brief: Callback function for led execution
 *
 * **/
void v_led_process_callback(const void *arg) {
	/* **/
	led_control_update_t *px_led_control_update = (led_control_update_t*) arg;
	/* Private led couting values ???*/
	static uint8_t led_blynk_update_interval[LED_MAX_NUM];
	static uint8_t led_blynk_update_couting[LED_MAX_NUM];
	static uint8_t led_blynk_timeout[LED_MAX_NUM];

	for (uint8_t led_index; led_index < LED_MAX_NUM; led_index++) {
		if (px_led_control_update[led_index].is_update == UPDATE) {
			/* Update the new param*/
			led_blynk_update_interval[led_index] =
					1000
							/ ((LED_UPDATE_INTERVAL_MS)
									* px_led_control_update[led_index].led_control.led_freq);
			led_blynk_timeout[led_index] =
					1000
							* (px_led_control_update[led_index].led_control.led_timeout)/LED_UPDATE_INTERVAL_MS;
			px_led_control_update[led_index].is_update = NONE;
		} else {
			/* Continue to execute **/
			switch (px_led_control_update[led_index].led_control.led_state) {
			case LED_ON:
				v_led_controller(led_index, LED_ON);
				break;
			case LED_OFF:
				v_led_controller(led_index, LED_OFF);
				break;
			case LED_BLYNK:
				if (led_blynk_update_couting[led_index] == 0) {
					led_blynk_update_couting[led_index] =
							led_blynk_update_interval[led_index];
					led_blynk_timeout[led_index]--;
					if (led_blynk_timeout[led_index] == 0) {
						px_led_control_update[led_index].led_control.led_state =
								LED_OFF;
					}
					v_led_controller(led_index, LED_BLYNK);
				} else {
					led_blynk_update_couting[led_index]--;

				}
				break;
			}

		}
	}

}

vLedTask(const void *arg) {
	/* Global struct mapping */
	iot_node_t *px_iot_node_handle = (iot_node_t*) arg;

	/*
	 * Create led instance and set default values
	 * */
	led_control_update_t led_control_update[LED_MAX_NUM];
	for (uint8_t led_index = 0; led_index < LED_MAX_NUM; led_index++) {
		led_control_update[led_index].is_update = NONE;
		led_control_update[led_index].led_control.led_num = led_index;
		led_control_update[led_index].led_control.led_freq = 0;
		led_control_update[led_index].led_control.led_state = LED_OFF;
		led_control_update[led_index].led_control.led_timeout = 0;
	}
	/*HW init*/

	/*SW timmer init*/
	osTimerDef(ledTimer, v_led_process_callback);
	osTimerId ledTimerHandle = osTimerCreate(osTimer(ledTimer), osTimerPeriodic,
			(void*) led_control_update);
	osTimerStart(ledTimerHandle, LED_UPDATE_INTERVAL_MS);

	led_control_update_t *led_control_queue;
	while (1) {

		/* Take the queue */
		osEvent event = osMessageGet(px_iot_node_handle->ledQueueHandle,
		osWaitForever);
		if (event.status == osOK) {
			led_control_queue = event.value.p;
			led_control_update[led_control_queue->led_control.led_num].is_update =
					UPDATE;
			led_control_update[led_control_queue->led_control.led_num].led_control.led_num =
					led_control_queue->led_control.led_num;
			led_control_update[led_control_queue->led_control.led_num].led_control.led_freq =
					led_control_queue->led_control.led_freq;
			led_control_update[led_control_queue->led_control.led_num].led_control.led_state =
					led_control_queue->led_control.led_state;
			led_control_update[led_control_queue->led_control.led_num].led_control.led_timeout =
					led_control_queue->led_control.led_timeout;

//			memcpy(led_control_update[led_control_queue->led_control.led_num],)
		}
		/* Update to callback*/
	}
}
