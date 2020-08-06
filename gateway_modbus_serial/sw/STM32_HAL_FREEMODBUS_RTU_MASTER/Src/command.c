#include <command.h>
#include "cmdline.h"
#include "usart.h"
#include "flash.h"
#include "FreeRTOS.h"
#include "main.h"

#include "lwip.h"
#include "socket.h"
#include "lwip/ip4_addr.h"

extern network_param_t netParam;
extern network_param_t mqttHostParam;
extern osThreadId netFlashSave;
extern osThreadId defaultTaskHandle;
extern osSemaphoreId resetHandle;
extern osMessageQId xQueueResetHandle;
extern netId;
extern int_to_byte netIdn[3];
int
Cmd_set_localip(int argc, char *argv[]);
int
Cmd_set_mqttip(int argc, char *argv[]);
int
Cmd_set_id(int argc, char *argv[]);
int
Cmd_save(int argc, char *argv[]);
tCmdLineEntry g_psCmdTable[] = { { "help", Cmd_help,
		" : Display list of commands" }, { "setlocalip", Cmd_set_localip,
		" : Set static ip for brigde" }, { "setmqttip", Cmd_set_mqttip,
		" : Set mqtt server ip for brigde" }, { "setid", Cmd_set_id,
		" : Set mqtt server ip for brigde" }, { "save", Cmd_save,
		" : Save all configuration to flash and reboot" },
//    { "SETRGBLED",      setRGBLED,   "    : set colour of RGB LED" },
//    { "SETLIGHT",     setLight,      "   : set light level" },
//		{ "SETALL",  setAllLight,         ": set all light channel" },
//    { "CONTROLRELAY",  controlRelay,         ": Control the relay" },

		{ 0, 0, 0 } };

const char * ErrorCode[4] = { "CMDLINE_BAD_CMD", "CMDLINE_TOO_MANY_ARGS",
		"CMDLINE_TOO_FEW_ARGS", "CMDLINE_INVALID_ARG" };

uint8_t commandBuffer[100];
uint32_t commandBufferIndex = 0;
uint32_t gotCommandFlag = 0;

void UARTIntHandler(void) {
	uint8_t receivedChar;
	uint32_t ui32Status;
	char *EnterCMD = "\r\n>";
	receivedChar = (uint8_t) ((huart1).Instance->DR & (uint8_t) 0x00FF);

	HAL_UART_Transmit(&huart1, &receivedChar, 1, 100);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
	if (receivedChar != 13) {
		if ((receivedChar == 8) || (receivedChar == 127)) {
			if (commandBufferIndex > 0)
				commandBufferIndex--;
		} else {
			commandBuffer[commandBufferIndex] = receivedChar;
			commandBufferIndex++;
		}
	} else {
		if (commandBufferIndex != 0) {
			commandBuffer[commandBufferIndex] = 0;
			commandBufferIndex = 0;
			gotCommandFlag = 1;
		}
		HAL_UART_Transmit(&huart1, EnterCMD, 3, 100);
	}

}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) {
	}
}
int Cmd_save(int argc, char *argv[]) {

	uint32_t handle = 1;
	xQueueSend(xQueueResetHandle,&handle,portMAX_DELAY);

}
int Cmd_set_localip(int argc, char *argv[]) {
	printf("\nCmd_set_localip\r\n");
	printf("------------------\r\n");
	ip4_addr_t local_ip;
//	HAL_UART_Transmit(&huart1, *argv, strlen(*argv), 100);
//	HAL_UART_Transmit(&huart1, *(argv+1), strlen(*(argv+1)), 100);
	if (ipaddr_aton(*(argv + 1), &local_ip)) {
		netParam.ip.idata = local_ip.addr;
		printf("\r\n New IP: %d %d %d %d", netParam.ip.cdata[0],
				netParam.ip.cdata[1], netParam.ip.cdata[2],
				netParam.ip.cdata[3]);
//	printf("\r\nInvalid param\r\n");

	} else {
		printf("\r\nInvalid param\r\n");
	}
	printf("\r\n------------------\r\n");
	return 0;
}
int Cmd_set_mqttip(int argc, char *argv[]) {
	printf("\nCmd_set_mqttip\r\n");
	printf("------------------\r\n");
	ip4_addr_t broker_ip;
//	HAL_UART_Transmit(&huart1, *argv, strlen(*argv), 100);
//	HAL_UART_Transmit(&huart1, *(argv+1), strlen(*(argv+1)), 100);
	if (ipaddr_aton(*(argv + 1), &broker_ip)) {
		mqttHostParam.ip.idata = broker_ip.addr;
		printf("\r\n New Broker IP: %d %d %d %d", mqttHostParam.ip.cdata[0],
				mqttHostParam.ip.cdata[1], mqttHostParam.ip.cdata[2],
				mqttHostParam.ip.cdata[3]);
//	printf("\r\nInvalid param\r\n");

	} else {
		printf("\r\nInvalid param\r\n");

		printf("\r\n------------------\r\n");
		return 0;
	}
}
int Cmd_set_id(int argc, char *argv[]) {
	printf("\nCmd_set_id\r\n");
	printf("\r\n------------------\r\n");
//	HAL_UART_Transmit(&huart1, argv, argc, 100);
	netId = atol(*(argv+1));
//	memcpy(netIdn, *(argv+1), 12);
	printf("\r\n New net ID: %d",netId);
	printf("\r\n------------------\r\n");
	return 0;
}
int Cmd_help(int argc, char *argv[]) {
	tCmdLineEntry *pEntry;
//      HAL_UART_Transmit(&huart1, "\nAvailable commands\r\n", 1, 100);
// Print some header text.
	printf("\nAvailable commands\r\n");
	printf("------------------\r\n");

// Point at the beginning of the command table.
	pEntry = &g_psCmdTable[0];

// Enter a loop to read each entry from the command table.  The
// end of the table has been reached when the command name is NULL.
	while (pEntry->pcCmd) {
		// Print the command name and the brief description.
		printf("%s%s\n\r", pEntry->pcCmd, pEntry->pcHelp);

		// Advance to the next entry in the table.
		pEntry++;

	}
	printf("------------------\r\n");
// Return success.
	return (0);
}
