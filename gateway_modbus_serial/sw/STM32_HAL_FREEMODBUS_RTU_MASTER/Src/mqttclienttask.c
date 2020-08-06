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
#include "modbus_mqtt_bridge.h"
/* Network includes. */
//#include "MQTTClient.h"
#include "lwip/apps/mqtt.h"
#include "lwip/apps/mqtt_priv.h"
#include "lwip/err.h"
/* Shared Variables ----------------------------------*/
osThreadId netMQTTTask;
extern osMessageQId xQueueControlHandle;
extern osMessageQId xQueueMessageHandle;
extern osSemaphoreId netMqttIpSemaphoreHandle;
extern osMessageQId xQueueDownlinkHandle;
extern osMessageQId xQueueUplinkHandle;
extern network_param_t mqttHostParam;
extern network_param_t netParam;
extern uint32_t netId;
const uint16_t u16_mqtt_port = 9031;
void netmqttTask(void *arg);

/* Private Varianles*/
static int inpub_id;
/* Start implementation ------------------------------*/

//void onMessageArrived(MessageData *data)
//{
//printf("Message Arrived \r\n");
//	printf("\r\nMessage arrived on topic %.*s: %.*s", data->topicName->lenstring.len, data->topicName->lenstring.data,
//		data->message->payloadlen, data->message->payload);
//}
//void onMessageArrived_slave(MessageData *data)
//{
//printf("Message Arrived \r\n");
//	printf("\r\nMessage arrived on topic %.*s: %.*s", data->topicName->lenstring.len, data->topicName->lenstring.data,
//		data->message->payloadlen, data->message->payload);
//}
//void onMessageArrived_master(MessageData *data)
//{
//printf("Message Arrived \r\n");
//	printf("\r\nMessage arrived on topic %.*s: %.*s", data->topicName->lenstring.len, data->topicName->lenstring.data,
//		data->message->payloadlen, data->message->payload);
//}
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len,
		u8_t flags) {
	printf("\r\n Incoming publish payload with length %d, flags %u\n", len,
			(unsigned int) flags);

	if (flags & MQTT_DATA_FLAG_LAST) {
		/* Last fragment of payload received (or whole part if payload fits receive buffer
		 See MQTT_VAR_HEADER_BUFFER_LEN)  */
		printf("\r\n mqtt_incoming_data_cb: %s\n", (const char *) data);
		mqtt_modbus_thread_down((char*) data, len);

		/* Call function or do action depending on reference, in this case inpub_id */
//		if (inpub_id == 0) {
//			/* Don't trust the publisher, check zero termination */
//			if (data[len - 1] == 0) {
//				printf("mqtt_incoming_data_cb: %s\n", (const char *) data);
//			}
//		} else if (inpub_id == 1) {
//			/* Call an 'A' function... */
//		} else {
//			printf("mqtt_incoming_data_cb: Ignoring payload...\n");
//		}
	} else {
		/* Handle fragmented payload, store in buffer, write to file or whatever */
	}
}

static void mqtt_sub_request_cb(void *arg, err_t result) {
	/* Just print the result code here for simplicity,
	 normal behaviour would be to take some action if subscribe fails like
	 notifying user, retry subscribe or disconnect from server */
	printf("Subscribe result: %d\n", result);
}

static void mqtt_incoming_publish_cb(void *arg, const char *topic,
		u32_t tot_len) {
	printf("\r\n Incoming publish at topic %s with total length %u\n", topic,
			(unsigned int) tot_len);

	/* Decode topic string into a user defined reference */
	if (strcmp(topic, "print_payload") == 0) {
		inpub_id = 0;
	} else if (topic[0] == 'A') {
		/* All topics starting with 'A' might be handled at the same way */
		inpub_id = 1;
	} else {
		/* For all other topics */
		inpub_id = 2;
	}
}
void mqtt_connection_cb(mqtt_client_t *client, void *arg,
		mqtt_connection_status_t status) {
	printf("\r\n MQTT Status: %d \r\n", status);
	err_t err;
	char sub_topic[20];
	memset(sub_topic, 0, sizeof(sub_topic));
	strcat(sub_topic, "modbus/");
	strcat(sub_topic, itoa_user(netId, 10));
	strcat(sub_topic, "/downlink");
	HAL_UART_Transmit(&huart1, sub_topic, 10, 1000);
	if (status == MQTT_CONNECT_ACCEPTED) {
		printf("\r\n mqtt_connection_cb: Successfully connected\n");

		/* Setup callback for incoming publish requests */
		mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb,
				mqtt_incoming_data_cb, arg);

		/* Subscribe to a topic named "subtopic" with QoS level 1, call mqtt_sub_request_cb with result */
		err = mqtt_subscribe(client, sub_topic, 1, mqtt_sub_request_cb, arg);
//		printf("mqtt_subscribe to: %s ", sub_topic);

		if (err != ERR_OK) {
			printf("\r\nmqtt_subscribe return: %d\n", err);
		}

	} else {
		printf("\r\nmqtt_connection_cb: Disconnected, reason: %d\n", status);
		ip4_addr_t broker_ipaddr;
		broker_ipaddr.addr = mqttHostParam.ip.idata;
		struct mqtt_connect_client_info_t mqtt_client_info;
		mqtt_client_info.client_id = "BAC_02";
		mqtt_client_info.client_user = "admin";
		mqtt_client_info.client_pass = NULL;
		mqtt_client_info.keep_alive = 1000;
		mqtt_client_info.will_topic = NULL;
		mqtt_client_info.will_qos = 1;
		mqtt_client_info.will_retain = 0;

		err_t er;
		er = mqtt_client_connect(client, &broker_ipaddr, 9031,
				mqtt_connection_cb, NULL, &mqtt_client_info);
		vTaskDelay(100);

		printf("\r\b MQTT Service started successfully \r\n");
		if (er == ERR_OK) {
			printf("\r\n Connect to the Server %d.%d.%d.%d",
					mqttHostParam.ip.cdata[0], mqttHostParam.ip.cdata[1],
					mqttHostParam.ip.cdata[2], mqttHostParam.ip.cdata[3]);
		} else {
			printf("\r\n Failed to connect to MQTT server with er: %d", er);
		}

		/* Its more nice to be connected, so try to reconnect */
//		example_do_connect(client);
	}
}
static void mqtt_pub_request_cb(void *arg, err_t result) {
	if (result != ERR_OK) {
		printf("\r\n Publish result: %d\n", result);
	} else {
//		printf("\r\n Published ");
	}
}
void example_publish(mqtt_client_t *client, void *arg) {
	char buf[50];
	char membuf[20];
	RTC_TimeTypeDef sTime = { 0 };
	RTC_DateTypeDef sDate = { 0 };
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	sprintf(buf, "\r\n Time: %d %d %d", sTime.Hours, sTime.Minutes,
			sTime.Seconds);
	sprintf(membuf, "\r\n MemFree: %d", xPortGetFreeHeapSize());

	const char *pub_payload =
			"This is a collection of lectures and labs Linux kernel topics. The lectures focus on theoretical and Linux kernel exploration.";
	err_t err;
	u8_t qos = 2; /* 0 1 or 2, see MQTT specification */
	u8_t retain = 0; /* No don't retain such crappy payload... */
	err = mqtt_publish(client, "pub_topic", buf, strlen(buf), qos, retain,
			mqtt_pub_request_cb, arg);
	err = mqtt_publish(client, "pub_topic", membuf, strlen(membuf), qos, retain,
			mqtt_pub_request_cb, arg);
	err = mqtt_publish(client, "pub_topic", pub_payload, strlen(pub_payload),
			qos, retain, mqtt_pub_request_cb, arg);

	if (err != ERR_OK) {
		printf("\r\n Publish err: %d\n", err);
	}
}
void netmqttTask(void *arg) {
	printf("\r\n netmqttTask \r\n");
	xQueueControl_t xQueueControl;
	uint8_t uiSysState;
	xQueueControl.xTask = netMQTTTask;
	/*Waiting for controller start this task*/
	do {
		osDelay(10);
		xQueuePeek(xQueueMessageHandle, &uiSysState, 0);
	} while (uiSysState != SYS_MQTT);
	xQueueReceive(xQueueMessageHandle, &uiSysState, 0);
	printf("\r\b MQTT Service starting \r\n");
	/*Starting initial*/
	err_t er;
	mqtt_client_t *mqtt_client;
	ip4_addr_t broker_ipaddr;
	broker_ipaddr.addr = mqttHostParam.ip.idata;
	struct mqtt_connect_client_info_t mqtt_client_info;
	mqtt_client_info.client_id = "BAC_01";
	mqtt_client_info.client_user = "admin";
	mqtt_client_info.client_pass = NULL;
	mqtt_client_info.keep_alive = 1000;
	mqtt_client_info.will_topic = NULL;
	mqtt_client_info.will_qos = 1;
	mqtt_client_info.will_retain = 0;
//	if (ipaddr_aton("192.168.1.24", &broker_ipaddr)) {
//		er = ERR_OK;
//	}
	mqtt_client = mqtt_client_new();
	er = mqtt_client_connect(mqtt_client, &broker_ipaddr, 9031,
			mqtt_connection_cb, NULL, &mqtt_client_info);
	vTaskDelay(100);

	printf("\r\b MQTT Service started successfully \r\n");
	if (er == ERR_OK) {
		printf("\r\n Connect to the Server %d.%d.%d.%d",
				mqttHostParam.ip.cdata[0], mqttHostParam.ip.cdata[1],
				mqttHostParam.ip.cdata[2], mqttHostParam.ip.cdata[3]);
	} else {
		printf("\r\n Failed to connect to MQTT server with er: %d", er);
	}
	/*Notify controller the starting is successful*/
	xQueueControl.xState = TASK_RUNNING;
	xQueueSend(xQueueControlHandle, &xQueueControl, 10);
	BaseType_t portBase;
#define port_DEFAULT_DELAY 	1000
	/*a loop in mqtt_modbus_thread_up(); function*/
	char pub_topic[20];
	memset(pub_topic, 0, sizeof(pub_topic));
	strcat(pub_topic, "modbus/");
	strcat(pub_topic, itoa_user(netId, 10));
	strcat(pub_topic, "/uplink");
	mqtt_modbus_thread_up(mqtt_client, pub_topic);
	while (1)
		;
	/*Todo: Take semaphore to reconnect mqtt*/
//		portBase = xSemaphoreTake(netMqttIpSemaphoreHandle,port_DEFAULT_DELAY);
//		if(portBase == pdPASS)
//		{
//			/*Free up old instance and update mqtt server ip*/
//			mqtt_client_free(mqtt_client);
//			broker_ipaddr.addr = mqttHostParam.ip.idata;
//			er = mqtt_client_connect(mqtt_client, &broker_ipaddr, u16_mqtt_port,
//						mqtt_connection_cb, NULL, &mqtt_client_info);
//			if (er == ERR_OK) {
//					printf("\r\n Connect to the Server %d.%d.%d.%d",
//							mqttHostParam.ip.cdata[0], mqttHostParam.ip.cdata[1],
//							mqttHostParam.ip.cdata[2], mqttHostParam.ip.cdata[3]);
//				} else {
//					printf("\r\n Failed to connect to MQTT server with er: %d", er);
//				}
//		}
	/*Todo: Create queue to connect brigde*/
//		example_publish(mqtt_client, NULL);
//		MQTTPublish(&client, topic_sub, &mdata);
//		vTaskDelay(10);
}
