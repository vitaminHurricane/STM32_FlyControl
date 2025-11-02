#include "main.h"

/*
*
*
*
*
*/

void MYIIC_Init(void)
{
	MX_GPIO_Init();
}

void Write_SCL(GPIO_PinState level)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, level);
}

void Write_SDA(GPIO_PinState level)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, level);       //set SDA level (during the low level of SCL)
}

uint8_t Read_SDA(void)
{
	uint8_t value;
	value = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14);
	return value;
}

void MyIIC_Start(void)
{
	Write_SDA(1);                 //set both high level first
	Write_SCL(1);
	Write_SDA(0);			      //pull SDA level down firstly
	Write_SCL(0);				  //pull SCL level down secondly
}

void MyIIC_Stop(void)
{
	Write_SDA(0);                //confirm SDA high level before stop 
	Write_SCL(1);				 //push SCL level up firstly
	Write_SDA(1);				 //push SDA level up secondly
}

void MyIIC_SendByte(uint8_t Byte)    //"Byte -> the data you want to send"
{
	for (uint8_t i = 0; i < 8; i++) {
		Write_SDA(Byte & (0x80 >> i));        //Set SDA level during the low level of SCL(must)
		Write_SCL(1);						  //push SCL level up so that Slave could read the SDA data
		Write_SCL(0);						  //pull SCL level down to permit Host to set SDA level again
	}	
}

uint8_t MyIIC_ReceiveByte(void)
{
	uint8_t byte = 0x00;		      		//temporary container to sort Byte
	Write_SDA(1);							//Host relice SDA so that Slave could write SDA	
	for (uint8_t i = 0; i < 8; i++) {		//repeat 8 times to recive a whole Byte
		Write_SCL(1);						//Host push SCL level up to read data from Slave
		if (Read_SDA() == 1) {
			byte |= (0x80 >> i);			
		}	
		Write_SCL(0);						//Host pull SCL level down so that Slave could change SDA data
	}	
	return byte;
}

void MyIIC_SendAns(uint8_t level)
{
	Write_SDA(level);	
	Write_SCL(1);
	Write_SCL(0);
}

uint8_t MyIIC_ReceiveAns(void)
{
	uint8_t level;
	Write_SDA(1);
	Write_SCL(1);
	level = Read_SDA();
	Write_SCL(0);
	return level;
}


