#ifndef _PERIPHERAL__H_
#define _PERIPHERAL__H_

/* Built-in C library includes ----------------------*/
#include <stdio.h>
#include <stdint.h>

/* Define */
#define PORT_MAX_DEV	1
#define MB_MAX_PORT		4
#define MB_DEFAUL_DEV	0


enum DEV_STATE {
	DEV_NOT_ACT,
	DEV_ACT,
	DEV_BUSY,
	DEV_ERR
};
enum PORT_STATE
{
	PORT_NOT_ACT,
	PORT_ACT
};
/* Private typedef prototype*/

typedef union
{
	uint32_t idata;
	uint8_t  cdata[4];
}int_to_byte;

typedef struct
{
	int_to_byte username[3];
	int_to_byte password[3];
}host_param_t;
typedef struct
{
	uint8_t uiAdr; /*Address indicate type of device*/
	uint8_t uiDevState; /**/

}port_device_t;
typedef struct
{
	port_device_t portDev[PORT_MAX_DEV];
	uint8_t uiPortState;
}port_param_t;

typedef struct
{
	int_to_byte ip;
	int_to_byte netmask;
	int_to_byte gateway;

}network_param_t;
/*Function Address define*/
#define FLASH_EXIST_ADDRESS   0x08070000
#define FLASH_ID_ADDRESS      0x08070004
#define FLASH_PAGE_ADDRESS    0x08070008
#define FLASH_NET_ADDRESS	  0x08070030
#define FLASH_HOST_ADDRESS    0x08070050
#define FLASH_PORT_ADDRESS    0x08070070 // not use
#define FLASH_IDN_ADDRESS	  0x08070070

#define FLASH_SAVED_DATA      0x01


#define MEM_MAX_SIZE	(40)

/*Public Function prototype---------------------*/

void xFlashLoad(void) ;
void xFlashSave(void) ;

uint8_t* itoa_user(uint32_t val, uint8_t base );

#endif
