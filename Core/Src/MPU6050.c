#include "main.h"
#include "i2c.h"
#include "MPU6050.h"
#include "Debug.h"
#include <stdio.h>
#include <math.h>

float yaw = 0, pitch = 0, roll = 0;
float yaw_a = 0, pitch_a = 0, roll_a = 0;
float yaw_g = 0, pitch_g = 0, roll_g = 0;

infor MPU6050;

void __MPU6050_Write_Reg(uint16_t Reg_address, uint8_t data)
{
    HAL_I2C_Mem_Write(&MPU6050_IIC, MPU6050_Address << 1, Reg_address, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
}

uint8_t __MPU6050_Read_Reg(uint8_t Reg_address)
{
    uint8_t data;
    HAL_I2C_Mem_Read(&MPU6050_IIC, MPU6050_Address << 1, Reg_address, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
    return data;
}

void __MPU6050_Read_Reg_Continue(uint8_t Reg_address, uint8_t *buffer)
{
	HAL_I2C_Mem_Read(&MPU6050_IIC, MPU6050_Address << 1, Reg_address, I2C_MEMADD_SIZE_8BIT, buffer, 2, 100);
}

void MPU6050_Init(void)
{
	__MPU6050_Write_Reg(MPU6050_PWR_MGMT_1, 0x01);
	__MPU6050_Write_Reg(MPU6050_PWR_MGMT_2, 0x00);
	__MPU6050_Write_Reg(MPU6050_SMPLRT_DIV, 0x09);
	__MPU6050_Write_Reg(MPU6050_CONFIG, 0x06);
	__MPU6050_Write_Reg(MPU6050_GYRO_CONFIG, 0x18);
	__MPU6050_Write_Reg(MPU6050_ACCEL_CONFIG, 0x18);
}

void MPU6050_Call(void)
{
    uint8_t temp;
    temp = __MPU6050_Read_Reg(MPU6050_WHO_AM_I);
    if ((temp & 0x68) == 0x68) {
        printf("MPU6050 OK\r\n");
    } else {
        printf("MPU6050 can't find\r\n");
    }
}

void MPU6050_GetData(infor* temp)
{
    int16_t data_h, data_l;
	int16_t sum_Ax = 0, sum_Ay = 0, sum_Az = 0;
	int16_t sum_Gx = 0, sum_Gy = 0, sum_Gz = 0;

	data_h = __MPU6050_Read_Reg(MPU6050_ACCEL_XOUT_H);
	data_l = __MPU6050_Read_Reg(MPU6050_ACCEL_XOUT_L);
	(*temp).Acc_X = ((data_h << 8) | data_l);
	
	data_h = __MPU6050_Read_Reg(MPU6050_ACCEL_YOUT_H);
	data_l = __MPU6050_Read_Reg(MPU6050_ACCEL_YOUT_L);
	(*temp).Acc_Y = ((data_h << 8) | data_l);
	
	data_h = __MPU6050_Read_Reg(MPU6050_ACCEL_ZOUT_H);
	data_l = __MPU6050_Read_Reg(MPU6050_ACCEL_ZOUT_L);
	(*temp).Acc_Z = ((data_h << 8) | data_l);
	
	data_h = __MPU6050_Read_Reg(MPU6050_GYRO_XOUT_H);
	data_l = __MPU6050_Read_Reg(MPU6050_GYRO_XOUT_L);
	(*temp).Gyro_X = ((data_h << 8) | data_l) + 45;		//抵消零飘
	
	data_h = __MPU6050_Read_Reg(MPU6050_GYRO_YOUT_H);
	data_l = __MPU6050_Read_Reg(MPU6050_GYRO_YOUT_L);
	(*temp).Gyro_Y = ((data_h << 8) | data_l) + 15;		//抵消零飘
	
	data_h = __MPU6050_Read_Reg(MPU6050_GYRO_ZOUT_H);
	data_l = __MPU6050_Read_Reg(MPU6050_GYRO_ZOUT_L);
	(*temp).Gyro_Z = ((data_h << 8) | data_l) - 48;		//抵消零飘

	//printf("%d, %d, %d\r\n", (*temp).Acc_X, (*temp).Acc_Y, (*temp).Acc_Z/*, (*temp).Gyro_X, (*temp).Gyro_Y, (*temp).Gyro_Z*/);
	//printf("%d, %d, %d\r\n", /*(*temp).Acc_X, (*temp).Acc_Y, (*temp).Acc_Z,*/ (*temp).Gyro_X, (*temp).Gyro_Y, (*temp).Gyro_Z);
}

void MPU6050_AttitudeEstimation_G(infor MPU6050)
{
	pitch = pitch - 0.01 * ((float)MPU6050.Gyro_Y / 16.4);
	roll = roll + 0.01 * ((float)MPU6050.Gyro_X / 16.4);
	yaw = yaw + 0.01 * ((float)MPU6050.Gyro_Z / 16.4);

	printf("%f, %f, %f\r\n", pitch, roll, yaw);
}

void MPU6050_AttitudeEstimation_A(infor MPU6050)
{
	yaw = 0;
	pitch = atan2((float)MPU6050.Acc_X, (float)MPU6050.Acc_Z) / Pi * 180.0;
	roll = atan2((float)MPU6050.Acc_Y, (float)MPU6050.Acc_Z) / Pi * 180.0;

	printf("%f, %f, %f\r\n", pitch, roll, yaw);
}

void MPU6050_AttitudeEstimation(infor MPU6050)
{
	static float pre_yaw = 0;
	
	pitch_g = pitch - 0.01 * ((float)MPU6050.Gyro_Y / 16.4);
	roll_g = roll + 0.01 * ((float)MPU6050.Gyro_X / 16.4);
	yaw_g = yaw + 0.01 * ((float)MPU6050.Gyro_Z / 11.4);
	
	pitch_a = atan2((float)MPU6050.Acc_X, (float)MPU6050.Acc_Z) / Pi * 180.0;
	roll_a = atan2((float)MPU6050.Acc_Y, (float)MPU6050.Acc_Z) / Pi * 180.0;
	
	pitch = 0.95238 * pitch_g + 0.04762 * pitch_a;
	roll = 0.95238 * roll_g + 0.04762 * roll_a;
	
	float deta = fabs(yaw_g - pre_yaw);

	if (deta > 0.1) {
		yaw = yaw_g;		
	} else {
		yaw = pre_yaw;
	}
	pre_yaw = yaw;
    printf("%f, %f, %f\r\n", -pitch, roll, yaw);
}

//0.95238, 0.04762
//0.90909, 0.09091

