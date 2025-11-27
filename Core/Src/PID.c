#include "main.h"
#include "MPU6050.h"
#include "PID.h"
#include "Motor.h"
#include "H24.h"
#include <stdio.h>
#include <math.h>

//float P_p = 4.92, I_p = 0.08/*0.0457*/, D_p = 58;
//float P_r = 5.43, I_r = 0.125/*0.0476*/, D_r = 60;
//float P_y = 5, I_y = 0, D_y = 20;

// float P_p = 5.02, I_p = 0.09/*0.0457*/, D_p = 38;
// float P_r = 5.43, I_r = 0.135/*0.0476*/, D_r = 40;
// float P_y = 5, I_y = 0, D_y = 0;

float P_p = 4.92, I_p = 0.11/*0.0457*/, D_p = 58;
float P_r = 5.43, I_r = 0.135/*0.0476*/, D_r = 65;
float P_y = 7.3, I_y = 0, D_y = 40;

float target_pitch, target_roll, target_yaw;
float current_pitch, current_roll, current_yaw;

float balance_P = 2, balance_R = -2.7, balance_Y = 0;

static float err_pre_p = 0, err_now_p = 0, err_int_p = 0;
static float err_pre_r = 0, err_now_r = 0, err_int_r = 0;
static float err_pre_y = 0, err_now_y = 0, err_int_y = 0;

void __PID_LimitErrorInt(float *err_int)
{
    if (*err_int > INT_MAX) {
        *err_int = INT_MAX;
    } else if (*err_int < -INT_MAX) {
        *err_int = -INT_MAX;
    }
}

void __PID_LimitResult(float *result)
{
    if (*result > FIX_MAX) {
        *result = FIX_MAX;
    } else if (*result < -FIX_MAX) {
        *result = -FIX_MAX;
    }
}

void PID_SetPitch(float target)
{
    target_pitch = target;
}

void PID_SetRoll(float target)
{
    target_roll = target;
}

void PID_SetYaw(float target)
{
    target_yaw = target;
}

float PID_Pitch(void)
{
    current_pitch = pitch;
    err_pre_p = err_now_p;
    err_now_p = target_pitch - current_pitch;

    static float temp = 0;
    if (fabs(err_now_p) > 1 && throttle >= 150) {
        err_int_p += err_now_p;
        temp = err_int_p * I_p;
    } 
    __PID_LimitErrorInt(&temp);

    float result = -P_p * err_now_p - temp - D_p * (err_now_p - err_pre_p);
    __PID_LimitResult(&result);
    return result;
}

float PID_Roll(void)
{
    current_roll = roll;
    err_pre_r = err_now_r;
    err_now_r = target_roll - current_roll;

    static float temp = 0;
    if (fabs(err_now_r) > 1 && throttle >= 150) {          //当误差大于正负1°时开启积分修正
        err_int_r += err_now_r;
        temp = err_int_r * I_r;
    } 
    __PID_LimitErrorInt(&temp);

    float result = P_r * err_now_r + temp + D_r * (err_now_r - err_pre_r);
    __PID_LimitResult(&result);
    return result;
}

float PID_Yaw(void)
{
    current_yaw = yaw;
    err_pre_y = err_now_y;
    err_now_y = target_yaw - current_yaw;

    static float temp = 0;
    if (throttle >= 50) {
        err_int_y += err_now_y;
        temp = err_int_y * I_y;
    }
    __PID_LimitErrorInt(&temp);

    float result = P_y * err_now_y + temp + D_y * (err_now_y - err_pre_y);
    __PID_LimitResult(&result);
    return result;
}

void PID_Control(void)
{
    float fix_p = PID_Pitch();
    float fix_r = PID_Roll();
    float fix_y = PID_Yaw();
    float fixed_1, fixed_2, fixed_3, fixed_4;
	
    fixed_1 = throttle + fix_p + fix_r + fix_y;
    fixed_4 = throttle + fix_p - fix_r - fix_y + 90;
    fixed_2 = throttle - fix_p - fix_r + fix_y + 110;
    fixed_3 = throttle - fix_p + fix_r - fix_y + 20;
	//printf("%f\r\n", fixed_1);
	
	Motor_SetSpeed(Channel_1, fixed_1);
    Motor_SetSpeed(Channel_2, fixed_2);
    Motor_SetSpeed(Channel_3, fixed_3);
    Motor_SetSpeed(Channel_4, fixed_4);
}

void PID_DataClear(void)
{
    err_int_p = err_int_r = err_int_y = 0;
    err_pre_p = err_pre_r = err_pre_y = 0;
    err_now_p = err_now_r = err_now_y = 0;
}

void PID_DataUpdate(float p, float r, float y)             //PID姿态更新
{
    PID_SetPitch(p);
    PID_SetRoll(r);
    PID_SetYaw(y);
    //printf("%.2f, %.2f, %.2f\r\n", target_pitch, target_roll, target_yaw);
}

//目标pitch增大，无人机后倾，反之前倾
//目标roll增大，无人机左倾，反之右倾

