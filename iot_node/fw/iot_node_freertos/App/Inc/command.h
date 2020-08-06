#ifndef	_COMMAND_H
#define	_COMMAND_H
#include "cmdline.h"
#include <stdint.h>
#include <stdbool.h>
#include "usart.h"
#include "main.h"
extern uint32_t gotCommandFlag;
extern uint8_t commandBuffer[100];
int
Cmd_help(int argc, char *argv[]);
int
setRGBLED(int argc, char *argv[]);
int Cmd_set(int argc, char *argv[]);
int Cmd_get(int argc, char *argv[]);

void command_recv_callback_irq(UART_HandleTypeDef *huart) ;

void vCmdTask(const void *arg);
#endif
