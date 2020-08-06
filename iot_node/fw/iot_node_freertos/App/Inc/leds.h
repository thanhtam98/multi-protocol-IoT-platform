#ifndef __LEDS_D__
#define __LEDS_D__

#include "main.h"
#include "cmsis_os.h"
enum {
	LED_ON, LED_OFF, LED_BLYNK
};
enum {
	NONE, UPDATE = !NONE
};
typedef struct {
	uint8_t led_num;
	uint8_t led_state;
	uint8_t led_freq;
	uint8_t led_timeout;
} led_control_t;

typedef struct  {
	led_control_t led_control;
	uint8_t is_update;
}led_control_update_t;

#define LED_MAX_NUM		4
#define LED_UPDATE_INTERVAL_MS 	10

#endif
