#include <command.h>
#include <string.h>
#include "cmdline.h"
#include "main.h"
#include "param.h"
#include "usart.h"
#include "envtask.h"
#include "epprom.h"
#include "rak.h"
tCmdLineEntry g_psCmdTable[] = { { "help", Cmd_help,
		" : Display list of commands" }, { "set", Cmd_set,
		" : Set the specific param in the next arg" }, { "get", Cmd_get,
		" : Get the specific param in the next arg" },

{ 0, 0, 0 } };

const char * ErrorCode[4] = { "CMDLINE_BAD_CMD", "CMDLINE_TOO_MANY_ARGS",
		"CMDLINE_TOO_FEW_ARGS", "CMDLINE_INVALID_ARG" };

uint8_t commandBuffer[100];
uint8_t commandSendBuffer[100];
uint32_t commandBufferIndex = 0;
uint32_t gotCommandFlag = 0;
uint8_t isEcho = 1;

/*Brief: must be mapped to Rx Interrupt!
 *
 * */
void command_recv_callback_irq(UART_HandleTypeDef *huart) {
	uint8_t receivedChar;
	uint32_t ui32Status;
	char *EnterCMD = "\r\n>";
	receivedChar = (uint8_t) ((*huart).Instance->DR & (uint8_t) 0x00FF);

	if (isEcho == 1)
//		HAL_UART_Transmit_IT(huart, &receivedChar, 1);

//	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
		if ((receivedChar != 10) && (receivedChar != 13)) {
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
			if (isEcho == 1)
//			HAL_UART_Transmit_IT(huart, EnterCMD, 3);
				PRINTF_EN = 0;
		}

}
int Cmd_set(int argc, char *argv[]) {
	char merg[4];
	uint8_t port;
	if ((strcmp(*(argv + 1), "mb") == 0)) {

		if ((strcmp(*(argv + 2), "mode") == 0)) {
			u_mem_set(NODE_MB_MODE_ADR, atoi(*(argv + 3)));
		} else if ((strcmp(*(argv + 2), "baud") == 0)) {
			u_mem_set(NODE_MB_BAUD_ADR, atoi(*(argv + 3)));
		} else if ((strcmp(*(argv + 2), "db") == 0)) {
			u_mem_set(NODE_MB_DATABITS_ADR, atoi(*(argv + 3)));
		} else if ((strcmp(*(argv + 2), "stbi") == 0)) {
			u_mem_set(NODE_MB_STOPBITS_ADR, atoi(*(argv + 3)));
//			strcat(commandSendBuffer,
//					itoa_user(u_mem_get(NODE_MB_STOPBITS_ADR), 10));
		} else if ((strcmp(*(argv + 2), "pari") == 0)) {
			u_mem_set(NODE_MB_PARTITY_ADR, atoi(*(argv + 3)));
		} else if ((strcmp(*(argv + 2), "id") == 0)) {
			u_mem_set(NODE_MB_ID_ADR, atoi(*(argv + 3)));
		}
	} else if ((strcmp(*(argv + 1), "lr") == 0)) {
		if ((strcmp(*(argv + 2), "adr") == 0)) {

			u_mem_set(NODE_LRWAN_DATARATE_ADR, atoi(*(argv + 3)));

		}
		if ((strcmp(*(argv + 2), "fr") == 0)) {

			u_mem_set(NODE_LRWAN_FREQ_ADR, atoi(*(argv + 3)));

		}
		if ((strcmp(*(argv + 2), "cf") == 0)) {

			u_mem_set(NODE_LRWAN_CONFIRM_ADR, atoi(*(argv + 3)));

		}
		if ((strcmp(*(argv + 2), "class") == 0)) {

			u_mem_set(NODE_LRWAN_CLASS_ADR, atoi(*(argv + 3)));

		}
		if ((strcmp(*(argv + 2), "pwr") == 0)) {

			u_mem_set(NODE_LRWAN_TX_POWER_ADR, atoi(*(argv + 3)));

		}
		if ((strcmp(*(argv + 2), "auto") == 0)) {

			u_mem_set(NODE_LRWAN_AUTO_JOIN_ADR, atoi(*(argv + 3)));

		}
		if ((strcmp(*(argv + 2), "join") == 0)) {

			//u_mem_set(NODE_LRWAN_AUTO_JOIN_ADR, atoi(*(argv + 3)));
			//give semaphore to rak task
		}
		if ((strcmp(*(argv + 2), "deveui") == 0)) {
			for (uint8_t idx = 0; idx < NODE_LRWAN_DEVEUI_LEN; idx++) {
				memcpy(merg, *(argv + 3) + 2 * idx, 2);
				u_mem_set(NODE_LRWAN_DEVEUI_ADR + idx,
						(uint8_t) strtol(merg, NULL, 16));
//				printf("%d \r\n",  (uint8_t) strtol(merg, NULL, 16));
			}

		}
		if ((strcmp(*(argv + 2), "appeui") == 0)) {
			for (uint8_t idx = 0; idx < NODE_LRWAN_APPEUI_LEN; idx++) {
				memcpy(merg, *(argv + 3) + 2 * idx, 2);
				u_mem_set(NODE_LRWAN_APPEUI_ADR + idx,
						(uint8_t) strtol(merg, NULL, 16));
			}

		}
		if ((strcmp(*(argv + 2), "appkey") == 0)) {
			for (uint8_t idx = 0; idx < NODE_LRWAN_APPKEY_LEN; idx++) {
				memcpy(merg, *(argv + 3) + 2 * idx, 2);
				u_mem_set(NODE_LRWAN_APPKEY_ADR + idx,
						(uint8_t) strtol(merg, NULL, 16));
			}

		}
		if ((strcmp(*(argv + 2), "mode") == 0)) {
			u_mem_set(NODE_LRWAN_MODE_ADR, atoi(*(argv + 3)));
//					strcat(commandSendBuffer,
//							itoa_user(u_mem_get(NODE_LRWAN_MODE_ADR), 10));

		}
	} else if ((strcmp(*(argv + 1), "id") == 0)) {
		u_mem_set(NODE_ID_ADR, atoi(*(argv + 2)));

	} else if ((strcmp(*(argv + 1), "cf") == 0)) {
		u_mem_set(NODE_HAVE_PARAM_ADR, EEP_PARAM);
	}
	else if ((strcmp(*(argv + 1), "mbmode") == 0)) {
			u_mem_set(NODE_MB_WORKMODE_ADR, atoi(*(argv + 2)));
		}
	else if ((strcmp(*(argv + 1), "lrmode") == 0)) {
			u_mem_set(NODE_LRWAN_WORKMODE_ADR, atoi(*(argv + 2)));
		}
//	else if ((strcmp(*(argv + 1), "port") == 0)) {
//		port = atoi((*(argv + 2)));
//		u_mem_set(NODE_IO_BASE + 2 * port, atoi((*(argv + 3))));
//		u_mem_set(NODE_IO_BASE + 2 * port + 1, atoi((*(argv + 4))));
//
//	}
	else if ((strcmp(*(argv + 1), "p_pf") == 0)) {

		port = atoi(*(argv + 2));
		u_mem_set(NODE_IO_BASE + 2 * port, atoi((*(argv + 3))));
	} else if ((strcmp(*(argv + 1), "save") == 0)) {
		v_epr_save(PARAM_LOAD_ALL);

	} else if ((strcmp(*(argv + 1), "reset") == 0)) {
		NVIC_SystemReset();

	}
	/* Reponse -----------------------------------------------------*/
	char *reponse = "OK\r\n";
	HAL_UART_Transmit(&huart2, reponse, 4, 1000);

}
int Cmd_get(int argc, char *argv[]) {

	uint8_t port = 0;
	memset(commandSendBuffer, 0, 50);
	if ((strcmp(*(argv + 1), "mb") == 0)) {

		if ((strcmp(*(argv + 2), "mode") == 0)) {
			strcat(commandSendBuffer,
					itoa_user(u_mem_get(NODE_MB_MODE_ADR), 10));
		} else if ((strcmp(*(argv + 2), "baud") == 0)) {
			strcat(commandSendBuffer,
					itoa_user(u_mem_get(NODE_MB_BAUD_ADR), 10));
		} else if ((strcmp(*(argv + 2), "db") == 0)) {
			strcat(commandSendBuffer,
					itoa_user(u_mem_get(NODE_MB_DATABITS_ADR), 10));
		} else if ((strcmp(*(argv + 2), "stbi") == 0)) {
			strcat(commandSendBuffer,
					itoa_user(u_mem_get(NODE_MB_STOPBITS_ADR), 10));
		} else if ((strcmp(*(argv + 2), "pari") == 0)) {
			strcat(commandSendBuffer,
					itoa_user(u_mem_get(NODE_MB_PARTITY_ADR), 10));
		} else if ((strcmp(*(argv + 2), "id") == 0)) {
			strcat(commandSendBuffer, itoa_user(u_mem_get(NODE_MB_ID_ADR), 10));
		}
	} else if ((strcmp(*(argv + 1), "lr") == 0)) {
		if ((strcmp(*(argv + 2), "adr") == 0)) {
			strcat(commandSendBuffer,
					itoa_user(u_mem_get(NODE_LRWAN_DATARATE_ADR), 10));
//			u_mem_set(NODE_LRWAN_DATARATE_ADR, atoi(*(argv + 3)));

		}
		if ((strcmp(*(argv + 2), "fr") == 0)) {
			strcat(commandSendBuffer,
					itoa_user(u_mem_get(NODE_LRWAN_FREQ_ADR), 10));

//			u_mem_set(NODE_LRWAN_FREQ_ADR, atoi(*(argv + 3)));

		}
		if ((strcmp(*(argv + 2), "cf") == 0)) {
			strcat(commandSendBuffer,
					itoa_user(u_mem_get(NODE_LRWAN_CONFIRM_ADR), 10));

//			u_mem_set(NODE_LRWAN_CONFIRM_ADR, atoi(*(argv + 3)));

		}
		if ((strcmp(*(argv + 2), "class") == 0)) {
			strcat(commandSendBuffer,
					itoa_user(u_mem_get(NODE_LRWAN_CLASS_ADR), 10));

//			u_mem_set(NODE_LRWAN_CLASS_ADR, atoi(*(argv + 3)));

		}
		if ((strcmp(*(argv + 2), "pwr") == 0)) {
			strcat(commandSendBuffer,
					itoa_user(u_mem_get(NODE_LRWAN_TX_POWER_ADR), 10));

		}
		if ((strcmp(*(argv + 2), "auto") == 0)) {
			strcat(commandSendBuffer,
					itoa_user(u_mem_get(NODE_LRWAN_AUTO_JOIN_ADR), 10));

		}

		if ((strcmp(*(argv + 2), "join") == 0)) {
//						strcat(commandSendBuffer,
//								itoa_user(u_mem_get(NODE_LRWAN_TX_POWER_ADR), 10));

			strcat(commandSendBuffer, itoa_user(isJoinedLoraWAN, 10));
		}
		if ((strcmp(*(argv + 2), "mode") == 0)) {
			strcat(commandSendBuffer,
					itoa_user(u_mem_get(NODE_LRWAN_MODE_ADR), 10));

		}
		if ((strcmp(*(argv + 2), "deveui") == 0)) {
			for (uint8_t idx = 0; idx < NODE_LRWAN_DEVEUI_LEN; idx++) {
				strcat(commandSendBuffer,
						itoa_user(u_mem_get(NODE_LRWAN_DEVEUI_ADR + idx), 16));

				//printf("%d \r\n", u_mem_get(NODE_LRWAN_DEVEUI_ADR + idx));
			}

		}
		if ((strcmp(*(argv + 2), "appeui") == 0)) {
			for (uint8_t idx = 0; idx < NODE_LRWAN_APPEUI_LEN; idx++) {
				strcat(commandSendBuffer,
						itoa_user(u_mem_get(NODE_LRWAN_APPEUI_ADR + idx), 16));
			}

		}
		if ((strcmp(*(argv + 2), "appkey") == 0)) {
			for (uint8_t idx = 0; idx < NODE_LRWAN_APPKEY_LEN; idx++) {
				strcat(commandSendBuffer,
						itoa_user(u_mem_get(NODE_LRWAN_APPKEY_ADR + idx), 16));
			}

		}
	} else if ((strcmp(*(argv + 1), "id") == 0)) {
		strcat(commandSendBuffer, itoa_user(u_mem_get(NODE_ID_ADR), 10));

	} else if ((strcmp(*(argv + 1), "cf") == 0)) {
		strcat(commandSendBuffer,
				itoa_user(u_mem_get(NODE_HAVE_PARAM_ADR), 10));
	} else if ((strcmp(*(argv + 1), "mbmode") == 0)) {
		strcat(commandSendBuffer,
				itoa_user(u_mem_get(NODE_MB_WORKMODE_ADR), 10));
	} else if ((strcmp(*(argv + 1), "lrmode") == 0)) {
		strcat(commandSendBuffer,
				itoa_user(u_mem_get(NODE_LRWAN_WORKMODE_ADR), 10));
	}

	else if ((strcmp(*(argv + 1), "p_pf") == 0)) {

		port = atoi(*(argv + 2));
		strcat(commandSendBuffer,
				itoa_user(u_mem_get(NODE_IO_BASE + 2 * port), 10));
	}

//	else if ((strcmp(*(argv + 1), "p_val") == 0)) {
//
//			port = atoi(*(argv + 2));
//			strcat(commandSendBuffer,
//					itoa_user(u_mem_get(NODE_IO_BASE + 2 * port),
//							10));
//		}
//	}

//	else if ((strcmp(*(argv + 1), "cf") == 0)) {
//		strcat(commandSendBuffer,
//				itoa_user(u_mem_get(NODE_HAVE_PARAM_ADR), 10));
//	}
	strcat(commandSendBuffer, "\r\n");
	/* Reponse -----------------------------------------------------*/
	HAL_UART_Transmit(&huart2, commandSendBuffer, strlen(commandSendBuffer),
			1000);

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

void vCmdTask(const void *arg) {
	char *Badcommand = "Bad Command\r\n>";
	int ret;
	while (1) {

		if (gotCommandFlag) {
			gotCommandFlag = 0;
			ret = CmdLineProcess(commandBuffer);
			if (ret == CMDLINE_BAD_CMD)
				HAL_UART_Transmit(&huart2, Badcommand, strlen(Badcommand), 100);
			//					UARTprintf();
		}
		osDelay(10);
	}

}
