/* Built-in C library includes ---------------*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
/* Platform includes --------------------------*/
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "main.h"
#include "mqttclienttask.h"
#include "flash.h"
#include "rtc.h"
#include "usart.h"
#include "json.h"
#include "modbus_mqtt_bridge.h"
#include "mbproto.h"

/* Network includes----------------------------*/
#include "lwip/apps/mqtt.h"
#include "lwip/apps/mqtt_priv.h"
#include "err.h"
#include "lwip.h"
#include "lwip/init.h"
#include "lwip/netif.h"
/* Shared Variables --------------------------*/
extern osMessageQId xQueueDownlinkHandle;
extern osMessageQId xQueueUplinkHandle;

/* Private Variables -------------------------*/
uint8_t mqtt_couter_err = 0;

/* Start implementation ----------------------*/

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start
			&& strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}
static void mqtt_bridge_pub_request_cb(void *arg, err_t result) {
	if (result != ERR_OK) {
		printf("\r\n Publish result: %d\n", result);
	} else {
//		printf("\r\n Published ");
	}
}

/**@ Brief: Get data from modbus dev and forward it to mqtt server
 * @ Todo:
 * 		Parse json object from server
 *
 *
 *
 *
 *
 * */
void mqtt_modbus_thread_up(mqtt_client_t *client, char *pub_topic) {
	/**/
	BaseType_t Err = pdFALSE;
	xQueueMbMqtt_t xQueueMbMQtt;
	portCHAR publish_buffer[MAX_JSON_LEN];
	err_t err;
	while (1) {
		Err = xQueueReceive(xQueueUplinkHandle, &xQueueMbMQtt,
		portDEFAULT_WAIT_TIME*3);

		//		Err = xQueueReceive(xQueueUplinkHandle, &xQueueMbMQtt,
		//		portDEFAULT_WAIT_TIME);
		if (Err == pdPASS) {
			printf("\r\n Uplink xQueue received");
			/*Create Json and publish to mqtt */
			memset(publish_buffer, 0, MAX_JSON_LEN);
			strcat(publish_buffer, "{\"NodeID\":");
			strcat(publish_buffer, itoa_user(xQueueMbMQtt.NodeID, 10));
			strcat(publish_buffer, ",\"FunC\":");
			strcat(publish_buffer, itoa_user(xQueueMbMQtt.FunC, 10));
			strcat(publish_buffer, ",\"RegAdrH\":");
			strcat(publish_buffer,
					itoa_user(xQueueMbMQtt.RegAdr.i8data[1], 10));
			strcat(publish_buffer, ",\"RegAdrL\":");
			strcat(publish_buffer,
					itoa_user(xQueueMbMQtt.RegAdr.i8data[0], 10));
			strcat(publish_buffer, ",\"RegDataH\":");
			strcat(publish_buffer,
					itoa_user(xQueueMbMQtt.RegData.i8data[1], 10));
			strcat(publish_buffer, ",\"RegDataL\":");
			strcat(publish_buffer,
					itoa_user(xQueueMbMQtt.RegData.i8data[0], 10));
			strcat(publish_buffer, ",\"PortID\":");
			strcat(publish_buffer, itoa_user(xQueueMbMQtt.PortID, 10));
			strcat(publish_buffer, "}");
//			HAL_UART_Transmit(&huart1, publish_buffer, MAX_JSON_LEN, 1000);
			err = mqtt_publish(client, pub_topic, publish_buffer,
					strlen(publish_buffer), QOS_0, 0,
					mqtt_bridge_pub_request_cb,
					NULL);
			if (err != ERR_OK) {
				printf("\r\n Publish err_ntt: %d\n", err);
				if (err == -11)
				{
//					MX_LWIP_Init();
				}
			}

		} else {
			/*Create Json and publish to mqtt */
			memset(publish_buffer, 0, MAX_JSON_LEN);
			strcat(publish_buffer, "{\"NodeID\":");
			strcat(publish_buffer, itoa_user(0, 10));
			strcat(publish_buffer, ",\"FunC\":");
			strcat(publish_buffer, itoa_user(MB_FUNC_ACK, 10));
			strcat(publish_buffer, ",\"RegAdrH\":");
			strcat(publish_buffer,
					itoa_user(xQueueMbMQtt.RegAdr.i8data[1], 10));
			strcat(publish_buffer, ",\"RegAdrL\":");
			strcat(publish_buffer,
					itoa_user(xQueueMbMQtt.RegAdr.i8data[0], 10));
			strcat(publish_buffer, ",\"RegDataH\":");
			strcat(publish_buffer,
					itoa_user(xQueueMbMQtt.RegData.i8data[1], 10));
			strcat(publish_buffer, ",\"RegDataL\":");
			strcat(publish_buffer,
					itoa_user(xQueueMbMQtt.RegData.i8data[0], 10));
			strcat(publish_buffer, ",\"PortID\":");
			strcat(publish_buffer, itoa_user(xQueueMbMQtt.PortID, 10));
			strcat(publish_buffer, "}");
//			HAL_UART_Transmit(&huart1, publish_buffer, MAX_JSON_LEN, 1000);
			err = mqtt_publish(client, pub_topic, publish_buffer,
					strlen(publish_buffer), QOS_0, 0,
					mqtt_bridge_pub_request_cb,
					NULL);
			if (err != ERR_OK) {
				mqtt_couter_err++;

				printf("\r\n Publish err: %d\n", err);
				if (mqtt_couter_err == 10) NVIC_SystemReset();
			}
			else
			{
				mqtt_couter_err = 0;
			}

		}
//		vTaskDelay(10);
	}
}
//	how to got mqtt intance??

/**@ Brief: Parse and transfer from mqtt server to modbus device
 * @ Todo:
 * 		Parse json object from server
 *
 *
 *
 *
 *
 * */
uint8_t mqtt_modbus_thread_down(char *pJsonMQTTBuffer,
		uint16_t pJsonMQTTBufferLen) {
	/*Parsing json by using clone source :) */
	int i;
	int r;
	jsmn_parser p;
	jsmntok_t t[JSON_MAX_LEN]; /* We expect no more than JSON_MAX_LEN tokens */
	jsmn_init(&p);
	xQueueMbMqtt_t xQueueMbMqtt;

	r = jsmn_parse(&p, pJsonMQTTBuffer, pJsonMQTTBufferLen, t,
			sizeof(t) / sizeof(t[0]));
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}
	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
		if (jsoneq(pJsonMQTTBuffer, &t[i], "NodeID") == 0) {
			/* We may use strndup() to fetch string value */
			printf("\r\n - NodeID: %.*s\n", t[i + 1].end - t[i + 1].start,
					pJsonMQTTBuffer + t[i + 1].start);
			xQueueMbMqtt.NodeID = atoi(pJsonMQTTBuffer + t[i + 1].start);

			printf("\r\n nodeid: %d", atoi(pJsonMQTTBuffer + t[i + 1].start));
			i++;
		} else if (jsoneq(pJsonMQTTBuffer, &t[i], "FunC") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
			printf("\r\n - FunC: %.*s\n", t[i + 1].end - t[i + 1].start,
					pJsonMQTTBuffer + t[i + 1].start);
			xQueueMbMqtt.FunC = atoi(pJsonMQTTBuffer + t[i + 1].start);

			i++;
		} else if (jsoneq(pJsonMQTTBuffer, &t[i], "RegAdrL") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
			printf("\r\n - RegAdr: %.*s\n", t[i + 1].end - t[i + 1].start,
					pJsonMQTTBuffer + t[i + 1].start);
			xQueueMbMqtt.RegAdr.i8data[0] = atoi(
					pJsonMQTTBuffer + t[i + 1].start);
			i++;
		} else if (jsoneq(pJsonMQTTBuffer, &t[i], "RegAdrH") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
			printf("\r\n - RegData: %.*s\n", t[i + 1].end - t[i + 1].start,
					pJsonMQTTBuffer + t[i + 1].start);
			xQueueMbMqtt.RegAdr.i8data[1] = atoi(
					pJsonMQTTBuffer + t[i + 1].start);

			i++;
		} else if (jsoneq(pJsonMQTTBuffer, &t[i], "RegDataL") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
			printf("- RegAdr: %.*s\n", t[i + 1].end - t[i + 1].start,
					pJsonMQTTBuffer + t[i + 1].start);
			xQueueMbMqtt.RegData.i8data[0] = atoi(
					pJsonMQTTBuffer + t[i + 1].start);

			i++;
		} else if (jsoneq(pJsonMQTTBuffer, &t[i], "RegDataH") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
			printf("- RegData: %.*s\n", t[i + 1].end - t[i + 1].start,
					pJsonMQTTBuffer + t[i + 1].start);
			xQueueMbMqtt.RegData.i8data[1] = atoi(
					pJsonMQTTBuffer + t[i + 1].start);

			i++;
		} else if (jsoneq(pJsonMQTTBuffer, &t[i], "PortID") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
			printf("- PortID: %.*s\n", t[i + 1].end - t[i + 1].start,
					pJsonMQTTBuffer + t[i + 1].start);
			xQueueMbMqtt.PortID = atoi(pJsonMQTTBuffer + t[i + 1].start);

			i++;
		}

	}

	BaseType_t Err = pdFALSE;
//	Err = xQueueSend(xQueueUplinkHandle, &xQueueMbMqtt,
//				portDEFAULT_WAIT_TIME);
	Err = xQueueSend(xQueueDownlinkHandle, &xQueueMbMqtt,
			portDEFAULT_WAIT_TIME);
//	Err = mqtt_publish(client, pub_topic, publish_buffer,
//						strlen(publish_buffer), QOS_0, 0,
//						mqtt_bridge_pub_request_cb,
//						NULL);
	if (Err == pdPASS) {
		printf("\r\n Modbus_MQTT Downlink queued: OK \r\n");

	} else {
		printf("\r\n Modbus_MQTT Downlink queued: False \r\n");
	}

}

