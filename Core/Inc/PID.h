#ifndef __PID_H
#define __PID_H

#define INT_MAX         50
#define FIX_MAX         200

extern float target_roll, target_pitch, target_yaw;
extern float balance_P, balance_R, balance_Y;

/*********************************private*************************************/
void __PID_LimitErrorInt(float *err_int);
void __PID_LimitResult(float *result);
/*****************************************************************************/

/*********************************public**************************************/
void PID_SetPitch(float target);
void PID_SetRoll(float target);
void PID_SetYaw(float target);
float PID_Pitch(void);
float PID_Roll(void);
float PID_Yaw(void);
void PID_Control(void);
void PID_DataClear(void);
void PID_DataUpdate(float p, float r, float y);
/*****************************************************************************/

#endif
