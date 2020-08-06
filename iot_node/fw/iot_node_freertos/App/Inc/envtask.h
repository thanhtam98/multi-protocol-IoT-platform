#ifndef _ENVTASK__H_
#define _ENVTASK__H_

#include "sharedmem.h"

/*
 *
 * */
#define IO_MAX_PORT		7
/* IO
 *   [SW1][SW2][PULS][ADC1][ADC2][OneWire]
 *
 * */

/* Profile Identification**/
enum {
	IO_SW_NC,
	IO_SW_NO,
	IO_PUL_PER_SEC,
	IO_PUL_PER_MIN,
	IO_PUL_PER_HOUR,
	IO_ADC_LIGHT,
	IO_ADC_TEMP,
	IO_ADC_HUMID,
	IO_ONEWIRE_DHT11,
	IO_ONEWIRE_DHT22,
	IO_ONWWIRE_DS18B20
};
enum {
	PORT1,
	PORT2,
	PORT3,
	PORT4,
	PORT5,
	PORT6,
	PORT7
};
typedef struct {
	uint8_t profile;
	uint8_t timeInterval; // second
	uint8_t timeCouter;
} io_port_t;
/*Public Task prototype ------------------- */
void vEnvTask(void const *arg);
void ControlTask(void const *arg);


#endif
