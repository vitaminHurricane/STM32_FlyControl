#include "main.h"
#include "i2c.h"
#include "BMP180.h"
#include "Delay.h"
#include <stdio.h>
#include <math.h>

int16_t AC1, AC2, AC3, B1, B2, MB, MC, MD;
uint16_t AC4, AC5, AC6;
int32_t UT, UP, X1, X2, X3, B3, B5, B6, B7, T, p;
uint32_t B4;

float Temperature, Altitude;

void __BMP_WriteReg(uint16_t reg_address, uint8_t data)
{
    HAL_I2C_Mem_Write(&BMP_IIC, BMP_Address << 1, reg_address, I2C_MEMADD_SIZE_8BIT, &data, 1, 500);
}

uint8_t __BMP_ReadReg(uint16_t reg_address)
{
    uint8_t data;
    HAL_I2C_Mem_Read(&BMP_IIC, BMP_Address << 1, reg_address, I2C_MEMADD_SIZE_8BIT, &data, 1, 500);
    return data;
}

void BMP_Init(void)
{
    int32_t data_l, data_h;
    data_h = __BMP_ReadReg(BMP_AC1_HIGH);
    data_l = __BMP_ReadReg(BMP_AC1_LOW);
    AC1 = (data_h << 8) | data_l;
    data_h = __BMP_ReadReg(BMP_AC2_HIGH);
    data_l = __BMP_ReadReg(BMP_AC2_LOW);
    AC2 = (data_h << 8) | data_l;
    data_h = __BMP_ReadReg(BMP_AC3_HIGH);
    data_l = __BMP_ReadReg(BMP_AC3_LOW);
    AC3 = (data_h << 8) | data_l;
    data_h = __BMP_ReadReg(BMP_AC4_HIGH);
    data_l = __BMP_ReadReg(BMP_AC4_LOW);
    AC4 = (uint16_t)((data_h << 8) | data_l);
    data_h = __BMP_ReadReg(BMP_AC5_HIGH);
    data_l = __BMP_ReadReg(BMP_AC5_LOW);
    AC5 = (uint16_t)((data_h << 8) | data_l);
    data_h = __BMP_ReadReg(BMP_AC6_HIGH);
    data_l = __BMP_ReadReg(BMP_AC6_LOW);
    AC6 = (uint16_t)((data_h << 8) | data_l);
    data_h = __BMP_ReadReg(BMP_B1_HIGH);
    data_l = __BMP_ReadReg(BMP_B1_LOW);
    B1 = (data_h << 8) | data_l;
    data_h = __BMP_ReadReg(BMP_B2_HIGH);
    data_l = __BMP_ReadReg(BMP_B2_LOW);
    B2 = (data_h << 8) | data_l;
    data_h = __BMP_ReadReg(BMP_MB_HIGH);
    data_l = __BMP_ReadReg(BMP_MB_LOW);
    MB = (data_h << 8) | data_l;
    data_h = __BMP_ReadReg(BMP_MC_HIGH);
    data_l = __BMP_ReadReg(BMP_MC_LOW);
    MC = (data_h << 8) | data_l;
    data_h = __BMP_ReadReg(BMP_MD_HIGH);
    data_l = __BMP_ReadReg(BMP_MD_LOW);
    MD = (data_h << 8) | data_l;
}

void BMP_Call(void)
{
    if (HAL_I2C_IsDeviceReady(&BMP_IIC, BMP_Address << 1, 1, 100) == HAL_OK) {
        printf("BMP ready\r\n");
    } else {
        printf("No BMP180\r\n");
    }
}

void BMP_GetInfor(void)
{
    int32_t data_h, data_m, data_l;
    __BMP_WriteReg(BMP_Control, 0x2E);
    Delay_ms(5);
    data_h = __BMP_ReadReg(BMP_OUT_HIGH);
    data_l = __BMP_ReadReg(BMP_OUT_LOW);
    UT = (data_h << 8) | data_l;

    __BMP_WriteReg(BMP_Control, (BMP_Pressure2 | (OSS << 6)));
    Delay_ms(15);
    data_h = __BMP_ReadReg(BMP_OUT_HIGH);
    data_m = __BMP_ReadReg(BMP_OUT_LOW);
    data_l = __BMP_ReadReg(BMP_OUT_XLAB);
    UP = ((data_h << 16) + (data_m << 8) + data_l) >> (8 - OSS);

    X1 = (UT - AC6) * AC5 / pow(2, 15);
    X2 = MC * pow(2, 11) / (X1 + MD);
    B5 = X1 + X2;
    T = (B5 + 8) / 16;
    Temperature = (float)T * 0.1;
    printf("%f\r\n", Temperature);

    B6 = B5 - 4000;
    X1 = (B2 * (B6 * B6 / pow(2, 12))) / pow(2, 11);
    X2 = AC2 * B6 / pow(2, 11);
    X3 = X1 + X2;
    B3 = ((AC1 * 4 + X3) << OSS + 2) / 4;
    X1 = AC3 * B6 / pow(2, 13);
    X2 = (B1 * (B6 * B6 / pow(2, 12))) / pow(2, 16);
    X3 = ((X1 + X2) + 2) / 4;
    B4 = AC4 * (uint32_t)(X3 + 32768) / pow(2, 15);
    B7 = ((uint32_t)UP - B3) * (50000 >> OSS);
    if (B7 < 0x80000000) {
        p = (B7 * 2) / B4;
    } else {
        p = (B7 / B4) * 2;
    }
    X1 = (p / pow(2, 8)) * (p / pow(2, 8));
    X1 = (X1 * 3038) / pow(2, 16);
    X2 = (-7357 * p) / pow(2, 16);
    p = p + (X1 + X2 + 3791) / 16;
    //Altitude = 44330 * (1 - pow(((float)P / 101325), 1 / 5.255));
    printf("%d\r\n", p);
    printf("\r\n");
}


