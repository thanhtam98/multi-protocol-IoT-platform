#ifndef	_COMMAND_H
#define	_COMMAND_H
#include "cmdline.h"
#include <stdint.h>
#include <stdbool.h>

extern uint32_t gotCommandFlag;
extern uint8_t commandBuffer[100];
int
Cmd_help(int argc, char *argv[]);
int
setRGBLED(int argc, char *argv[]);
int
setLight(int argc, char *argv[]);
int
controlRelay(int argc, char *argv[]);
int
setAllLight(int argc, char *argv[]);
void UARTIntHandler(void);
#endif
