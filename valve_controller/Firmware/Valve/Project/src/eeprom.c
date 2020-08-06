/**
  ******************************************************************************
  * @file    Project/User/EEPROM
  * @author  NTT - BKSTEK Team
  * @version V1.0
  * @date    15-11-2019
  * @brief   EEPROM functions
	*/
#include "eeprom.h"
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

void EEPROM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	I2C_DeInit(I2C2);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//OD
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, ENABLE);
	/* Release I2C1 from reset state */
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C2, DISABLE);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000 ;
	I2C_Init(I2C2, &I2C_InitStructure);
	I2C_Cmd(I2C2,ENABLE);
}
void EEPROM_Write (uint8_t Addr, uint8_t data)
{
	  /*wait until I2C bus is not busy*/
  while(I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY));
    
	/* Send START condition */
  I2C_GenerateSTART(I2C2, ENABLE);
  
    /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT)); 
  
   /* Send MMA address for write */
  I2C_Send7bitAddress(I2C2, MEM_DEVICE_WRITE_ADDR, I2C_Direction_Transmitter);
    
	/* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
   
    /* Send the MMA's Register address to write to */    
  I2C_SendData(I2C2, Addr); 
  
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
   
    /* Send the byte to be written */
  I2C_SendData(I2C2, data);
  
   /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));  
   
   /* Send STOP condition */
  I2C_GenerateSTOP(I2C2, ENABLE);
	  /*wait until I2C bus is not busy*/
  while(I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY));
//			I2C_GenerateSTART(I2C2, ENABLE);

//			while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

//			I2C_Send7bitAddress(I2C2, MEM_DEVICE_WRITE_ADDR, I2C_Direction_Transmitter);

//			while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

//			I2C_SendData(I2C2, Addr);

//			while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

//			I2C_SendData(I2C2, data);

//			while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

//			I2C_GenerateSTOP(I2C2, ENABLE);
	

}
uint8_t EEPROM_Read(uint8_t Addr)
{
			uint8_t RxData = 0;

//			I2C_GenerateSTART(I2C2, ENABLE);

//			while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

//			I2C_Send7bitAddress(I2C2, MEM_DEVICE_WRITE_ADDR, I2C_Direction_Transmitter);

//			while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

//			I2C_SendData(I2C2, Addr);

//			while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

//			I2C_GenerateSTART(I2C2, ENABLE);

//			while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

//			I2C_Send7bitAddress(I2C2, MEM_DEVICE_READ_ADDR, I2C_Direction_Receiver);

//			while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

//			I2C_GenerateSTOP(I2C2, ENABLE);

//			Data = I2C_ReceiveData(I2C2);
//			return Data;
//u8 RxData;
	/*wait until I2C bus is not busy*/
  while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));

	 /* Send START condition */
  I2C_GenerateSTART(I2C2, ENABLE);
 
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

  /* Send MMA address for write */
  I2C_Send7bitAddress(I2C2, MEM_DEVICE_WRITE_ADDR, I2C_Direction_Transmitter);
   
    /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(I2C2, ENABLE);

   /* Send the MMA's Register address to write to */
  I2C_SendData(I2C2, Addr);  

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2C2, ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
  
  /* Send MMA address for read */
  I2C_Send7bitAddress(I2C2, MEM_DEVICE_WRITE_ADDR, I2C_Direction_Receiver);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  I2C_AcknowledgeConfig(I2C2, DISABLE);
	I2C_GenerateSTOP(I2C2, ENABLE);
   /* Test on wait for EV7 */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));
	
	/* Read a byte from the MMA Register */
	RxData = I2C_ReceiveData(I2C2);

	/* Disable Acknowledgement */
    
	/* Send STOP Condition */
    

	/*Retrun Data*/
	return RxData;
}
