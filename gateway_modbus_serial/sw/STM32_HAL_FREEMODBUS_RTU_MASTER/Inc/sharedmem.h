#ifndef _PERIPHERAL__H_
#define _PERIPHERAL__H_

/* Built-in C library includes ----------------------*/
#include <stdio.h>
#include <stdint.h>
/*Function Address define*/

#define FUNC_READ_TEMP_ADR			(1)
#define FUNC_READ_HUMD_ADR			(2)
#define FUNC_READ_TAIR_ADR			(3)
#define FUNC_READ_LIGH_ADR			(4)

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
uint8_t uiMemGet(uint8_t uiFuncAdr);
void uiMemSet(uint8_t uiFuncAdr, uint8_t uiValue);

#endif
