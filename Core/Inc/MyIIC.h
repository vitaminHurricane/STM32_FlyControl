#ifndef __MYIIC_H
#define __MYIIC_H
#include "main.h"

void MYIIC_Init(void);
uint8_t Read_SDA(void);
void MyIIC_Start(void);
void MyIIC_Stop(void);
void MyIIC_SendByte(uint8_t Byte);
uint8_t MyIIC_ReceiveByte(void);
void MyIIC_SendAns(uint8_t level);
uint8_t MyIIC_ReceiveAns(void);

#endif

