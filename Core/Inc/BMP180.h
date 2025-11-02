#ifndef __BMP180_H
#define __BMP180_H
#include "i2c.h"

#define BMP_IIC             hi2c1
#define OSS                 2

#define BMP_Address         0x77
#define BMP_AC1_HIGH        0xAA
#define BMP_AC1_LOW         0XAB
#define BMP_AC2_HIGH        0xAC
#define BMP_AC2_LOW         0xAD
#define BMP_AC3_HIGH        0xAE
#define BMP_AC3_LOW         0xAF
#define BMP_AC4_HIGH        0xB0
#define BMP_AC4_LOW         0xB1
#define BMP_AC5_HIGH        0xB2
#define BMP_AC5_LOW         0xB3
#define BMP_AC6_HIGH        0xB4
#define BMP_AC6_LOW         0xB5
#define BMP_B1_HIGH         0xB6
#define BMP_B1_LOW          0xB7
#define BMP_B2_HIGH         0xB8
#define BMP_B2_LOW          0xB9
#define BMP_MB_HIGH         0xBA
#define BMP_MB_LOW          0xBB
#define BMP_MC_HIGH         0xBC
#define BMP_MC_LOW          0xBD
#define BMP_MD_HIGH         0xBE
#define BMP_MD_LOW          0xBF
#define BMP_Control         0xF4
#define BMP_Temperature     0x2E
#define BMP_Pressure0       0x34
#define BMP_Pressure1       0x74
#define BMP_Pressure2       0xB4
#define BMP_Pressure3       0xF4
#define BMP_ID              0xD0
#define BMP_SOFT_RESET      0xE0
#define BMP_CTRL_MEAS       0xF4
#define BMP_OUT_LOW         0xF7
#define BMP_OUT_HIGH        0xF6
#define BMP_OUT_XLAB        0xF8

extern float Temperature, Altitude;

/*****************************private************************************/
void __BMP_WriteReg(uint16_t reg_address, uint8_t data);
uint8_t __BMP_ReadReg(uint16_t reg_address);
/************************************************************************/

/*****************************public*************************************/
void BMP_Init(void);
void BMP_Call(void);
void BMP_GetInfor(void);
/************************************************************************/


#endif
