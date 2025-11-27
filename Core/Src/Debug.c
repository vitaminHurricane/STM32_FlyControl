#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "Motor.h"
#include "tim.h"
#include "Debug.h"
#include "usart.h"
#include "MPU6050.h"
#include "PID.h"

char R_buffer[MAX] = "\0";
char command[MAX] = "\0";

int fputc(int ch, FILE *f)          //重定向串口打印
{
    HAL_UART_Transmit(&VOFA_UART, (uint8_t *)&ch, 1, 50);
    return ch;
}

void __Debug_ClearBuffer(uint8_t size)
{
    for (uint8_t i = size; i < MAX; i++) {
        R_buffer[i] = '\0';
    }
}

void Debug_DataRecieve(void)
{
    HAL_UARTEx_ReceiveToIdle_DMA(&VOFA_UART, (uint8_t *)R_buffer, MAX);
}

void Debug_Command(char *command)
{
    if (strcmp(command, "Motor1_Test_Start\r\n") == 0) {
        Motor_SetLED(Channel_1, GPIO_PIN_SET);
        Motor_SetSpeed(Channel_1, 100);
        printf("Finished\r\n");
    } else if (strcmp(command, "Motor1_Test_End\r\n") == 0) {
        Motor_SetLED(Channel_1, GPIO_PIN_RESET);
        Motor_SetSpeed(Channel_1, 0);
        printf("Finished\r\n");
    } else if (strcmp(command, "Motor2_Test_Start\r\n") == 0) {
        Motor_SetLED(Channel_2, GPIO_PIN_SET);
        Motor_SetSpeed(Channel_2, 20);
        printf("Finished\r\n");
    } else if (strcmp(command, "Motor2_Test_End\r\n") == 0) {
        Motor_SetLED(Channel_2, GPIO_PIN_RESET);
        Motor_SetSpeed(Channel_2, 0);
        printf("Finished\r\n");
    } else if (strcmp(command, "Motor3_Test_Start\r\n") == 0) {
        Motor_SetLED(Channel_3, GPIO_PIN_SET);
        Motor_SetSpeed(Channel_3, 20);
        printf("Finished\r\n");
    } else if (strcmp(command, "Motor3_Test_End\r\n") == 0) {
        Motor_SetLED(Channel_3, GPIO_PIN_RESET);
        Motor_SetSpeed(Channel_3, 0);     
        printf("Finished\r\n");
    } else if (strcmp(command, "Motor4_Test_Start\r\n") == 0) {
        Motor_SetLED(Channel_4, GPIO_PIN_SET);
        Motor_SetSpeed(Channel_4, 20);
        printf("Finished\r\n");
    } else if (strcmp(command, "Motor4_Test_End\r\n") == 0) {
        Motor_SetLED(Channel_4, GPIO_PIN_RESET);
        Motor_SetSpeed(Channel_4, 0); 
        printf("Finished\r\n");
    } else if (strcmp(command, "Test_End\r\n") == 0) {
        Motor_SetAllLED(GPIO_PIN_RESET);
        Motor_SetSpeed(Channel_1, 0);
        Motor_SetSpeed(Channel_2, 0);
        Motor_SetSpeed(Channel_3, 0);        
        Motor_SetSpeed(Channel_4, 0);        
        printf("Finished\r\n");
    } else if (command[0] != '\0') {
        printf("No this command, Try again\r\n");
    }
    command[0] = '\0';
}

void Debug_Devide(uint8_t *array, float p, float r, float y)
{
    uint8_t p_1, p_2, r_1, r_2, y_1, y_2;
    uint8_t state1, state2, state3;
    if (p > 0) {
        state1 = 1;
    } else {
        state1 = 0;
        p = fabs(p);
    }
    if (r > 0) {
        state2 = 1;
    } else {
        state2 = 0;
        r = fabs(r);
    }
    if (y > 0) {
        state3 = 1;
    } else {
        state3 = 0;
        y = fabs(y);
    }
    p_1 = (int)p, r_1 = (int)r, y_1 = (int)y;
    p_2 = (p - (int)p) * 100, r_2 = (r - (int)r) * 100, y_2 = (y - (int)y) * 100;
    array[1] = p_1, array[2] = p_2;
    array[4] = r_1, array[5] = r_2;
    array[7] = y_1, array[8] = y_2;
    array[0] = state1, array[3] = state2, array[6] = state3;
}

void Debug_P(void)
{
    if (balance_P - pitch > 0.5) {
        Motor_SetLED(Channel_1, GPIO_PIN_SET);
        Motor_SetLED(Channel_4, GPIO_PIN_SET);
        Motor_SetLED(Channel_2, GPIO_PIN_RESET);
        Motor_SetLED(Channel_3, GPIO_PIN_RESET);
    } else if (pitch - balance_P > 0.5) {
        Motor_SetLED(Channel_1, GPIO_PIN_RESET);
        Motor_SetLED(Channel_4, GPIO_PIN_RESET);
        Motor_SetLED(Channel_2, GPIO_PIN_SET);
        Motor_SetLED(Channel_3, GPIO_PIN_SET);
    }
}

void Debug_R(void)
{
    if (roll - balance_R > 0.5) {
        Motor_SetLED(Channel_2, GPIO_PIN_RESET);
        Motor_SetLED(Channel_4, GPIO_PIN_RESET);
        Motor_SetLED(Channel_1, GPIO_PIN_SET);
        Motor_SetLED(Channel_3, GPIO_PIN_SET);
    } else if (balance_R - roll> 0.5) {
        Motor_SetLED(Channel_2, GPIO_PIN_SET);
        Motor_SetLED(Channel_4, GPIO_PIN_SET);
        Motor_SetLED(Channel_1, GPIO_PIN_RESET);
        Motor_SetLED(Channel_3, GPIO_PIN_RESET);
    }
}

