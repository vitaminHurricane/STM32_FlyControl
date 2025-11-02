#ifndef __MOTOR_H
#define __MOTRO_H
#include "main.h"

#define MAX_SPEED           900

#define LED_PORT1           GPIOA
#define LED_PORT2           GPIOD
#define LED_PORT3           GPIOB
#define LED_PORT4           GPIOB
#define LED_PIN_1           GPIO_PIN_3
#define LED_PIN_2           GPIO_PIN_3
#define LED_PIN_3           GPIO_PIN_9
#define LED_PIN_4           GPIO_PIN_12

#define MOTOR_TIMER         htim1
#define Channel_1           TIM_CHANNEL_1           //右上角
#define Channel_2           TIM_CHANNEL_2           //左下角
#define Channel_3           TIM_CHANNEL_3           //右下角
#define Channel_4           TIM_CHANNEL_4           //左上角


/****************************private*********************************/
uint16_t __Motor_LimitSpeed(int32_t speed);
void __Motor_SetCounter(uint32_t channel, int16_t cnt);
uint16_t __Motor_GetCounter(uint32_t channel);
/********************************************************************/

/****************************public**********************************/
void Motor_Init(void);
void Motor_SetSpeed(uint32_t channel, int16_t cnt);
void Motor_FixSpeed(uint32_t channel, float fix);
void Motor_SetLED(uint32_t channel, GPIO_PinState state);
void Motor_SetAllLED(GPIO_PinState state);
/********************************************************************/

#endif
