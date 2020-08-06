/*
 * 	INFORMATION ABOUT THIS SOURCE - I COPIED AND MODIFIED  ":)
 * | -------------------------------- R2T Team Libraries -------------------------
 * | @Created On Sep,11,2015
 * | @File Name : at24_hal_i2c
 * | @Brief : STM32 HAL Driver for AT24 eeprom series
 * | @Copyright :
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * |
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * | @Author :  Sina Darvishi
 * | @Website : R2T.IR
 * |
 **/
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include  "epprom.h"
#include  "i2c.h"
#include  "main.h"


#include "i2c.h"

#if (_EEPROM_USE_FREERTOS == 1)
#include "cmsis_os.h"
#define ee24_delay(x)   osDelay(x)
#else
#define ee24_delay(x)   HAL_Delay(x)
#endif

#if (_EEPROM_SIZE_KBIT == 1) || (_EEPROM_SIZE_KBIT == 2)
#define _EEPROM_PSIZE     8
#elif (_EEPROM_SIZE_KBIT == 4) || (_EEPROM_SIZE_KBIT == 8) || (_EEPROM_SIZE_KBIT == 16)
#define _EEPROM_PSIZE     16
#else
#define _EEPROM_PSIZE     32
#endif


/* Private variables ----------------*/
uint8_t ee24_lock = 0;


/* Start implementation ------------------*/
uint8_t at24_isConnected(void)
{
  #if (_EEPROM_USE_WP_PIN==1)
  HAL_GPIO_WritePin(_EEPROM_WP_GPIO,_EEPROM_WP_PIN,GPIO_PIN_SET);
  #endif
  if (HAL_I2C_IsDeviceReady(&AT24_I2C, AT24_ADR, 2, 100)==HAL_OK)
    return 1;
  else
    return 0;
}

uint8_t at24_write_bytes(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData,
		uint16_t TxBufferSize) {
#define TIMEOUT  100
	  if (ee24_lock == 1)
	    return 0;
	  ee24_lock = 1;
	  uint16_t w;
	  uint32_t startTime = HAL_GetTick();
		#if	(_EEPROM_USE_WP_PIN==1)
		HAL_GPIO_WritePin(_EEPROM_WP_GPIO, _EEPROM_WP_PIN,GPIO_PIN_RESET);
		#endif
	  while (1)
	  {
	    w = _EEPROM_PSIZE - (MemAddress  % _EEPROM_PSIZE);
	    if (w > TxBufferSize)
	      w = TxBufferSize;
	    #if ((_EEPROM_SIZE_KBIT==1) || (_EEPROM_SIZE_KBIT==2))
	    if (HAL_I2C_Mem_Write(&AT24_I2C, DevAddress, MemAddress, I2C_MEMADD_SIZE_8BIT, data, w, 100) == HAL_OK)
	    #elif (_EEPROM_SIZE_KBIT==4)
	    if (HAL_I2C_Mem_Write(&AT24_I2C, DevAddress | ((MemAddress & 0x0100 >> 7)), (MemAddress & 0xff), I2C_MEMADD_SIZE_8BIT, pData, w, 100) == HAL_OK)
	    #elif (_EEPROM_SIZE_KBIT==8)
	    if (HAL_I2C_Mem_Write(&AT24_I2C, DevAddress | ((MemAddress & 0x0300 >> 7)), (MemAddress & 0xff), I2C_MEMADD_SIZE_8BIT, pData, w, 100) == HAL_OK)
	    #elif (_EEPROM_SIZE_KBIT==16)
	    if (HAL_I2C_Mem_Write(&AT24_I2C, DevAddress | ((MemAddress & 0x0700 >> 7)), (MemAddress & 0xff), I2C_MEMADD_SIZE_8BIT, pData, w, 100) == HAL_OK)
	    #else
	    if (HAL_I2C_Mem_Write(&AT24_I2C, DevAddress, MemAddress, I2C_MEMADD_SIZE_16BIT, pData, w, 100) == HAL_OK)
	    #endif
	    {
	      ee24_delay(10);
	      TxBufferSize -= w;
	      pData += w;
	      MemAddress += w;
	      if (TxBufferSize == 0)
	      {
	        #if (_EEPROM_USE_WP_PIN==1)
	        HAL_GPIO_WritePin(_EEPROM_WP_GPIO, _EEPROM_WP_PIN, GPIO_PIN_SET);
	        #endif
	        ee24_lock = 0;
	        return 1;
	      }
	      if (HAL_GetTick() - startTime >= TIMEOUT)
	      {
	        ee24_lock = 0;
	        return 0;
	      }
	    }
	    else
	    {
	      #if (_EEPROM_USE_WP_PIN==1)
	      HAL_GPIO_WritePin(_EEPROM_WP_GPIO, _EEPROM_WP_PIN, GPIO_PIN_SET);
	      #endif
	      ee24_lock = 0;
	      return 0;
	    }
	  }

}
uint8_t at24_read_bytes(uint16_t DevAddress, uint16_t MemAddress, uint8_t *pData,
		uint16_t RxBufferSize)
{
	 if (ee24_lock == 1)
	    return 0;
	  ee24_lock = 1;
	  #if (_EEPROM_USE_WP_PIN==1)
	  HAL_GPIO_WritePin(_EEPROM_WP_GPIO, _EEPROM_WP_PIN, GPIO_PIN_SET);
	  #endif
	  #if ((_EEPROM_SIZE_KBIT==1) || (_EEPROM_SIZE_KBIT==2))
	  if (HAL_I2C_Mem_Read(&AT24_I2C, DevAddress, MemAddress, I2C_MEMADD_SIZE_8BIT, data, len, 100) == HAL_OK)
	  #elif (_EEPROM_SIZE_KBIT == 4)
	  if (HAL_I2C_Mem_Read(&AT24_I2C, DevAddress | ((MemAddress & 0x0100 >> 7)), (MemAddress & 0xff), I2C_MEMADD_SIZE_8BIT, pData, RxBufferSize, 1000) == HAL_OK)
	  #elif (_EEPROM_SIZE_KBIT == 8)
	  if (HAL_I2C_Mem_Read(&AT24_I2C, DevAddress | ((MemAddress & 0x0300 >> 7)), (MemAddress & 0xff), I2C_MEMADD_SIZE_8BIT, pData, RxBufferSize, 1000) == HAL_OK)
	  #elif (_EEPROM_SIZE_KBIT==16)
	  if (HAL_I2C_Mem_Read(&AT24_I2C, DevAddress | ((MemAddress & 0x0700>> 7)), (MemAddress & 0xff), I2C_MEMADD_SIZE_8BIT, pData, RxBufferSize, 1000) == HAL_OK)
	  #else
	  if (HAL_I2C_Mem_Read(&AT24_I2C, _EEPROM_ADDRESS, MemAddress, I2C_MEMADD_SIZE_16BIT, pData, len, timeout) == HAL_OK)
	  #endif
	  {
	    ee24_lock = 0;
	    return 1;
	  }
	  else
	  {
	    ee24_lock = 0;
	    return 0;
	  }
}





/**
 * @brief               : This function handles Writing Array of Bytes on the specific MemAddress .
 * 					   This program have this feature that don't force you to use absolute 16 bytes
 * 					   you can use more than 16 bytes buffer.
 * @param  hi2c         : Pointer to a I2C_HandleTypeDef structure that contains
 *                        the configuration information for the specified I2C.
 * @param	DevMemAddress   : specifies the slave MemAddress to be programmed(EEPROM MemAddress).
 * @param	MemMemAddress   : Internal memory MemAddress (WHERE YOU WANNA WRITE TO)
 * @param	pData	     : Pointer to data buffer
 * @param  TxBufferSize : Amount of data you wanna Write
 * @retval
 */
/**
 * Note that function not work if the number of bytes > 16. Be careful!
 * */
//uint8_t at24_write_bytes(uint16_t DevMemAddress, uint16_t MemMemAddress, uint8_t *pData,
//		uint16_t TxBufferSize) {
//
//	uint8_t TimeOut = 0;
//	if (TxBufferSize > 16) return ERR_MAX_SIZE;
//	while (HAL_I2C_Mem_Write(&at24_i2c, (uint16_t) DevMemAddress, (uint16_t) MemMemAddress,
//	I2C_MEMADD_SIZE_8BIT, (uint8_t*) pData, (uint16_t) TxBufferSize, 1000)
//			!= HAL_OK && TimeOut < 3) {
//		DBG("Write Error \r\n");
//		HAL_Delay(1000);
//		TimeOut++;
//	}
//	if (TimeOut >2) return ERR_TIMEOUT;
//	return OK;
//
//}

/**
 * Note that function not work if the number of bytes > 16. Be careful!
 * */
//uint8_t at24_read_bytes(uint16_t DevMemAddress, uint16_t MemMemAddress, uint8_t *pData,
//		uint16_t RxBufferSize) {
//	uint8_t TimeOut = 0;
//	if (RxBufferSize > 16) return ERR_MAX_SIZE;
//	while (HAL_I2C_Mem_Read(&at24_i2c, (uint16_t) DevMemAddress, (uint16_t) MemMemAddress,
//	I2C_MEMADD_SIZE_8BIT, (uint8_t*) pData, (uint16_t) RxBufferSize, 1000)
//			!= HAL_OK && TimeOut < 3) {
//		DBG("Read Error \r\n");
//		HAL_Delay(1000);
//		TimeOut++;
//	}
//	if (TimeOut > 2) return ERR_TIMEOUT;
//	return OK;
//}

