#ifndef __APP_H__
#define __APP_H__

#include "cmsis_os.h"
#include "main.h"
#include "gpio.h"
#include "app.h"
#include "dht.h"
#include "rak.h"
#include "mbtask.h"
#include "envtask.h"
#include "sharedmem.h"
#include "param.h"
#include "i2c.h"
#include "command.h"
#include "btn.h"
#include "leds.h"


typedef struct {
	osMessageQId btnQueueHandle;
	osMessageQId ledQueueHandle;
	osMessageQId envQueueHandle;
}iot_node_t;



void vAppDefault(void);
void vAppConfiguration(void);








#endif
