#ifndef __MODBUS_MQTT_BRIDGE__H__
#define __MODBUS_MQTT_BRIDGE__H__
//#include "MQTTClient.h"
#include "lwip/apps/mqtt.h"
#include "lwip/apps/mqtt_priv.h"
#include "lwip/err.h"
/* Public define*/
#define JSON_MAX_LEN 128
#define portDEFAULT_WAIT_TIME 1000
#define MAX_JSON_LEN 			100
//#define ""


/* Public function prototype ----------------*/
uint8_t mqtt_modbus_thread_down(char *pJsonMQTTBuffer, uint16_t ui16BufferLen);
void mqtt_modbus_thread_up(mqtt_client_t *arg, char *pub_topic);
#endif
