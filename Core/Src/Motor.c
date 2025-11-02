#include "main.h"
#include "tim.h"
#include "Motor.h"
#include "Delay.h"
#include <stdio.h>

//通道三的电机相同占空比，比其他电机转速都慢

uint16_t __Motor_LimitSpeed(int32_t speed)
{
    if (speed < 0) {
        speed = 0;
    }
    if (speed > MAX_SPEED) {
        speed = MAX_SPEED;
    }
    return speed;
}

void __Motor_SetCounter(uint32_t channel, int16_t cnt)
{
	cnt = __Motor_LimitSpeed(cnt);
	__HAL_TIM_SetCompare(&MOTOR_TIMER, channel, cnt);
}

uint16_t __Motor_GetCounter(uint32_t channel)
{
    uint16_t cnt = __HAL_TIM_GetCompare(&MOTOR_TIMER, channel);
    return cnt;
}

void Motor_Init(void)
{
    HAL_TIM_Base_Start_IT(&MOTOR_TIMER);
    HAL_TIM_PWM_Start(&MOTOR_TIMER, Channel_1);
    Motor_SetLED(Channel_1, GPIO_PIN_SET);
    Delay_ms(200);
    HAL_TIM_PWM_Start(&MOTOR_TIMER, Channel_4);
    Motor_SetLED(Channel_4, GPIO_PIN_SET);
    Delay_ms(200);
    HAL_TIM_PWM_Start(&MOTOR_TIMER, Channel_2);
    Motor_SetLED(Channel_2, GPIO_PIN_SET);
    Delay_ms(200);
    HAL_TIM_PWM_Start(&MOTOR_TIMER, Channel_3);
    Motor_SetLED(Channel_3, GPIO_PIN_SET);
    Delay_ms(500);
    Motor_SetAllLED(GPIO_PIN_RESET);
}

void Motor_SetSpeed(uint32_t channel, int16_t cnt)
{
    __Motor_SetCounter(channel, cnt);
}

void Motor_FixSpeed(uint32_t channel, float fix)
{
    int16_t basic = __Motor_GetCounter(channel);
    int16_t result;
    if (basic + fix > 0) {
        result = basic + fix;
    } else {
        result = 0;
    }
    Motor_SetSpeed(channel, result);
	printf("%d\r\n", result);
}

void Motor_SetLED(uint32_t channel, GPIO_PinState state)
{
    if (channel == Channel_1) {
        HAL_GPIO_WritePin(LED_PORT1, LED_PIN_1, state);
    } else if (channel == Channel_2) {
        HAL_GPIO_WritePin(LED_PORT2, LED_PIN_2, state);      
    } else if (channel == Channel_3) {
        HAL_GPIO_WritePin(LED_PORT3, LED_PIN_3, state);
    } else if (channel == Channel_4) {
        HAL_GPIO_WritePin(LED_PORT4, LED_PIN_4, state);
    }
}

void Motor_SetAllLED(GPIO_PinState state)
{
    HAL_GPIO_WritePin(LED_PORT1, LED_PIN_1, state);
    HAL_GPIO_WritePin(LED_PORT2, LED_PIN_2, state);
    HAL_GPIO_WritePin(LED_PORT3, LED_PIN_3, state);
    HAL_GPIO_WritePin(LED_PORT4, LED_PIN_4, state);
}

