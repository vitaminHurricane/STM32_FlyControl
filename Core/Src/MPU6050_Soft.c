// #include "main.h"
// #include "MyIIC.h"
// #include "MPU6050_Soft.h"
// #include <math.h>
// #include <stdio.h>

// float yaw = 0, pitch = 0, roll = 0;
// float yaw_a = 0, pitch_a = 0, roll_a = 0;
// float yaw_g = 0, pitch_g = 0, roll_g = 0;

// infor MPU6050;

// void MPU6050_WriteReg(uint8_t regAddress, uint8_t data)       //Host write one Byte to Slave Reggester
// {
// 	MyIIC_Start();
// 	MyIIC_SendByte(MPU6050_add);        //Set the Slave address
// 	MyIIC_ReceiveAns();					//Receive the answer, here do nothing to check the answer
// 	MyIIC_SendByte(regAddress);         //Set the Slave reggester address you want to write in
// 	MyIIC_ReceiveAns();					//Receive the answer, here do nothing to check the answer
// 	MyIIC_SendByte(data);               //Set the data you want to write
// 	MyIIC_ReceiveAns();					//Receive the answer, here do nothing to check the answer
// 	MyIIC_Stop();						//Stop writing
// }

// uint8_t MPU6050_ReadReg(uint8_t regAddress)			//Host Read one Byte from Slave Reggester
// {
// 	uint8_t data;
	
// 	MyIIC_Start();							//  Send data first because if use MyIIC_SendByte(MPU6050_add | 0x01) firstly,
// 	MyIIC_SendByte(MPU6050_add);			//you can't confirm Slave reggester address anymore. Thus we use write mode to
// 	MyIIC_ReceiveAns();						//confirm Slave and Slave reggester address, change read mode later.
// 	MyIIC_SendByte(regAddress);	
// 	MyIIC_ReceiveAns();						//Set the Slave and Slave reggester address like MPU6050_WriteReg()
	
// 	MyIIC_Start();							//Reconfirm the address mode, need reload IIC
// 	MyIIC_SendByte(MPU6050_add | 0x01);		//Change the mode to read data from confirmed reggester address
// 	MyIIC_ReceiveAns();
// 	data = MyIIC_ReceiveByte();
// 	MyIIC_SendAns(1);               	 	//Host send an answer to tell Slave whether to send data again
// 	MyIIC_Stop();							//Send 1 to stop, send 0 to continue
	
// 	return data;
// }

// void MPU6050_Init(void)
// {
// 	MYIIC_Init();

// 	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
// 	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
// 	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
// 	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
// 	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
// 	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
// }

// void MPU6050_GetData(infor* temp)          //��ȡ��̬����
// {
// 	int16_t data_h, data_l;
	
// 	data_h = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
// 	data_l = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
// 	(*temp).Acc_X = (data_h << 8) | data_l;
	
// 	data_h = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
// 	data_l = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
// 	(*temp).Acc_Y = (data_h << 8) | data_l;
	
// 	data_h = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
// 	data_l = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
// 	(*temp).Acc_Z = (data_h << 8) | data_l;
	
// 	data_h = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
// 	data_l = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
// 	(*temp).Gyro_X = ((data_h << 8) | data_l) /*- 41*/;
	
// 	data_h = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
// 	data_l = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
// 	(*temp).Gyro_Y = ((data_h << 8) | data_l) /*+ 397*/;
	
// 	data_h = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
// 	data_l = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
// 	(*temp).Gyro_Z = ((data_h << 8) | data_l) /*+ 112*/;	

// 	//printf("%d, %d, %d\r\n", (*temp).Acc_X, (*temp).Acc_Y, (*temp).Acc_Z);
// 	printf("%d, %d, %d\r\n", (*temp).Gyro_X, (*temp).Gyro_Y, (*temp).Gyro_Z);
// }

// void MPU6050_AttitudeEstimation_G(void)
// {
// 	pitch = pitch - 0.01 * ((float)MPU6050.Gyro_Y / 3.45);
// 	roll = roll + 0.01 * ((float)MPU6050.Gyro_X / 3.45);
// 	yaw = yaw + 0.01 * ((float)MPU6050.Gyro_Z / 3.45);
// }

// void MPU6050_AttitudeEstimation_A(void)
// {
// 	yaw = 0;
// 	pitch = atan2(MPU6050.Acc_X, MPU6050.Acc_Z) / Pi * 180;
// 	roll = atan2(MPU6050.Acc_Y, MPU6050.Acc_Z) / Pi * 180;
// }

// void MPU6050_AttitudeEstimation(void)
// {
// 	pitch_g = pitch - 0.001 * ((float)MPU6050.Gyro_Y / 3.45);
// 	roll_g = roll + 0.001 * ((float)MPU6050.Gyro_X / 3.45);
// 	yaw_g = yaw + 0.001 * ((float)MPU6050.Gyro_Z / 3.45);

// 	pitch_a = atan2((float)MPU6050.Acc_X, (float)MPU6050.Acc_Z) / Pi * 180.0;
// 	roll_a = atan2((float)MPU6050.Acc_Y, (float)MPU6050.Acc_Z) / Pi * 180.0;

// 	//pitch = 0.9091 * pitch_a + 0.0909 * pitch_g;
// 	//roll = 0.9091 * roll_a + 0.0909 * roll_g;
// 	pitch = 0.99009 * pitch_g + 0.00991 * pitch_a;
// 	roll = 0.99009 * roll_g + 0.00991 * roll_a;

// 	yaw = yaw_g;
// }

// //    0.95238    0.04762
// //    0.99009    0.00991

