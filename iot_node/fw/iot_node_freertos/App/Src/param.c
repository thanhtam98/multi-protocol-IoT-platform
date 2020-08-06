#include "param.h"
#include "epprom.h"

uint8_t PARAM[EEP_MAX_SIZE];

uParam_t U_PARAM[PARAM_MAX_SIZE] = {

		{ NODE_HAVE_PARAM_ADR, NODE_MB_DEFAULT_LEN },
		{ NODE_ID_ADR,NODE_MB_DEFAULT_LEN }, { NODE_MB_ID_ADR, NODE_MB_DEFAULT_LEN }, {
		NODE_MB_SERCFG_ADR, NODE_MB_DEFAULT_LEN }, { NODE_MB_MODE_ADR,
		NODE_MB_DEFAULT_LEN }, { NODE_MB_BAUD_ADR, NODE_MB_DEFAULT_LEN }, {
		NODE_MB_PARTITY_ADR,
		NODE_MB_DEFAULT_LEN },

		{ NODE_LRWAN_DEVEUI_ADR, NODE_LRWAN_DEVEUI_LEN }, {
		NODE_LRWAN_APPKEY_ADR, NODE_LRWAN_APPKEY_LEN }, {
		NODE_LRWAN_APPEUI_ADR, NODE_LRWAN_APPKEY_LEN },

		{NODE_IO_PORT_1_ADR, NODE_IO_PORT_1_LEN},
		{NODE_IO_PORT_2_ADR, NODE_IO_PORT_2_LEN},
		{NODE_IO_PORT_3_ADR, NODE_IO_PORT_3_LEN},
		{NODE_IO_PORT_4_ADR, NODE_IO_PORT_4_LEN},
		{NODE_IO_PORT_5_ADR, NODE_IO_PORT_5_LEN},


		{ 0, 0 }

};

/*
 * Brief: Load stored data from EEPROM to RAM
 *
 *
 * */

void v_epr_load(uint16_t usAdr) {
	uint8_t err;
	if (usAdr == PARAM_LOAD_ALL) {
		err = at24_read_bytes(AT24_ADR, 0x00,PARAM,EEP_MAX_SIZE);
		/*Load all value in Eeprom to RAM*/
//		for (uint8_t funcID = 0; funcID < PARAM_MAX_SIZE; funcID++) {
//			err = at24_read_bytes(AT24_ADR, U_PARAM[funcID].uAdr,
//					&PARAM[U_PARAM[funcID].uAdr], U_PARAM[funcID].uLen);
			if (err == 0) {
				printf("Read TimeOut \r\n");

			}
//		}
	} else {
		/*Load the specific adr requested*/
		err = at24_read_bytes(AT24_ADR, usAdr, &PARAM[usAdr],
				0x01);
		if (err == 0) {
			printf("Read TimeOut \r\n");

		}
	}

}
/*
 * Brief: Store data to Eeprom
 *
 *
 * */

void v_epr_save(uint16_t usAdr) {
	uint8_t err;
	if (usAdr == PARAM_LOAD_ALL) {
		err = at24_write_bytes(AT24_ADR,0x00, PARAM,
				EEP_MAX_SIZE);
		if (err = 0) {
			printf("Write TimeOut \r\n");

		}
		/*Store all value in Eeprom to RAM*/
//		for (uint8_t funcID = 0; funcID < PARAM_MAX_SIZE; funcID++) {
//			err = at24_write_bytes(AT24_ADR, U_PARAM[funcID].uAdr,
//					&PARAM[U_PARAM[funcID].uAdr], U_PARAM[funcID].uLen);
//			if (err == ERR_TIMEOUT) {
//				printf("Write TimeOut \r\n");
//				break;
//			}
//		}
//		at24_write_bytes(AT24_ADR, NODE_HAVE_PARAM_ADR,
//				&PARAM[NODE_HAVE_PARAM_ADR], 8);
//
//		at24_write_bytes(AT24_ADR, NODE_LRWAN_DEVEUI_ADR,
//				&PARAM[NODE_LRWAN_DEVEUI_ADR], 8);
//		at24_write_bytes(AT24_ADR, NODE_LRWAN_APPKEY_ADR,
//				&PARAM[NODE_LRWAN_APPKEY_ADR], 16);
//		at24_write_bytes(AT24_ADR, NODE_LRWAN_APPEUI_ADR,
//				&PARAM[NODE_LRWAN_APPEUI_ADR], 16);

	} else {

		/*Store the specific adr requested*/
		err = at24_write_bytes(AT24_ADR,usAdr, &PARAM[usAdr],
				0x01);
		if (err = 0) {
			printf("Write TimeOut \r\n");

		}
	}

}
/**/

uint8_t u_mem_get(uint16_t usAdr) {

	if (usAdr < PARAM_MAX_SIZE) {
//		v_epr_load(usAdr);
		return PARAM[usAdr];
	} else
		return 0;
//	return
}

void u_mem_set(uint16_t usAdr, uint8_t uVal) {
	if (usAdr < EEP_MAX_SIZE) {

		PARAM[usAdr] = uVal;
//		v_epr_save(usAdr);
	}

}

