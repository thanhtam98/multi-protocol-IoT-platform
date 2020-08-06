
#ifndef _EEP_ROM__
#define _EEP_ROM__
#include <stdint.h>
/*Private define --------------------------------*/
#define I2C_SPPED 5000

#define EEPROM_PORT GPIOB
#define I2C_SLAVE_ADDRESS7      0xA0
#define MEM_DEVICE_WRITE_ADDR   0xA0
#define MEM_DEVICE_READ_ADDR    0xA1
/*Private function prototype --------------------------------*/
void EEPROM_Write (uint8_t Addr, uint8_t data);
uint8_t EEPROM_Read(uint8_t Addr);
void EEPROM_Init(void);
#endif