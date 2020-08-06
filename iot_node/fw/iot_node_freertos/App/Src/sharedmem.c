/* Built-in C library includes ----------------------*/

#include <stdio.h>
#include <stdint.h>

/* Platform includes --------------------------------*/
#include "main.h"
#include <sharedmem.h>
/* Private typedef -----------------------------------*/

//typedef struct {
//	uint8_t uiFunctionAdr;
//	uint8_t uiMem;
//} xMemHandler_t;

typedef  uint16_t xMemHandler_t;

/* Private define -----------------------------------*/
#define MEM_MAX_SIZE	(40)
#define MEM_RST_VAL


static xMemHandler_t xMemSharedHandler[MEM_MAX_SIZE];



/* Start Implementation -----------------------------*/

/**
 * Brief: Get data
 * uiFuncAdr:
 * retVal: Value of Mem at uiFuncAdr
 *
 */
uint16_t uiMemGet(uint8_t uiFuncAdr)
{
	return xMemSharedHandler[uiFuncAdr];

}
/**
 * Brief: Set data
 * uiFuncAdr:
 * retVal: None
 *
 */
void uiMemSet(uint8_t uiFuncAdr, uint16_t uiValue)
{
	xMemSharedHandler[uiFuncAdr] =uiValue ;
}


