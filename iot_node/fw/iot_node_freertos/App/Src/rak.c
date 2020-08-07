#include "rak.h"
#include "main.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include "param.h"
#include "sharedmem.h"

/* Private variables -------------------*/
char rakCommandSendBuffer[RAK_MAX_SEND_LEN];
char commandRecvBuffer[RAK_MAX_RECV_LEN];
uint16_t commandRecvBufferIndex = 0;
//uint8_t gotErrorFlag;
uint8_t gotCommandRecvFlag = 0;
uint8_t globalError = 0;
uint8_t isGetStatus = 0;

lr_packet_t lr_packet_recv;
enum {
	AT_NONE = 0, AT_ERR, AT_CMD, AT_OK, AT_RECV
};
enum {
	LR_NOT_RESPONE, LR_JOINED, LR_NOT_JOINED, LR_ERR, LR_BUSY, LR_RX_TIMEOUT
};
uint8_t isJoinedLoraWAN = 0;
/* Start implementation --------------------*/

/*
 *
 *
 * */
/*Function*/
/**
 * @brief
 *
 * @param
 * @retval None
 */
uint8_t* itoa_user(uint32_t val, uint8_t base) {
	static uint8_t buf[32] = { 0 };  // 32 bits
	int i = 30;
	uint32_t val_ = val;
	if (val == 0) {
		buf[i--] = '0';
//		if (base == 16) {
//			buf[i--] = '0';
//			return &buf[i + 1];
//		}
	}

	for (; val && i; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];
	if ((base == 16) && (((uint8_t) val_) < 16)) {
		buf[i--] = '0';
	}
//	if (base == 16)
//	{
//		buf[i] = '0';
//	}
	return &buf[i + 1];

}
uint8_t rak_waitingAT(void) {
	for (uint8_t u8tryPerSec = 0; u8tryPerSec < 10 * MAX_TIME_OUT;
			u8tryPerSec++) {
		osDelay(100);
		if (gotCommandRecvFlag != AT_NONE) {
			break;
		}
	}
	return gotCommandRecvFlag;
}
uint8_t rak_send_raw(char *datahex) {
	char *EnterCMD = "\r\n";
	HAL_UART_Transmit(&RAK_huart, (uint8_t*) datahex, strlen(datahex), 100);
	HAL_UART_Transmit(&RAK_huart, (uint8_t*) EnterCMD, 3, 100);

	/*Wait for the rak reponse*/
	uint8_t err;
	err = rak_waitingAT();
	DBG("\r\n Send to RAK ");
	DBG(datahex);
	DBG(" with code: %d", err);
	DBG(" and the raw data recv: ");
//	DBG(" and the raw data recv");
	DBG(commandRecvBuffer);
	/**/
//	HAL_UART_Transmit(&huart2, (uint8_t*) commandRecvBuffer, 3, 1000);
	/*	Clear buffer*/
	if (isGetStatus == 0) {
		memset(commandRecvBuffer, 0, RAK_MAX_RECV_LEN);
		gotCommandRecvFlag = AT_NONE;
	}
	return err;
	/* rak not responding*/

}

/*Brief:
 * @param: class Mode: Class A = 0; Class C = 2
 *
 *
 * */
uint8_t rak_setClass(uint8_t classMode) {
	uint8_t err;
	if (classMode > 2)
		return 0;
	memset(rakCommandSendBuffer, 0, RAK_MAX_SEND_LEN);
	strcat(rakCommandSendBuffer, "at+set_config=lora:class:");
	strcat(rakCommandSendBuffer, itoa_user(classMode, 10));
	err = rak_send_raw(rakCommandSendBuffer);
	return err;
}

uint8_t rak_setRegion(uint8_t region) {
	uint8_t err;
	if (region > 9) {
		return 0;
	}
	memset(rakCommandSendBuffer, 0, RAK_MAX_SEND_LEN);
	strcat(rakCommandSendBuffer, "at+set_config=lora:region:");
	switch (region) {
	case 0:

		strcat(rakCommandSendBuffer, "AS923");
		break;
	case 1:

		strcat(rakCommandSendBuffer, "AU915");
		break;
	case 2:

		strcat(rakCommandSendBuffer, "CN470");
		break;
	case 3:

		strcat(rakCommandSendBuffer, "CN779");
		break;
	case 4:

		strcat(rakCommandSendBuffer, "EU433");
		break;
	case 5:

		strcat(rakCommandSendBuffer, "EU868");
		break;
	case 6:

		strcat(rakCommandSendBuffer, "KR920");
		break;
	case 7:

		strcat(rakCommandSendBuffer, "IN865");
		break;
	case 8:

		strcat(rakCommandSendBuffer, "AU915");
		break;
	case 9:

		strcat(rakCommandSendBuffer, "AU915");
		break;
	}
	err = rak_send_raw(rakCommandSendBuffer);
	return err;

}

void rak_reset(void) {
	char *reset = "at+set_config=device:restart";
	char *EnterCMD = "\r\n";
	HAL_UART_Transmit(&RAK_huart, (uint8_t*) reset, strlen(reset), 1000);
	HAL_UART_Transmit(&RAK_huart, (uint8_t*) EnterCMD, 3, 100);
}

/*
 * mode = 0: LoraWAN Mode
 * mode = 1: Lora2P2 Mode
 *
 * */
uint8_t rak_setWorkingMode(uint8_t mode) {
	uint8_t err;
	if (mode > 2)
		return 0;
	memset(rakCommandSendBuffer, 0, RAK_MAX_SEND_LEN);
	strcat(rakCommandSendBuffer, "at+set_config=lora:work_mode:");
	strcat(rakCommandSendBuffer, itoa_user(mode, 10));
	err = rak_send_raw(rakCommandSendBuffer);
	return err;
}

/*
 * Mode = 0: OTAA
 * Mode = 1: ABP
 *
 * */

uint8_t rak_setJoinMode(uint8_t mode) {
	uint8_t err;
	if (mode > 2)
		return 0;
	memset(rakCommandSendBuffer, 0, RAK_MAX_SEND_LEN);
	strcat(rakCommandSendBuffer, "at+set_config=lora:join_mode:");
	strcat(rakCommandSendBuffer, itoa_user(mode, 10));
	err = rak_send_raw(rakCommandSendBuffer);
	return err;
}

uint8_t rak_join(void) {
	uint8_t err;
	memset(rakCommandSendBuffer, 0, RAK_MAX_SEND_LEN);
	strcat(rakCommandSendBuffer, "at+join");
	err = rak_send_raw(rakCommandSendBuffer);

//	for (uint8_t u8tryPerSec = 0; u8tryPerSec < MAX_TIME_OUT; u8tryPerSec++) {
//		osDelay(1000);
//		if (gotCommandRecvFlag == 1) {
//			break;
//		}
//	}
//	if (gotCommandRecvFlag == 1) {
//		gotCommandRecvFlag = 0;
//		/* Process the rak at command*/
//		if (commandRecvBuffer[0] == 'J') // OK
//				{
//
//			DBG("LR joined \r\n");
//			return LR_JOINED;
//		}
//		/*Todo: capture error code for deciding next command */
//		if (commandRecvBuffer[0] == 'E') //ERROR
//				{
//			DBG("LR join error \r\n");
//			return LR_ERR;
//		}
//
//		/*	Clear buffer*/
//		memset(commandRecvBuffer, 0, RAK_MAX_RECV_LEN);
//
//	} else {
//
//		return LR_NOT_RESPONE;
//		DBG("LR not reponse \r\n");
//		/* rak not responding*/
//	}

}

void rak_initOTAA(char *devEUI, char *appEUI, char *appKey) {

	memset(rakCommandSendBuffer, 0, RAK_MAX_SEND_LEN);
	strcat(rakCommandSendBuffer, "at+set_config=lora:dev_eui:");
	strcat(rakCommandSendBuffer, devEUI);
	rak_send_raw(rakCommandSendBuffer);
	memset(rakCommandSendBuffer, 0, RAK_MAX_SEND_LEN);
	strcat(rakCommandSendBuffer, "at+set_config=lora:app_eui:");
	strcat(rakCommandSendBuffer, appEUI);
	rak_send_raw(rakCommandSendBuffer);
	memset(rakCommandSendBuffer, 0, RAK_MAX_SEND_LEN);
	strcat(rakCommandSendBuffer, "appKey+set_config=lora:app_key:");
	strcat(rakCommandSendBuffer, devEUI);
	rak_send_raw(rakCommandSendBuffer);

}
/*
 * Type = 0: unc
 * Type = 1: cfm
 *
 * */
uint8_t rak_isConfirm(uint8_t type) {
	if (type > 1)
		return 0;
	memset(rakCommandSendBuffer, 0, RAK_MAX_SEND_LEN);
	strcat(rakCommandSendBuffer, "at+set_config=lora:confirm:");
	strcat(rakCommandSendBuffer, itoa_user(type, 10));
	return rak_send_raw(rakCommandSendBuffer);
}
uint8_t rak_setTxPower(uint8_t power) {
	memset(rakCommandSendBuffer, 0, RAK_MAX_SEND_LEN);
	strcat(rakCommandSendBuffer, "at+set_config=lora:tx_power:");
	strcat(rakCommandSendBuffer, itoa_user(power, 10));
	return rak_send_raw(rakCommandSendBuffer);
}
uint8_t rak_setAdr(uint8_t type) {
	if (type > 1)
		return 0;
	memset(rakCommandSendBuffer, 0, RAK_MAX_SEND_LEN);
	strcat(rakCommandSendBuffer, "at+set_config=lora:adr:");
	strcat(rakCommandSendBuffer, itoa_user(type, 10));
	return rak_send_raw(rakCommandSendBuffer);
}
uint8_t rak_setDr(uint8_t type) {
	memset(rakCommandSendBuffer, 0, RAK_MAX_SEND_LEN);
	strcat(rakCommandSendBuffer, "at+set_config=lora:dr:");
	strcat(rakCommandSendBuffer, itoa_user(type, 10));
	return rak_send_raw(rakCommandSendBuffer);
}

uint8_t rak_sendData(uint8_t port, char* data) {
	uint8_t err;
	if (port > 63)
		return 0;  // the biggest port is 63
	memset(rakCommandSendBuffer, 0, RAK_MAX_SEND_LEN);
	strcat(rakCommandSendBuffer, "at+send=lora:");
	strcat(rakCommandSendBuffer, itoa_user(port, 10));
	strcat(rakCommandSendBuffer, ":");
	strcat(rakCommandSendBuffer, data);
	err = rak_send_raw(rakCommandSendBuffer);
	return err;

}

uint8_t rak_getStatus(void) {
	isGetStatus = 1;
	char merg[4];
	char *pNetJoinStatus = NULL;
	char *pIndex = NULL;
	commandRecvBufferIndex = 0;
	memset(rakCommandSendBuffer, 0, RAK_MAX_SEND_LEN);
	strcat(rakCommandSendBuffer, "at+get_config=lora:status");
	rak_send_raw(rakCommandSendBuffer);

//	if (gotCommandRecvFlag == 1) {
//		gotCommandRecvFlag = 0;

//		Region: EU868
//		Send_interval: 600s
//		Auto send status: false.
//		Join_mode: ABP
//		DevAddr: 260125D7
//		AppsKey: 841986913ACD00BBC2BE2479D70F3228
//		NwksKey: 69AF20AEA26C01B243945A28C9172B42
//		Class: A
//		Joined Network:true => offset 9
//		IsConfirm: false
//		AdrEnable: true
//		EnableRepeaterSupport: false

	/* Process the rak at command*/
	pNetJoinStatus = strstr(commandRecvBuffer, "Network:");
	if (pNetJoinStatus != NULL) {

		/* Check true false status
		 * Simply only need check bit =)))
		 * */
		if (*(pNetJoinStatus + 9) == 't') {
//				isJoinedLoraWAN = LR_JOINED;
//			return LR_JOINED;
		} else {
//				isJoinedLoraWAN =LR_NOT_JOINED;
//			return LR_NOT_JOINED;
		}
//		}
		/* rak not responding*/
	}
	pIndex = strstr(commandRecvBuffer, "DevEui:");
	if (pIndex != NULL) {
		pIndex = pIndex + 8;
		for (uint8_t idx = 0; idx < NODE_LRWAN_DEVEUI_LEN; idx++) {
			memcpy(merg, (pIndex + 2 * idx), 2);
			u_mem_set(NODE_LRWAN_DEVEUI_ADR + idx,
					(uint8_t) strtol(merg, NULL, 16));
			//				printf("%d \r\n",  (uint8_t) strtol(merg, NULL, 16));
		}
		pIndex = NULL;
	}
	pIndex = strstr(commandRecvBuffer, "AppEui:");
	if (pIndex != NULL) {
		pIndex = pIndex + 8;
		for (uint8_t idx = 0; idx < NODE_LRWAN_APPEUI_LEN; idx++) {
			memcpy(merg, (pIndex + 2 * idx), 2);
			u_mem_set(NODE_LRWAN_APPEUI_ADR + idx,
					(uint8_t) strtol(merg, NULL, 16));
			//				printf("%d \r\n",  (uint8_t) strtol(merg, NULL, 16));
		}
		pIndex = NULL;
	}
	pIndex = strstr(commandRecvBuffer, "AppKey:");
	if (pIndex != NULL) {
		pIndex = pIndex + 8;
		for (uint8_t idx = 0; idx < NODE_LRWAN_APPKEY_LEN; idx++) {
			memcpy(merg, (pIndex + 2 * idx), 2);
			u_mem_set(NODE_LRWAN_APPKEY_ADR + idx,
					(uint8_t) strtol(merg, NULL, 16));
			//				printf("%d \r\n",  (uint8_t) strtol(merg, NULL, 16));
		}
		pIndex = NULL;
	}
	isGetStatus = 0;
	commandRecvBufferIndex = 0;
//		memset(commandRecvBuffer, 0, RAK_MAX_RECV_LEN);
}

/*Brief: command received processing
 *
 * **/
void rak_command_recv_process(void) {
	/*Packet support extraction  variables*/
	uint8_t buf[4];
	uint8_t* pColon;
	if (strcmp(commandRecvBuffer, "at+recv") == 0) {
		if ((strchr(commandRecvBuffer, '=') + 1) == '0') {
			/*This is ACK.*/
		} else {
			/*
			 * The downlink recieved at port 22 and the pay load is: 6e7474
			 * port,rssi,fnc,snr,payload
			 * Content:  at+recv=22,-54,9,3:6e7474{0D}{0A}*/
			if ((strchr(commandRecvBuffer, '=') + 3) != ',') {
				memset(buf, 0, 4);
				memcpy(buf, (strchr(commandRecvBuffer, '=') + 1), 2);
				lr_packet_recv.port = atoi(buf);
			} else {
				memset(buf, 0, 4);
				memcpy(buf, (strchr(commandRecvBuffer, '=') + 1), 1);
				lr_packet_recv.port = atoi(buf);
			}
			pColon = strchr(commandRecvBuffer, ':');
			if (*(pColon - 2) != ',') {
				memset(buf, 0, 4);
				memcpy(buf, (pColon - 2), 2);
				lr_packet_recv.len = atoi(buf);
			} else {
				memset(buf, 0, 4);
				memcpy(buf, (pColon - 1), 1);
				lr_packet_recv.len = atoi(buf);
			}

			for (uint8_t index = 0; index < lr_packet_recv.len; index++) {
				//
				memcpy(buf, (pColon + 1) + index * 2, 2);
				lr_packet_recv.pPayload[index] = strtol(buf, NULL, 16);
			}
			//memcpy(&lr_packet_recv.pPayload[0],(strchr(rakCommandSendBuffer, ':') + 1), strlen(strchr(rakCommandSendBuffer, ':') + 1
		}
	}

//	/*ERROR code */
//	if (strcmp(commandRecvBuffer, "ER") == 0) {
//
//		if ((commandRecvBuffer[7] == '8') && (commandRecvBuffer[8] == '0')) {
//			isJoinedLoraWAN = LR_BUSY;
//		}
//		if ((commandRecvBuffer[7] == '8') && (commandRecvBuffer[8] == '6')) {
//			isJoinedLoraWAN = LR_NOT_JOINED;
//		}
//	}

}

uint8_t rak_err_detector(void) {

	uint8_t error;
	globalError = atoi(&commandRecvBuffer[7]);
	error = globalError;
	DBG("Error code %d \r\n", error);
	memset(commandRecvBuffer, 0, RAK_MAX_RECV_LEN);

	return error;
}
/*Brief: Handle AT command received over serial interrupt
 *
 *
 * */
void rak_recv_isr(void) {
	uint8_t receivedChar;

	receivedChar = (uint8_t) ((RAK_huart).Instance->DR & (uint8_t) 0x00FF);
	if (isGetStatus == 0) {
		if ((receivedChar != 13) && (receivedChar != 10)) {

			commandRecvBuffer[commandRecvBufferIndex] = receivedChar;
			commandRecvBufferIndex++;
		}

		else {
//		commandRecvBuffer[commandRecvBufferIndex] = 0;

//		gotCommandRecvFlag = 1;
			if (commandRecvBuffer[1] == 'R') {
				//set Flag to call Error code detector
				gotCommandRecvFlag = AT_ERR;
				rak_err_detector();
			} else if (commandRecvBuffer[1] == 't') {
				//set flag to notify the command received
				gotCommandRecvFlag = AT_RECV;

			} else if ((commandRecvBuffer[1] == 'O')
					|| (commandRecvBuffer[1] == 'K')) {
				gotCommandRecvFlag = AT_OK;
			} else {
				gotCommandRecvFlag = AT_CMD;
			}

			commandRecvBufferIndex = 0;

		}
		if (commandRecvBufferIndex == RAK_MAX_RECV_LEN)
			commandRecvBufferIndex = 0;
	} else {
		commandRecvBuffer[commandRecvBufferIndex] = receivedChar;
		commandRecvBufferIndex++;
		if (commandRecvBufferIndex == RAK_MAX_RECV_LEN)
			commandRecvBufferIndex = 0;
	}

//	HAL_UART_Transmit(&RAK_huart, &receivedChar, 1, 100);
}

void vRakLRWAN(void) {

	char * DevEui = "60C5A8FFFE000001";
	char * AppEui = "70B3D57EF00047C0";
	char * AppKey = "5D833B4696D5E01E2F8DC880E30BA5FE";
	char * pData = "aabbccdd";
	char lrTxBuffer[60];

	rak_setClass(PARAM[NODE_LRWAN_CLASS_ADR]);
	rak_setRegion(PARAM[NODE_LRWAN_FREQ_ADR]);
	rak_setJoinMode(PARAM[NODE_LRWAN_MODE_ADR]);
	rak_isConfirm(PARAM[NODE_LRWAN_CONFIRM_ADR]);

	//	##rak_initOTAA(DevEui, AppEui, AppKey); // not call, use the default keys
	rak_setTxPower(PARAM[NODE_LRWAN_TX_POWER_ADR]);
	rak_isConfirm(PARAM[NODE_LRWAN_CONFIRM_ADR]);
	/* Datarate parameter**/
	if (PARAM[NODE_LRWAN_DATARATE_ADR] == 0) {
		/* if Adr is On, don't care any datarate **/
		rak_setAdr(1);
	} else {
		/* if adr is off, set static */
		rak_setDr(PARAM[NODE_LRWAN_CONFIRM_ADR]);
	}

	uint8_t err;
	if (PARAM[NODE_LRWAN_AUTO_JOIN_ADR] != 0) {
		err = rak_join();
		if (err == AT_CMD) {
			isJoinedLoraWAN = LR_JOINED;
			DBG("LR JOINED \r\n");
		} else {
			isJoinedLoraWAN = LR_NOT_JOINED;
			DBG("LR NOTJOINED \r\n");
		}
	}

	//	rak_setDr(5);
	while (1) {
		/*Thread up*/

		memset(lrTxBuffer, 0, 50);
		//		strcat(lrTxBuffer,itoa_user(PORT_IO_SW_1, 16));
		//		strcat(lrTxBuffer,itoa_user((uint8_t)(uiMemGet(PORT_IO_SW_1) >> 8), 16));
		//		strcat(lrTxBuffer,itoa_user((uint8_t)(uiMemGet(PORT_IO_SW_1) ), 16));
		//
		//		strcat(lrTxBuffer,itoa_user(PORT_IO_SW_2, 16));
		//		strcat(lrTxBuffer,itoa_user((uint8_t)(uiMemGet(PORT_IO_SW_2) >> 8), 16));
		//		strcat(lrTxBuffer,itoa_user((uint8_t)(uiMemGet(PORT_IO_SW_2) ), 16));
		//
		//		strcat(lrTxBuffer,itoa_user(PORT_PULSE, 16));
		//		strcat(lrTxBuffer,itoa_user((uint8_t)(uiMemGet(PORT_PULSE) >> 8), 16));
		//		strcat(lrTxBuffer,itoa_user((uint8_t)(uiMemGet(PORT_PULSE) ), 16));
		//

		//
		//		strcat(lrTxBuffer,itoa_user(PORT_ADC_2, 16));
		//		strcat(lrTxBuffer,itoa_user((uint8_t)(uiMemGet(PORT_ADC_2) >> 8), 16));
		//		strcat(lrTxBuffer,itoa_user((uint8_t)(uiMemGet(PORT_ADC_2) ), 16));
		//
		strcat(lrTxBuffer, itoa_user(1, 16));
		strcat(lrTxBuffer,
				itoa_user((uint8_t) (uiMemGet(PORT_ONE_WIRE) >> 8), 16));
		strcat(lrTxBuffer, itoa_user((uint8_t) (uiMemGet(PORT_ONE_WIRE)), 16));
		//
		strcat(lrTxBuffer, itoa_user(2, 16));
		strcat(lrTxBuffer,
				itoa_user((uint8_t) (uiMemGet(PORT_ONE_WIRE_) >> 8), 16));
		strcat(lrTxBuffer, itoa_user((uint8_t) (uiMemGet(PORT_ONE_WIRE_)), 16));
		strcat(lrTxBuffer, itoa_user(3, 16));
		strcat(lrTxBuffer,
				itoa_user((uint8_t) (uiMemGet(PORT_ADC_1) >> 8), 16));
		strcat(lrTxBuffer, itoa_user((uint8_t) (uiMemGet(PORT_ADC_1)), 16));

//uplink

		err = rak_sendData(1, lrTxBuffer);
		if (err == AT_ERR) {
			DBG("LRWAN error code: %d \r\n", globalError);
			switch (globalError) {
			case 86:
				rak_join();
				globalError = 0;
				DBG("Rejoining ... \r\n");
				break;

			case 80:
				globalError = 0;
				osDelay(4000);
				break;

			default:
				break;
			}
		}

		/*Thread down*/

		/*Handling*/
		isJoinedLoraWAN = rak_getStatus();
		//		if (isJoinedLoraWAN == LR_NOT_JOINED) {
		//			DBG("Re joining to LRWAN: ");
		//			osDelay(3000);
		//			isJoinedLoraWAN = rak_join();
		//			DBG("Re joining to LRWAN with code: %d \r\n", isJoinedLoraWAN);
		//		}
		//		if (isJoinedLoraWAN == LR_BUSY) {
		//			DBG("LRWAN is busy now, sleeping in 5s \r\n");
		//			osDelay(5000);
		//		}
		/*
		 * Receive downlink data and error code detecting
		 *
		 * **/

		if (gotCommandRecvFlag != AT_NONE) {
			switch (gotCommandRecvFlag) {
			case AT_ERR: {
				//				err = rak_err_detector();
				if (globalError == 86)
					isJoinedLoraWAN = LR_NOT_JOINED;
				if (globalError == 80)
					isJoinedLoraWAN = LR_BUSY;
				if (globalError == 96)
					isJoinedLoraWAN = LR_RX_TIMEOUT;
				if (globalError == 99)
					isJoinedLoraWAN = LR_NOT_JOINED;

				DBG("LRWAN error code: %d \r\n", err);

				break;
			}
			case AT_RECV: {
				rak_command_recv_process();
				break;
			}
			case AT_CMD: {

				break;
			}
			}

			gotCommandRecvFlag = AT_NONE;
			/*	Clear buffer*/
			memset(commandRecvBuffer, 0, RAK_MAX_RECV_LEN);

			osDelay(5000);
		}
	}
}

void vRakTask(void const *arg) {

	/* Os platform**/

	/* RAK platform*/

	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
	rak_getStatus();

//	if (PARAM[NODE_HAVE_PARAM_ADR] == EEP_PARAM) {
//		for (uint8_t idx = 0; idx < 8; idx++) {
//			strcat(DevEui,
//					itoa_user(u_mem_get(NODE_LRWAN_DEVEUI_ADR + idx), 16));
//		}
//		for (uint8_t idx = 0; idx < 16; idx++) {
//			strcat(AppEui,
//					itoa_user(u_mem_get(NODE_LRWAN_APPEUI_ADR + idx), 16));
//		}
//		for (uint8_t idx = 0; idx < 16; idx++) {
//			strcat(AppKey,
//					itoa_user(u_mem_get(NODE_LRWAN_APPKEY_ADR + idx), 16));
//		}
//	}
	/*Initial */
	if (PARAM[NODE_LRWAN_WORKMODE_ADR] == LR_WAN) {
		vRakLRWAN();
	} else {
		while (1)
			;
	}

}
