/* Built-in C library includes ----------------------*/

#include <stdio.h>
#include <stdint.h>
/* Platform includes --------------------------------*/

#include "main.h"
#include "cmsis_os.h"
#include "sharedmem.h"
#include "param.h"
#include "usart.h"
/* Modbus includes ---------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "mb_m.h"
#include "mbconfig.h"

/* -----------------------Master Defines -------------------------------------*/
#define M_DISCRETE_INPUT_START        0
#define M_DISCRETE_INPUT_NDISCRETES   16
#define M_COIL_START                  0
#define M_COIL_NCOILS                 64
#define M_REG_INPUT_START             0
#define M_REG_INPUT_NREGS             10
#define M_REG_HOLDING_START           0
#define M_REG_HOLDING_NREGS           10
/* master mode: holding register's all address */
#define          M_HD_RESERVE                     0
/* master mode: input register's all address */
#define          M_IN_RESERVE                     0
/* master mode: coil's all address */
#define          M_CO_RESERVE                     0
/* master mode: discrete's all address */
#define          M_DI_RESERVE                     0

extern osTimerId myTimer01Handle;
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 8

static USHORT usRegInputStart = REG_INPUT_START;
static USHORT usRegInputBuf[REG_INPUT_NREGS];
void Callback01(void const * argument);

uint8_t mbIsMaster = 0;

void vTestModbusTask(void const * arg) {
	printf("\r\n ModbusTestTask: Init");
	while (1) {
		osDelay(2000);
		eMBMasterReqReadHoldingRegister(0x01, 0x02, 1, 100);

		printf("\r\nModbusTestTask: Sent data to dest");

	}
}
void vModBusTask(void const * argument) {
	/* ABCDEF */
	mbIsMaster = 0;
//	eMBMasterInit(MB_RTU, 2, 9600, MB_PAR_NONE);
//	eMBMasterEnable();
////	osTimerDef(myTimer01, Callback01);
////	myTimer01Handle = osTimerCreate(osTimer(myTimer01), osTimerPeriodic, NULL);
////	osTimerStart(myTimer01Handle, 1);
//	__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);

//	while (1) {
//		eMBMasterPoll();
//		osDelay(1);
//	}

	eMBErrorCode eStatus;
	if ((PARAM[NODE_HAVE_PARAM_ADR] == FLASH_PARAM)|(PARAM[NODE_HAVE_PARAM_ADR] == EEP_PARAM)) {
//		eStatus = eMBInit(MB_RTU, PARAM[NODE_MB_ID_ADR], 3, 9600, MB_PAR_NONE);
		eStatus = eMBInit(MB_RTU, PARAM[NODE_MB_ID_ADR ], 3, 9600, MB_PAR_NONE);
	} else {
		eStatus = eMBInit(MB_RTU, 1, 3, 9600, MB_PAR_NONE);
	}
	eStatus = eMBEnable();
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);
//  osTimerStart(myTimer01Handle, 1);
	while (1) {
		eMBPoll();
		osDelay(10);
	}
}

eMBErrorCode eMBRegInputCB(UCHAR * pucRegBuffer, USHORT usAddress,
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

eMBErrorCode eMBRegHoldingCB(UCHAR * pucRegBuffer, USHORT usAddress,
		USHORT usNRegs, eMBRegisterMode eMode) {
	eMBErrorCode eStatus = MB_ENOERR;
	USHORT iRegIndex;
	USHORT * pusRegHoldingBuf;
	USHORT REG_HOLDING_START;
	USHORT REG_HOLDING_NREGS;
	USHORT usRegHoldStart;

//   pusRegHoldingBuf = usSRegHoldBuf;
//   REG_HOLDING_START = S_REG_HOLDING_START;
//   REG_HOLDING_NREGS = S_REG_HOLDING_NREGS;
//   usRegHoldStart = usSRegHoldStart;
	DBG("\r\n MB callback function");
	usAddress--;
	if (usAddress < MEM_MAX_SIZE) {
		switch (eMode) {
		case MB_REG_READ:
			while (usNRegs > 0) {
				DBG("\r\n MB callback uiMemGet: adr: %d val: %d", usAddress,
						(uiMemGet(usAddress)));
				*pucRegBuffer++ = (UCHAR) (uiMemGet(usAddress) >> 8);
				*pucRegBuffer++ = (UCHAR) (uiMemGet(usAddress) & 0xFF);
				usAddress++;
				usNRegs--;

			}
			break;
		case MB_REG_WRITE:
			while (usNRegs > 0) {

				uiMemSet(usAddress,
						(uint16_t) ((*(pucRegBuffer)) << 8)
								| *(pucRegBuffer + 1));
				DBG("\r\n MB callback uiMemSet: adr: %d val: %d", usAddress,
						*(pucRegBuffer + 1));
				usAddress++;
				usNRegs--;
			}

			break;
		}
	}

	return MB_ENOERR;
}

eMBErrorCode eMBRegCoilsCB(UCHAR * pucRegBuffer, USHORT usAddress,
		USHORT usNCoils, eMBRegisterMode eMode) {
	return MB_ENOREG;
}

eMBErrorCode eMBRegDiscreteCB(UCHAR * pucRegBuffer, USHORT usAddress,
		USHORT usNDiscrete) {
	return MB_ENOREG;
}
USHORT usMRegInStart = M_REG_INPUT_START;
USHORT usMRegInBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_INPUT_NREGS];
//Master mode:HoldingRegister variables
USHORT usMRegHoldStart = M_REG_HOLDING_START;
USHORT usMRegHoldBuf[MB_MASTER_TOTAL_SLAVE_NUM][M_REG_HOLDING_NREGS];

eMBErrorCode eMBMasterRegHoldingCB(UCHAR * pucRegBuffer, USHORT usAddress,
		USHORT usNRegs, eMBRegisterMode eMode) {

	eMBErrorCode eStatus = MB_ENOERR;
	USHORT iRegIndex;
	USHORT * pusRegHoldingBuf;
	USHORT REG_HOLDING_START;
	USHORT REG_HOLDING_NREGS;
	USHORT usRegHoldStart;

	pusRegHoldingBuf = usMRegHoldBuf[ucMBMasterGetDestAddress() - 1];
	REG_HOLDING_START = M_REG_HOLDING_START;
	REG_HOLDING_NREGS = M_REG_HOLDING_NREGS;
	usRegHoldStart = usMRegHoldStart;
	/* if mode is read, the master will write the received date to buffer. */
	eMode = MB_REG_WRITE;

	/* it already plus one in modbus function method. */
	usAddress--;

	if ((usAddress >= REG_HOLDING_START)
			&& (usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS)) {
		iRegIndex = usAddress - usRegHoldStart;
		switch (eMode) {
		/* read current register values from the protocol stack. */
		case MB_REG_READ:
			while (usNRegs > 0) {
				*pucRegBuffer++ = (UCHAR) (pusRegHoldingBuf[iRegIndex] >> 8);
				*pucRegBuffer++ = (UCHAR) (pusRegHoldingBuf[iRegIndex] & 0xFF);
				iRegIndex++;
				usNRegs--;

			}
			break;
			/* write current register values with new values from the protocol stack. */
		case MB_REG_WRITE:
			while (usNRegs > 0) {
				pusRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
				pusRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
				iRegIndex++;
				usNRegs--;
			}
			break;
		}
	} else {
		eStatus = MB_ENOREG;
	}
	return eStatus;
}
void Callback01(void const * argument)
{
  /* USER CODE BEGIN Callback01 */

      prvvMasterTIMERExpiredISR();
  /* USER CODE END Callback01 */
}
