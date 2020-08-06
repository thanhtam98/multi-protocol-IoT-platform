/* 
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: mbrtu_m.c,v 1.60 2013/08/20 11:18:10 Armink Add Master Functions $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/

#include "mb.h"
#include "mb_m.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbproto.h"
#include "mbfunc.h"

#include "mbport.h"
#if MB_MASTER_RTU_ENABLED == 1
#include "mbrtu.h"
#endif
#if MB_MASTER_ASCII_ENABLED == 1
#include "mbascii.h"
#endif
#if MB_MASTER_TCP_ENABLED == 1
#include "mbtcp.h"
#endif

#ifndef MB_PORT_HAS_CLOSE
#define MB_PORT_HAS_CLOSE 0
#endif

/* ----------------------- Static variables ---------------------------------*/

static UCHAR ucMBMasterDestAddress[MB_RS485_MAX_PORT];
static BOOL xMBRunInMasterMode[MB_RS485_MAX_PORT] =
		{ FALSE, FALSE, FALSE, FALSE };
static eMBMasterErrorEventType eMBMasterCurErrorType[MB_RS485_MAX_PORT];

static enum {
	STATE_ENABLED, STATE_DISABLED, STATE_NOT_INITIALIZED, STATE_ESTABLISHED,
} eMBState = STATE_NOT_INITIALIZED;

/* Functions pointer which are initialized in eMBInit( ). Depending on the
 * mode (RTU or ASCII) the are set to the correct implementations.
 * Using for Modbus Master,Add by Armink 20130813
 */
static peMBFrameSend peMBMasterFrameSendCur;
static pvMBFrameStart pvMBMasterFrameStartCur;
static pvMBFrameStop pvMBMasterFrameStopCur;
static peMBFrameReceive peMBMasterFrameReceiveCur;
static pvMBFrameClose pvMBMasterFrameCloseCur;

/* Callback functions required by the porting layer. They are called when
 * an external event has happend which includes a timeout or the reception
 * or transmission of a character.
 * Using for Modbus Master,Add by Armink 20130813
 */
BOOL (*pxMBMasterFrameCBByteReceived)(UCHAR ucPort);
BOOL (*pxMBMasterFrameCBTransmitterEmpty)(UCHAR ucPort);
BOOL (*pxMBMasterPortCBTimerExpired)(UCHAR ucPort);

BOOL (*pxMBMasterFrameCBReceiveFSMCur)(UCHAR ucPort);
BOOL (*pxMBMasterFrameCBTransmitFSMCur)(UCHAR ucPort);

/* An array of Modbus functions handlers which associates Modbus function
 * codes with implementing functions.
 */
static xMBFunctionHandler xMasterFuncHandlers[MB_FUNC_HANDLERS_MAX] =
		{
#if MB_FUNC_OTHER_REP_SLAVEID_ENABLED > 0
				//TODO Add Master function define
				{	MB_FUNC_OTHER_REPORT_SLAVEID, eMBFuncReportSlaveID},
#endif
#if MB_FUNC_READ_INPUT_ENABLED > 0
				{	MB_FUNC_READ_INPUT_REGISTER, eMBMasterFuncReadInputRegister},
#endif
#if MB_FUNC_READ_HOLDING_ENABLED > 0
				{ MB_FUNC_READ_HOLDING_REGISTER,
						eMBMasterFuncReadHoldingRegister },
#endif
#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0
				{	MB_FUNC_WRITE_MULTIPLE_REGISTERS, eMBMasterFuncWriteMultipleHoldingRegister},
#endif
#if MB_FUNC_WRITE_HOLDING_ENABLED > 0
				{ MB_FUNC_WRITE_REGISTER, eMBMasterFuncWriteHoldingRegister },
#endif
#if MB_FUNC_READWRITE_HOLDING_ENABLED > 0
		{	MB_FUNC_READWRITE_MULTIPLE_REGISTERS, eMBMasterFuncReadWriteMultipleHoldingRegister},
#endif
#if MB_FUNC_READ_COILS_ENABLED > 0
		{	MB_FUNC_READ_COILS, eMBMasterFuncReadCoils},
#endif
#if MB_FUNC_WRITE_COIL_ENABLED > 0
		{	MB_FUNC_WRITE_SINGLE_COIL, eMBMasterFuncWriteCoil},
#endif
#if MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED > 0
		{	MB_FUNC_WRITE_MULTIPLE_COILS, eMBMasterFuncWriteMultipleCoils},
#endif
#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0
		{	MB_FUNC_READ_DISCRETE_INPUTS, eMBMasterFuncReadDiscreteInputs},
#endif
	};

/* ----------------------- Start implementation -----------------------------*/
eMBErrorCode eMBMasterInit(eMBMode eMode, UCHAR ucPort, ULONG ulBaudRate,
		eMBParity eParity) {
	eMBErrorCode eStatus = MB_ENOERR;

	switch (eMode) {
#if MB_MASTER_RTU_ENABLED > 0
	case MB_RTU:
		pvMBMasterFrameStartCur = eMBMasterRTUStart;
		pvMBMasterFrameStopCur = eMBMasterRTUStop;
		peMBMasterFrameSendCur = eMBMasterRTUSend;
		peMBMasterFrameReceiveCur = eMBMasterRTUReceive;
		pvMBMasterFrameCloseCur = MB_PORT_HAS_CLOSE ? vMBMasterPortClose : NULL;
		pxMBMasterFrameCBByteReceived = xMBMasterRTUReceiveFSM;
		pxMBMasterFrameCBTransmitterEmpty = xMBMasterRTUTransmitFSM;
		pxMBMasterPortCBTimerExpired = xMBMasterRTUTimerExpired;

		eStatus = eMBMasterRTUInit(ucPort, ulBaudRate, eParity);
		break;
#endif
#if MB_MASTER_ASCII_ENABLED > 0
		case MB_ASCII:
		pvMBMasterFrameStartCur = eMBMasterASCIIStart;
		pvMBMasterFrameStopCur = eMBMasterASCIIStop;
		peMBMasterFrameSendCur = eMBMasterASCIISend;
		peMBMasterFrameReceiveCur = eMBMasterASCIIReceive;
		pvMBMasterFrameCloseCur = MB_PORT_HAS_CLOSE ? vMBMasterPortClose : NULL;
		pxMBMasterFrameCBByteReceived = xMBMasterASCIIReceiveFSM;
		pxMBMasterFrameCBTransmitterEmpty = xMBMasterASCIITransmitFSM;
		pxMBMasterPortCBTimerExpired = xMBMasterASCIITimerT1SExpired;

		eStatus = eMBMasterASCIIInit(ucPort, ulBaudRate, eParity);
		break;
#endif
	default:
		eStatus = MB_EINVAL;
		break;
	}

	if (eStatus == MB_ENOERR) {
		if (!xMBMasterPortEventInit()) {
			/* port dependent event module initalization failed. */
			eStatus = MB_EPORTERR;
		} else {
			eMBState = STATE_DISABLED;
		}
		/* initialize the OS resource for modbus master. */
		vMBMasterOsResInit();
	}
	return eStatus;
}

eMBErrorCode eMBMasterClose(UCHAR ucPort) {
	eMBErrorCode eStatus = MB_ENOERR;

	if (eMBState == STATE_DISABLED) {
		if (pvMBMasterFrameCloseCur != NULL) {
			pvMBMasterFrameCloseCur(ucPort);
		}
	} else {
		eStatus = MB_EILLSTATE;
	}
	return eStatus;
}

eMBErrorCode eMBMasterEnable(UCHAR ucPort) {
	eMBErrorCode eStatus = MB_ENOERR;
	pvMBMasterFrameStartCur(ucPort);
	eMBState = STATE_ENABLED;
//    if (eMBState == STATE_DISABLED)
//    {
//        /* Activate the protocol stack. */
//        pvMBMasterFrameStartCur(ucPort);
//        eMBState = STATE_ENABLED;
//    }
//    else
//    {
//        eStatus = MB_EILLSTATE;
//    }
	return eStatus;
}

eMBErrorCode eMBMasterDisable(UCHAR ucPort) {
	eMBErrorCode eStatus;

	if ((eMBState == STATE_ENABLED) || (eMBState == STATE_ESTABLISHED)) {
		pvMBMasterFrameStopCur(ucPort);
		eMBState = STATE_DISABLED;
		eStatus = MB_ENOERR;
	} else if (eMBState == STATE_DISABLED) {
		eStatus = MB_ENOERR;
	} else {
		eStatus = MB_EILLSTATE;
	}
	return eStatus;
}

BOOL eMBMasterIsEstablished(UCHAR ucPort) {
	if (eMBState == STATE_ESTABLISHED) {
		return TRUE;
	} else {
		return FALSE;
	}
}

eMBErrorCode eMBMasterPoll(void) {
	static UCHAR *ucMBFrame[MB_RS485_MAX_PORT];
	static UCHAR ucRcvAddress[MB_RS485_MAX_PORT];
	static UCHAR ucFunctionCode[MB_RS485_MAX_PORT];
	static USHORT usLength[MB_RS485_MAX_PORT];
	static eMBException eException[MB_RS485_MAX_PORT];
	UCHAR ucPort;
	int i, j;
	eMBErrorCode eStatus[MB_RS485_MAX_PORT] = { MB_ENOERR, MB_ENOERR, MB_ENOERR,
			MB_ENOERR };
	eMBMasterEventType eEvent[MB_RS485_MAX_PORT];
	eMBMasterErrorEventType errorType[MB_RS485_MAX_PORT];

	/* Check if the protocol stack is ready. */
	if ((eMBState != STATE_ENABLED) && (eMBState != STATE_ESTABLISHED)) {
		return MB_EILLSTATE;
	}
	for (ucPort = 0; ucPort < MB_RS485_MAX_PORT; ucPort++) {
		/* Check if there is a event available. If not return control to caller.
		 * Otherwise we will handle the event. */
		if (xMBMasterPortEventGet(ucPort, &eEvent[ucPort]) == TRUE) {
			switch (eEvent[ucPort]) {
			case EV_MASTER_READY:
				eMBState = STATE_ESTABLISHED;
				break;

			case EV_MASTER_FRAME_RECEIVED: {
				//printf("\r\n ucPort: %d",ucPort);
				eStatus[ucPort] = peMBMasterFrameReceiveCur(ucPort,
						&ucRcvAddress[ucPort], &ucMBFrame[ucPort],
						&usLength[ucPort]);
				/* Check if the frame is for us. If not ,send an error process event. */
				if ((eStatus[ucPort] == MB_ENOERR)
						&& (ucRcvAddress[ucPort]
								== ucMBMasterGetDestAddress(ucPort))) {
					(void) xMBMasterPortEventPost(ucPort, EV_MASTER_EXECUTE);
				} else {
					vMBMasterSetErrorType(ucPort, EV_ERROR_RECEIVE_DATA);
					(void) xMBMasterPortEventPost(ucPort,
							EV_MASTER_ERROR_PROCESS);
				}
				break;
			}
			case EV_MASTER_EXECUTE: {
				ucFunctionCode[ucPort] = ucMBFrame[ucPort][MB_PDU_FUNC_OFF];
				eException[ucPort] = MB_EX_ILLEGAL_FUNCTION;
				/* If receive frame has exception .The receive function code highest bit is 1.*/
				if (ucFunctionCode[ucPort] >> 7) {
					eException[ucPort] =
							(eMBException) ucMBFrame[ucPort][MB_PDU_DATA_OFF];
				} else {
					for (i = 0; i < MB_FUNC_HANDLERS_MAX; i++) {
						/* No more function handlers registered. Abort. */
						if (xMasterFuncHandlers[i].ucFunctionCode == 0) {
							break;
						} else if (xMasterFuncHandlers[i].ucFunctionCode
								== ucFunctionCode[ucPort]) {
							vMBMasterSetCBRunInMasterMode(ucPort, TRUE);
							/* If master request is broadcast,
							 * the master need execute function for all slave.
							 */
							//TODO: after test protocol stack
							eException[ucPort] =
									xMasterFuncHandlers[i].pxHandler(ucPort,
											ucMBFrame[ucPort],
											&usLength[ucPort]);
							vMBMasterSetCBRunInMasterMode(ucPort, FALSE);
							break;
							if (xMBMasterRequestIsBroadcast(ucPort)) {
								// usLength[ucPort] = usMBMasterGetPDUSndLength(ucPort);
								// for(j = 1; j <= MB_MASTER_TOTAL_SLAVE_NUM; j++){
								// 	vMBMasterSetDestAddress(j);
								// 	eException = xMasterFuncHandlers[i].pxHandler(ucMBFrame, &usLength);
							}
						} else {

						}
						//   vMBMasterSetCBRunInMasterMode(ucPort, FALSE);
						//  break;
					}
				}

				/* If master has exception ,Master will send error process.Otherwise the Master is idle.*/
				if (eException[ucPort] != MB_EX_NONE) {
					vMBMasterSetErrorType(ucPort, EV_ERROR_EXECUTE_FUNCTION);
					(void) xMBMasterPortEventPost(ucPort,
							EV_MASTER_ERROR_PROCESS);
				} else {
					vMBMasterCBRequestScuuess(ucPort);
					vMBMasterRunResRelease(ucPort);
				}
				break;
			}
			case EV_MASTER_FRAME_SENT:
				/* Master is busy now. */
				vMBMasterGetPDUSndBuf(ucPort, &ucMBFrame[ucPort]);
				eStatus[ucPort] = peMBMasterFrameSendCur(ucPort,
						ucMBMasterGetDestAddress(ucPort), ucMBFrame[ucPort],
						usMBMasterGetPDUSndLength(ucPort));
				break;

			case EV_MASTER_ERROR_PROCESS:
				/* Execute specified error process callback function. */
				errorType[ucPort] = eMBMasterGetErrorType(ucPort);
				vMBMasterGetPDUSndBuf(ucPort, &ucMBFrame[ucPort]);
				switch (errorType[ucPort]) {
				case EV_ERROR_RESPOND_TIMEOUT:
					vMBMasterErrorCBRespondTimeout(ucPort,
							ucMBMasterGetDestAddress(ucPort), ucMBFrame[ucPort],
							usMBMasterGetPDUSndLength(ucPort));
					break;
				case EV_ERROR_RECEIVE_DATA:

					vMBMasterErrorCBReceiveData(ucPort,
							ucMBMasterGetDestAddress(ucPort), ucMBFrame[ucPort],
							usMBMasterGetPDUSndLength(ucPort));
					break;
				case EV_ERROR_EXECUTE_FUNCTION:
					vMBMasterErrorCBExecuteFunction(ucPort,
							ucMBMasterGetDestAddress(ucPort), ucMBFrame[ucPort],
							usMBMasterGetPDUSndLength(ucPort));
					break;
				}
				vMBMasterRunResRelease(ucPort);
				break;

			default:
				break;
			}
		}

	}
	return MB_ENOERR;
}

/* Get whether the Modbus Master is run in master mode.*/
BOOL xMBMasterGetCBRunInMasterMode(UCHAR ucPort) {
	return xMBRunInMasterMode[ucPort];
}
/* Set whether the Modbus Master is run in master mode.*/
void vMBMasterSetCBRunInMasterMode(UCHAR ucPort, BOOL IsMasterMode) {
	xMBRunInMasterMode[ucPort] = IsMasterMode;
}
/* Get Modbus Master send destination address. */
UCHAR ucMBMasterGetDestAddress(UCHAR ucPort) {
	return ucMBMasterDestAddress[ucPort];
}
/* Set Modbus Master send destination address. */
void vMBMasterSetDestAddress(UCHAR ucPort, UCHAR Address) {
	ucMBMasterDestAddress[ucPort] = Address;
}
/* Get Modbus Master current error event type. */
eMBMasterErrorEventType eMBMasterGetErrorType(UCHAR ucPort) {
	return eMBMasterCurErrorType[ucPort];
}
/* Set Modbus Master current error event type. */
void vMBMasterSetErrorType(UCHAR ucPort, eMBMasterErrorEventType errorType) {
	eMBMasterCurErrorType[ucPort] = errorType;
}
