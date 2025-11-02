#ifndef __MPU6050_H
#define __MPU6050_H
#include "main.h"
#include "i2c.h"

#define MPU6050_IIC             hi2c2

#define MPU6050_Address         0x68
#define	MPU6050_SMPLRT_DIV		0x19
#define	MPU6050_CONFIG			0x1A
#define	MPU6050_GYRO_CONFIG		0x1B
#define	MPU6050_ACCEL_CONFIG	0x1C

#define	MPU6050_ACCEL_XOUT_H	0x3B
#define	MPU6050_ACCEL_XOUT_L	0x3C
#define	MPU6050_ACCEL_YOUT_H	0x3D
#define	MPU6050_ACCEL_YOUT_L	0x3E
#define	MPU6050_ACCEL_ZOUT_H	0x3F
#define	MPU6050_ACCEL_ZOUT_L	0x40
#define	MPU6050_TEMP_OUT_H		0x41
#define	MPU6050_TEMP_OUT_L		0x42
#define	MPU6050_GYRO_XOUT_H		0x43
#define	MPU6050_GYRO_XOUT_L		0x44
#define	MPU6050_GYRO_YOUT_H		0x45
#define	MPU6050_GYRO_YOUT_L		0x46
#define	MPU6050_GYRO_ZOUT_H		0x47
#define	MPU6050_GYRO_ZOUT_L		0x48

#define	MPU6050_PWR_MGMT_1		0x6B
#define	MPU6050_PWR_MGMT_2		0x6C
#define	MPU6050_WHO_AM_I		0x75

#define Pi                      3.1415926

typedef struct infor{
	int16_t Acc_X, Acc_Y, Acc_Z;
	int16_t Gyro_X, Gyro_Y, Gyro_Z;
}infor;

extern float yaw, pitch, roll;
extern infor MPU6050;
extern uint8_t Read_buf[2];

/**************************************private****************************************/
void __MPU6050_Write_Reg(uint16_t Reg_address, uint8_t data);
uint8_t __MPU6050_Read_Reg(uint8_t Reg_address);
void __MPU6050_Read_Reg_Continue(uint8_t Reg_address, uint8_t *buffer);
/*************************************************************************************/


/**************************************public*****************************************/
void MPU6050_Init(void);
void MPU6050_Call(void);
void MPU6050_GetData(infor* temp);
void MPU6050_AttitudeEstimation_G(infor MPU6050);
void MPU6050_AttitudeEstimation_A(infor MPU6050);
void MPU6050_AttitudeEstimation(infor MPU6050);
/*************************************************************************************/


#endif
