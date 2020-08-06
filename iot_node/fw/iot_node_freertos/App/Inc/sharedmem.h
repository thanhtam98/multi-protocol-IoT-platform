#ifndef _PERIPHERAL__H_
#define _PERIPHERAL__H_

/* Built-in C library includes ----------------------*/
#include <stdio.h>
#include <stdint.h>
/*Function Address define*/
/* IO Mapping
 *   [SW1][SW2][PULS][ADC1][ADC2][OneWire]
 *
 * */

#define PORT_IO_SW_1				(1)
#define PORT_IO_SW_2 				(2)
#define PORT_PULSE					(3)
#define PORT_ADC_1					(4)
#define PORT_ADC_2					(5)
#define PORT_ONE_WIRE				(6)
#define PORT_ONE_WIRE_				(7)

#define FUNC_READ_TEMP_ADR			(1)
#define FUNC_READ_HUMD_ADR			(2)
#define FUNC_READ_TAIR_ADR			(4)
#define FUNC_READ_LIGH_ADR			(3)

#define FUNC_READ_USER1_ADR			(11)
#define FUNC_READ_USER2_ADR			(12)
#define FUNC_READ_USER3_ADR			(13)

#define FUNC_WRITE_SPEK_ADR			(21)
#define FUNC_WRITE_LED0_ADR			(22)
#define FUNC_WRITE_LED1_ADR			(23)
#define FUNC_WRITE_MOTO_ARD			(24)

#define FUNC_WRITE_USER1_ADR 		(31)


#define MEM_MAX_SIZE	(40)

/*Public Function prototype---------------------*/
uint16_t uiMemGet(uint8_t uiFuncAdr);
void uiMemSet(uint8_t uiFuncAdr, uint16_t uiValue);

#endif
