/* Built-in C library includes ----------------------*/

#include <flash.h>
#include <stdio.h>
#include <stdint.h>

/* Platform includes --------------------------------*/
#include "main.h"
/* Private typedef -----------------------------------*/

typedef uint8_t xMemHandler_t;

/* Private define -----------------------------------*/
#define MEM_MAX_SIZE	(40)
#define MEM_RST_VAL
#define MEM_TEMP_OFF    4
//static xMemHandler_t xMemSharedHandler[MEM_MAX_SIZE];
static uint32_t uiAdr = FLASH_PAGE_ADDRESS;

/* Shared Variable ----------------------------------*/

host_param_t hostParam;
port_param_t portParam[MB_MAX_PORT];
network_param_t netParam;
network_param_t mqttHostParam;
uint32_t 		netId;
/* Start Implementation -----------------------------*/

/**
 * Brief: Get data
 * uiFuncAdr:
 * retVal: Value of Mem at uiFuncAdr
 *
 */
void uiFlashGet(uint32_t uiFlashAdr, uint32_t *ptrValue) {
	*ptrValue = *((__IO uint32_t*) uiFlashAdr);

}
/**
 * Brief: Set data
 * uiFuncAdr:
 * retVal: None
 *
 */
void uiFlashSet(uint32_t uiFlashAdr, uint32_t *ptrValue) {

	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, uiFlashAdr, *ptrValue);

}
/**
 * Brief: Load stored data in flash
 *
 * retVal: None
 *
 */
void xFlashLoad(void) {

	/*Check if data have already saved*/
	uiAdr = FLASH_EXIST_ADDRESS;
	uint32_t flashExist = 0;
	uiFlashGet(uiAdr, (uint32_t*) &flashExist);
	if (flashExist == 1) {

		/*ID */
		uiAdr = FLASH_ID_ADDRESS - MEM_TEMP_OFF;
		uiFlashGet(uiAdr += 4, (uint32_t*) &netId);


		uiAdr = FLASH_PAGE_ADDRESS - MEM_TEMP_OFF;
		/*Username and Password*/
		uiFlashGet(uiAdr += 4, (uint32_t*) &hostParam.username[0]);
		uiFlashGet(uiAdr += 4, (uint32_t*) &hostParam.username[1]);
		uiFlashGet(uiAdr += 4, (uint32_t*) &hostParam.username[2]);
		uiFlashGet(uiAdr += 4, (uint32_t*) &hostParam.password[0]);
		uiFlashGet(uiAdr += 4, (uint32_t*) &hostParam.password[1]);
		uiFlashGet(uiAdr += 4, (uint32_t*) &hostParam.password[2]);


		/*Network*/
		uiAdr = FLASH_NET_ADDRESS  - MEM_TEMP_OFF;
		uiFlashGet(uiAdr += 4, (uint32_t*) &netParam.ip);
		uiFlashGet(uiAdr += 4, (uint32_t*) &netParam.netmask);
		uiFlashGet(uiAdr += 4, (uint32_t*) &netParam.gateway);

		/*Network MQTT Server*/
		uiAdr = FLASH_HOST_ADDRESS  - MEM_TEMP_OFF;
		uiFlashGet(uiAdr += 4, (uint32_t*) &mqttHostParam.ip);
		uiFlashGet(uiAdr += 4, (uint32_t*) &mqttHostParam.netmask);
		uiFlashGet(uiAdr += 4, (uint32_t*) &mqttHostParam.gateway);

		/*Port Configuration*/
//			uiAdr = FLASH_PORT_ADDRESS;
//			for (uint8_t uiPort = 0; uiPort < MB_MAX_PORT; uiPort++) {
//				for (uint8_t uiDev = 0; uiDev < PORT_MAX_DEV; uiDev++) {
//					uiFlashGet(uiAdr += 4,
//							(uint32_t*) &portParam[uiPort].portDev[uiDev]);
//				}
//				uiFlashGet(uiAdr += 4, &portParam[uiPort].uiPortState);
//			}

		printf("\r\n Loaded pre-config from Flash \r\n");
	} else {
		printf("\r\n Flash have no data! Load default-config \r\n");
		strcpy(&hostParam.username[0].cdata[0], "admin");
		strcpy(&hostParam.password[0].cdata[0], "admin");
		netParam.ip.cdata[0] = 192;
		netParam.ip.cdata[1] = 168;
		netParam.ip.cdata[2] = 1;
		netParam.ip.cdata[3] = 20;
		netParam.netmask.cdata[0] = 255;
		netParam.netmask.cdata[1] = 255;
		netParam.netmask.cdata[2] = 255;
		netParam.netmask.cdata[3] = 0;
		netParam.gateway.cdata[0] = 192;
		netParam.gateway.cdata[1] = 168;
		netParam.gateway.cdata[2] = 1;
		netParam.gateway.cdata[3] = 1;

		mqttHostParam.ip.cdata[0] = 192;
		mqttHostParam.ip.cdata[1] = 168;
		mqttHostParam.ip.cdata[2] = 1;
		mqttHostParam.ip.cdata[3] = 24;
		mqttHostParam.netmask.cdata[0] = 255;
		mqttHostParam.netmask.cdata[1] = 255;
		mqttHostParam.netmask.cdata[2] = 255;
		mqttHostParam.netmask.cdata[3] = 0;
		mqttHostParam.gateway.cdata[0] = 192;
		mqttHostParam.gateway.cdata[1] = 168;
		mqttHostParam.gateway.cdata[2] = 1;
		mqttHostParam.gateway.cdata[3] = 1;
		netId = 01;
		xFlashSave();

	}

}
/**
 * Brief: Save data to flash
 *
 * retVal: None
 *
 */
void xFlashSave(void) {

	HAL_FLASH_Unlock();
	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
	EraseInitStruct.Sector = FLASH_SECTOR_7;
	EraseInitStruct.NbSectors = 1;
	EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;

	uint32_t PageError = 0;
	HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);

	/*Node ID*/
	uiAdr = FLASH_ID_ADDRESS - MEM_TEMP_OFF;
	uiFlashSet(uiAdr += 4, (uint32_t*) &netId);
	/*Username and Password*/
	uiAdr = FLASH_PAGE_ADDRESS  - MEM_TEMP_OFF;
	uiFlashSet(uiAdr += 4, (uint32_t*) &hostParam.username[0]);
	uiFlashSet(uiAdr += 4, (uint32_t*) &hostParam.username[1]);
	uiFlashSet(uiAdr += 4, (uint32_t*) &hostParam.username[2]);
	uiFlashSet(uiAdr += 4, (uint32_t*) &hostParam.password[0]);
	uiFlashSet(uiAdr += 4, (uint32_t*) &hostParam.password[1]);
	uiFlashSet(uiAdr += 4, (uint32_t*) &hostParam.password[2]);

	/*Network*/
	uiAdr = FLASH_NET_ADDRESS  - MEM_TEMP_OFF;
	uiFlashSet(uiAdr += 4, (uint32_t*) &netParam.ip);
	uiFlashSet(uiAdr += 4, (uint32_t*) &netParam.netmask);
	uiFlashSet(uiAdr += 4, (uint32_t*) &netParam.gateway);

	/*MQTT Network*/
	uiAdr = FLASH_HOST_ADDRESS  - MEM_TEMP_OFF;
	uiFlashSet(uiAdr += 4, (uint32_t*) &mqttHostParam.ip);
	uiFlashSet(uiAdr += 4, (uint32_t*) &mqttHostParam.netmask);
	uiFlashSet(uiAdr += 4, (uint32_t*) &mqttHostParam.gateway);


	/*Port Configuration*/
	uiAdr = FLASH_PORT_ADDRESS  - MEM_TEMP_OFF;

//	for (uint8_t uiPort = 0; uiPort < MB_MAX_PORT; uiPort++) {
//		for (uint8_t uiDev = 0; uiDev < PORT_MAX_DEV; uiDev++) {
//			uiFlashSet(uiAdr += 4,
//					(uint32_t*) &portParam[uiPort].portDev[uiDev]);
//		}
//		uiFlashSet(uiAdr += 4, &portParam[uiPort].uiPortState);
//
//	}

	uiAdr = FLASH_EXIST_ADDRESS;
	uint32_t uiFashExist = 1;
	uiFlashSet(uiAdr, &uiFashExist);
	HAL_FLASH_Lock();
	printf("\r\n Flash saved successful  \r\n");
}

/*Function*/
/**
 * @brief
 * @param
 * @retval None
 */
uint8_t* itoa_user(uint32_t val, uint8_t base) {
	static uint8_t buf[32] = { 0 };  // 32 bits
	int i = 30;
	if (val == 0)
		buf[i--] = '0';
	for (; val && i; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];

	return &buf[i + 1];
}
