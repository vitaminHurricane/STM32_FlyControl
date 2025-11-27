#ifndef __DEBUG_H
#define __DEBUG_H
#include "usart.h"

/*
 *  注意：串口默认波特率为115200 
 *  FireWater协议以\r\n为结尾，数据间以逗号（英文）分开
 */

#define MAX            51
#define VOFA_UART      huart1          //此处填写vofa使用的串口句柄

extern char R_buffer[MAX];
extern char command[MAX];

/*******************************private************************************/
void __Debug_ClearBuffer(uint8_t size);
/**************************************************************************/

/*******************************public*************************************/
void Debug_DataRecieve(void);   
void Debug_Command(char *command);    
void Debug_Devide(uint8_t *array, float p, float r, float y);   
void Debug_R(void);
void Debug_P(void);    
/**************************************************************************/

#endif
