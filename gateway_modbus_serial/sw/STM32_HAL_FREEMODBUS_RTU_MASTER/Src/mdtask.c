/* Built-in C library includes ---------------*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* Platform includes --------------------------*/
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "main.h"
#include "sharedmem.h"
//#include "mb.h"
#include "mb_m.h"
#include "mbframe.h"
#include "mbport.h"
#include "mbconfig.h"
/* Shared Variable ----------------------------------*/
osThreadId mbProtocolTask;
osThreadId mbAppTask;
extern osMessageQId xQueueDownlinkHandle;
extern osMessageQId xQueueUplinkHandle;
extern TIM_HandleTypeDef htim7;
extern osTimerId myTimer01Handle;
extern osMessageQId xQueueControlHandle;
extern osMessageQId xQueueMessageHandle;
/* Private variables ---------------------------------------------------------*/
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 8

#define M_REG_HOLDING_START           0
#define M_REG_HOLDING_NREGS           40

static USHORT usRegInputStart = REG_INPUT_START;
static USHORT usRegInputBuf[REG_INPUT_NREGS];

USHORT usMRegHoldStart = M_REG_HOLDING_START;
USHORT usMRegHoldBuf[MB_RS485_MAX_PORT][MB_MASTER_TOTAL_SLAVE_NUM][M_REG_HOLDING_NREGS];

void ModbusRTUTask(void const *argument) {
#define PORT_INF_DELAY 0
	/* ABCDEF */
	usRegInputBuf[0] = 11;
	usRegInputBuf[1] = 22;
	usRegInputBuf[2] = 33;
	usRegInputBuf[3] = 44;
	usRegInputBuf[4] = 55;
	usRegInputBuf[5] = 66;
	usRegInputBuf[6] = 77;
	usRegInputBuf[7] = 88;
	osDelay(150);
	printf("\r\n ModbusRTUTask \r\n");
	BaseType_t xError;
	xQueueControl_t xQueueControl;
	uint8_t uiSysState;
	xQueueControl.xTask = mbProtocolTask;
	do {
		osDelay(10);
		xQueuePeek(xQueueMessageHandle, &uiSysState, 0);
	} while (uiSysState != SYS_MB_PROTOCOL);
	xQueueReceive(xQueueMessageHandle, &uiSysState, 0);

	printf("\r\n ModbusRTUTask Initing");

	eMBErrorCode eStatus = eMBMasterInit(MB_RTU, 1, 9600, MB_PAR_NONE);
	eStatus = eMBMasterEnable(PORT1);
	eStatus = eMBMasterEnable(PORT2);
	eStatus = eMBMasterEnable(PORT3);
	eStatus = eMBMasterEnable(PORT4);


	 HAL_TIM_Base_Start_IT(&htim7);

	/*State control machine*/
	xQueueControl.xState = TASK_RUNNING;
	xQueueSend(xQueueControlHandle, &xQueueControl, 10);
	while (1) {
		eMBMasterPoll();
		vTaskDelay(1);
	}
}

void ModbusTestTask(void const *argument) {
	osDelay(100);
	printf("\r\n ModbusTestTask \r\n");
	BaseType_t xError;
	xQueueControl_t xQueueControl;
	uint8_t uiSysState;
	xQueueControl.xTask = mbAppTask;
	do {
		osDelay(10);
		xQueuePeek(xQueueMessageHandle, &uiSysState, 0);
	} while (uiSysState != SYS_MB_APP);
	xQueueReceive(xQueueMessageHandle, &uiSysState, 0);
	printf("\r\n ModbusTestTask: Starting");
#define portDEFAULT_WAIT_TIME 1000
	BaseType_t Err = pdFALSE;
	xQueueMbMqtt_t xQueueMbMqtt;

	osDelay(500);
	eMBErrorCode eStatus = MB_ENOERR;
	uint8_t count = 0;

	xQueueControl.xState = TASK_RUNNING;
	xQueueSend(xQueueControlHandle, &xQueueControl, 10);
#define MB_DEFAULT_TEST_PORT	0x00
#define MB_DEFAULT_TEST_NREG	0x01
#define MB_DEFAULT_TEST_TIMEOUT 1
//#define MB_DEFAULT_TEST_R
	while (1) {
		Err = xQueueReceive(xQueueDownlinkHandle, &xQueueMbMqtt,
		portDEFAULT_WAIT_TIME);
		if (Err == pdPASS) {
			printf("\r\n Modbus_MQTT Downlink queued: Reveived \r\n");
			switch (xQueueMbMqtt.FunC) {
			case MB_FUNC_READ_HOLDING_REGISTER:
				eMBMasterReqReadHoldingRegister(xQueueMbMqtt.PortID,
						xQueueMbMqtt.NodeID, xQueueMbMqtt.RegAdr.i16data,
						MB_DEFAULT_TEST_NREG, MB_DEFAULT_TEST_TIMEOUT);

				break;
			case MB_FUNC_WRITE_REGISTER:
				eMBMasterReqWriteHoldingRegister(xQueueMbMqtt.PortID,
						xQueueMbMqtt.NodeID, xQueueMbMqtt.RegAdr.i16data,
						xQueueMbMqtt.RegData.i16data, MB_DEFAULT_TEST_NREG);
				break;
			}

		} else {
//			printf("\r\n Modbus_MQTT Downlink queued: False \r\n");
		}
	}
//	while (1) {
//
//		eMBMasterReqReadHoldingRegister(PORT1, 0x01, FUNC_READ_TEMP_ADR, 1, 1);
//		eMBMasterReqWriteHoldingRegister(0x00, 0x01, 21, 0x01, 0x01);
//
//		osDelay(500);
////		printf("\r\n Get FUNC_READ_TEMP_ADR: ");
////		printf("\r\n Get FUNC_READ_HUMD_ADR: ");
//		eMBMasterReqReadHoldingRegister(PORT1, 0x01, FUNC_READ_HUMD_ADR, 1, 1);
////
////				printf("\r\n Get FUNC_READ_LIGH_ADR: ");
//
////		osDelay(1);
//		eMBMasterReqReadHoldingRegister(0x00, 0x01, 0x00, 2, 1);
//		eMBMasterReqWriteHoldingRegister(PORT3, 0x01, 21, 0x00, 0x01);
////		if (count == 250)
////			count = 0;
//		printf("\r\nModbusTestTask: Sent data to dest");
//
//	}
}

eMBErrorCode eMBRegInputCB(UCHAR *pucRegBuffer, USHORT usAddress,
		USHORT usNRegs) {
	eMBErrorCode eStatus = MB_ENOERR;
	int iRegIndex;

	if ((usAddress >= REG_INPUT_START)
			&& (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS)) {
		iRegIndex = (int) (usAddress - usRegInputStart);
		while (usNRegs > 0) {
			*pucRegBuffer++ = (unsigned char) (usRegInputBuf[iRegIndex] >> 8);
			*pucRegBuffer++ = (unsigned char) (usRegInputBuf[iRegIndex] & 0xFF);
			iRegIndex++;
			usNRegs--;
		}

		//	HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
	} else {
		// HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
		eStatus = MB_ENOREG;
	}

	return eStatus;
}

eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress,
		USHORT usNRegs, eMBRegisterMode eMode) {
	return MB_ENOERR;
}

eMBErrorCode eMBRegCoilsCB(UCHAR *pucRegBuffer, USHORT usAddress,
		USHORT usNCoils, eMBRegisterMode eMode) {
	return MB_ENOREG;
}

eMBErrorCode eMBRegDiscreteCB(UCHAR *pucRegBuffer, USHORT usAddress,
		USHORT usNDiscrete) {
	return MB_ENOREG;
}

eMBErrorCode eMBMasterRegHoldingCB(UCHAR ucPort, UCHAR * pucRegBuffer, USHORT usAddress,
		USHORT usNRegs, eMBRegisterMode eMode) {
	eMBErrorCode eStatus = MB_ENOERR;
	USHORT iRegIndex;
	USHORT * pusRegHoldingBuf;
	USHORT REG_HOLDING_START;
	USHORT REG_HOLDING_NREGS;
	USHORT usRegHoldStart;

	/* FreeRTOS variable*/
	xQueueMbMqtt_t xQueueMbMqtt;
	xQueueMbMqtt.PortID = ucPort;
	xQueueMbMqtt.NodeID = ucMBMasterGetDestAddress(ucPort);

	pusRegHoldingBuf = usMRegHoldBuf[ucPort][ucMBMasterGetDestAddress(ucPort)
			- 1];
	REG_HOLDING_START = M_REG_HOLDING_START;
	REG_HOLDING_NREGS = M_REG_HOLDING_NREGS;
	usRegHoldStart = usMRegHoldStart;
	/* if mode is read, the master will write the received date to buffer. */
//	eMode = MB_REG_READ; // don't sure this case
	/* it already plus one in modbus function method. Tao dell biet no inc lam gi de roi dec xuong */
	usAddress--;
	xQueueMbMqtt.RegAdr.i8data[0] = (uint8_t)usAddress;
	xQueueMbMqtt.RegAdr.i8data[1] = (uint8_t)(usAddress >>8);
	if ((usAddress >= REG_HOLDING_START)
			&& ((uint8_t)usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS)) {
		iRegIndex = usAddress - usRegHoldStart;
		switch (eMode) {
		/* read current register values from the protocol stack. */
		//Swap this case
		case MB_REG_WRITE:
			xQueueMbMqtt.FunC = MB_FUNC_WRITE_REGISTER;
			while (usNRegs > 0) {
				/*            	mqtt_modbus_thread_up(); */
				xQueueMbMqtt.RegData.i8data[1] = (*pucRegBuffer);
				xQueueMbMqtt.RegData.i8data[0] = *(pucRegBuffer + 1);

				*pucRegBuffer++ = (UCHAR) (pusRegHoldingBuf[iRegIndex] >> 8);
				*pucRegBuffer++ = (UCHAR) (pusRegHoldingBuf[iRegIndex] & 0xFF);
				iRegIndex++;
				usNRegs--;
			}
			break;

			/* write current register values with new values from the protocol stack. */
		case MB_REG_READ:
			xQueueMbMqtt.FunC = MB_FUNC_READ_HOLDING_REGISTER;
			while (usNRegs > 0) {
///*            	mqtt_modbus_thread_up(); */

				printf("\r\n  data: %d ", *(pucRegBuffer + 1));
				printf("\r\nRecived data: %d ", (*pucRegBuffer));
				xQueueMbMqtt.RegData.i8data[1] = (*pucRegBuffer);
				xQueueMbMqtt.RegData.i8data[0] = *(pucRegBuffer + 1);
				pusRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
				pusRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
				iRegIndex++;
				usNRegs--;
			}
			break;
		}
#define portDEFAULT_WAIT_TIME 1000
		BaseType_t Err = pdFALSE;
		Err = xQueueSend(xQueueUplinkHandle, &xQueueMbMqtt,
				portDEFAULT_WAIT_TIME);
		if (Err == pdPASS) {
			printf("\r\n Modbus_MQTT Up queued: OK \r\n");

		} else {
			printf("\r\n Modbus_MQTT Up queued: False \r\n");
		}
	} else {
		eStatus = MB_ENOREG;
	}
	return eStatus;
}
