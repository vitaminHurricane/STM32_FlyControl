#include "main.h"
#include "H24.h"
#include "spi.h"
#include "Delay.h"
#include "H24_Command.h"
#include "Motor.h"
#include "PID.h"
#include <stdio.h>
#include <math.h>

#define RX_PACKET_WIDTH         9
#define TX_PACKET_WIDTH         9

uint8_t send_buf[BUF_MAX];
uint8_t recieve_buf[BUF_MAX];
uint8_t TX_Buf[TX_PACKET_WIDTH] = {0x00};
uint8_t RX_Buf[RX_PACKET_WIDTH] = {0x00};

uint8_t RX0_Add[5] = {0x0A, 0x01, 0x0B, 0x02, 0x0C};
uint8_t TX_Add[5] = {0x0A, 0x01, 0x0B, 0x02, 0x0C};

uint16_t throttle = 0;

bool Bind_Flag = false;

/********************************************Basic***************************************************/
void __H24_Write_CSN(GPIO_PinState state)
{
	HAL_GPIO_WritePin(NSS_PORT, NSS_PIN, state);
}

void __H24_Write_CE(GPIO_PinState state)
{
    HAL_GPIO_WritePin(CE_PORT, CE_PIN, state);
}

void __H24_Clear_TxBuf(void)
{
    for (uint8_t i = 0; i < TX_PACKET_WIDTH; i++) {
        TX_Buf[i] = 0x00;
    }
}

void __H24_Clear_RxBuf(void)
{
    for (uint8_t i = 1; i < RX_PACKET_WIDTH; i++) {
        RX_Buf[i] = 0x00;
    }
}

void H24_WriteReg(uint8_t regaddress, uint8_t data)
{
    __H24_Write_CSN(GPIO_PIN_RESET);
    uint8_t command = regaddress | NRF_WRITE_REG;
    HAL_SPI_Transmit(&NRF_SPI, &command, 1, 100);
    HAL_SPI_Transmit(&NRF_SPI, &data, 1, 100);
    __H24_Write_CSN(GPIO_PIN_SET);
}

void H24_WriteRegs(uint8_t regaddress, uint8_t* send_buf, uint8_t cnt)
{
    __H24_Write_CSN(GPIO_PIN_RESET);
    uint8_t command = regaddress | NRF_WRITE_REG;
    HAL_SPI_Transmit(&NRF_SPI, &command, 1, 100);
    HAL_SPI_Transmit(&NRF_SPI, send_buf, cnt, 100);
    __H24_Write_CSN(GPIO_PIN_SET);
}

uint8_t H24_ReadReg(uint8_t regaddress)
{
    __H24_Write_CSN(GPIO_PIN_RESET);
    uint8_t command = regaddress | NRF_READ_REG;
    HAL_SPI_Transmit(&NRF_SPI, &command, 1, 100);
    uint8_t data;
    HAL_SPI_Receive(&NRF_SPI, &data, 1, 100);
    __H24_Write_CSN(GPIO_PIN_SET);
    return data;
}

void H24_ReadRegs(uint8_t regaddress, uint8_t *recieve_buf, uint8_t cnt)
{
    __H24_Write_CSN(GPIO_PIN_RESET);
    uint8_t command = regaddress | NRF_READ_REG;
    HAL_SPI_Transmit(&NRF_SPI, &command, 1, 100);
    HAL_SPI_Receive(&NRF_SPI, recieve_buf, cnt, 100);
    __H24_Write_CSN(GPIO_PIN_SET);    
}

void H24_WriteTxPayload(uint8_t *send_buf, uint8_t cnt)
{
    __H24_Write_CSN(GPIO_PIN_RESET);
    uint8_t command = WR_TX_PLOAD;
    HAL_SPI_Transmit(&NRF_SPI, &command, 1, 100);
    HAL_SPI_Transmit(&NRF_SPI, send_buf, cnt, 100);
    __H24_Write_CSN(GPIO_PIN_SET);
}

void H24_ReadRxPayload(uint8_t *recieve_buf, uint8_t cnt)
{
    __H24_Write_CSN(GPIO_PIN_RESET);
    uint8_t command = RD_RX_PLOAD;
    HAL_SPI_Transmit(&NRF_SPI, &command, 1, 100);
    HAL_SPI_Receive(&NRF_SPI, recieve_buf, cnt, 100);
    __H24_Write_CSN(GPIO_PIN_SET);
}

void H24_ClearTx(void)
{
    __H24_Write_CSN(GPIO_PIN_RESET);
    uint8_t command = FLUSH_TX;
    HAL_SPI_Transmit(&NRF_SPI, &command, 1, 100);
    __H24_Write_CSN(GPIO_PIN_SET);
}

void H24_ClearRx(void)
{
    __H24_Write_CSN(GPIO_PIN_RESET);
    uint8_t command = FLUSH_RX;
    HAL_SPI_Transmit(&NRF_SPI, &command, 1, 100);
    __H24_Write_CSN(GPIO_PIN_SET);
}

uint8_t H24_ReadStatu(void)
{
    uint8_t statu, command;
    __H24_Write_CSN(GPIO_PIN_RESET);
    command = STATUS | NRF_READ_REG;
    HAL_SPI_Transmit(&NRF_SPI, &command, 1, 100);
    HAL_SPI_Receive(&NRF_SPI, &statu, 1, 100);
    __H24_Write_CSN(GPIO_PIN_SET);
    return statu;
}
/********************************************************************************************/

/*****************************************NRF_MODE*******************************************/
void H24_PowerDown(void)
{
    __H24_Write_CE(GPIO_PIN_RESET);
    uint8_t command;
    command = H24_ReadReg(CONFIG);
    command &= ~0x02;
    H24_WriteReg(CONFIG, command);
}

void H24_Standby1(void)
{
    __H24_Write_CE(GPIO_PIN_RESET);
    uint8_t command;
    command = H24_ReadReg(CONFIG);
    command |= 0x02;
    H24_WriteReg(CONFIG, command);
}

void H24_RxMode(void)
{
    __H24_Write_CE(GPIO_PIN_RESET);
    uint8_t command;
    command = H24_ReadReg(CONFIG);
    command |= 0x03;
    //printf("%X\r\n", command);
    H24_WriteReg(CONFIG, command);
    //printf("%X\r\n", command);
    __H24_Write_CE(GPIO_PIN_SET);
}

void H24_TxMode(void)
{
    __H24_Write_CE(GPIO_PIN_RESET);
    uint8_t command;
    command = H24_ReadReg(CONFIG);
    command |= 0x02;
    command &= ~0x01;
    H24_WriteReg(CONFIG, command);
    __H24_Write_CE(GPIO_PIN_SET);
}
/********************************************************************************************/

void H24_Init(void)
{
    H24_WriteReg(CONFIG, 0x08);             //配置配置寄存器
    H24_WriteReg(EN_AA, 0x3F);              //使能自动应答通道
    H24_WriteReg(EN_RXADDR, 0x01);          //使能接收通道
    H24_WriteReg(SETUP_AW, 0x03);           //配置地址宽度
    H24_WriteReg(SETUP_RETR, 0x03);         //设置自动重传
    H24_WriteReg(RF_CH, 0x02);              //设置工作频率
    H24_WriteReg(RF_SETUP, 0x0E);           //配置射频速率和发射功率
    H24_WriteRegs(RX_ADDR_P0, RX0_Add, RX_ADR_WIDTH);  //配置接收通道0的地址
    H24_WriteReg(RX_PW_P0, RX_PACKET_WIDTH);           //配置接收通道0有效载荷字节数

    H24_RxMode();
}

//发送数据包直接改写Tx_Buf里的数据即可
//接收数据包直接读取Rx_Buf里的数据即可
void H24_SendData(void)
{
    H24_WriteRegs(TX_ADDR, TX_Add, TX_ADR_WIDTH);
    H24_WriteTxPayload(TX_Buf, TX_PACKET_WIDTH);
    H24_WriteRegs(RX_ADDR_P0, TX_Add, RX_ADR_WIDTH);

    H24_TxMode();
    while (1) {
        uint8_t statu = H24_ReadStatu();
        printf("%X\r\n", statu);

        if (statu & 0x20) {
            printf("Send OK\r\n");
            break;
        } else if (statu & 0x10) {
            printf("Send Failed\r\n");
            break;
        }
    }
    H24_WriteReg(STATUS, 0x30);         //清空标志位
    H24_ClearTx();
    H24_WriteRegs(RX_ADDR_P0, RX0_Add, RX_ADR_WIDTH);

    H24_RxMode();
}

uint8_t H24_RecieveData(void)
{
    uint8_t statu = H24_ReadStatu();
    if (statu & 0x40) {
        H24_ReadRxPayload(RX_Buf, RX_PACKET_WIDTH);
        H24_WriteReg(STATUS, 0x40);
        H24_ClearRx();
        return 1;
    }
    return 0;
}

void H24_Command(void)
{
    int16_t temp[4];
    float zan;
    if (RX_Buf[0] == Motor_All_OFF) {
		throttle = 0;
        Motor_SetSpeed(Channel_1, throttle);
        Motor_SetSpeed(Channel_2, throttle);
        Motor_SetSpeed(Channel_3, throttle);        
        Motor_SetSpeed(Channel_4, throttle); 
        PID_Start = false;
        for (uint8_t cnt = 0; cnt < 3; cnt++) {
            Motor_SetAllLED(GPIO_PIN_RESET);
            Delay_ms(100);
            Motor_SetAllLED(GPIO_PIN_SET);
            Delay_ms(100);
        } 
        Bind_Flag = false;
        Motor_SetAllLED(GPIO_PIN_RESET);
    } else {
        switch (RX_Buf[0])
        {
        case 0x01:              //配对成功
            for (uint8_t cnt = 0; cnt < 3; cnt++) {
                Motor_SetAllLED(GPIO_PIN_SET);
                Delay_ms(50);
                Motor_SetAllLED(GPIO_PIN_RESET);
                Delay_ms(50);
            }
            Bind_Flag = true;
            get_time = current_time;
            Motor_SetAllLED(GPIO_PIN_SET);
            break;
        case 0x02:              //开始与遥控器传输数据
            temp[0] = (RX_Buf[2] << 6) | RX_Buf[1]; //油门
            temp[1] = (RX_Buf[4] << 8) | RX_Buf[3]; //Yaw
            temp[2] = (RX_Buf[6] << 8) | RX_Buf[5]; //Pitch
            temp[3] = (RX_Buf[8] << 8) | RX_Buf[7]; //Roll
            zan = temp[0] / 4096.0 * 1000;
            if (zan > 530) {
                throttle = (zan - 520) * 2;
            } else {
                throttle = 0;
            }
            zan = temp[1] / 4096.0 * 2;
            target_yaw -= zan;
            zan = temp[2] / 4096.0 * 20;
            //target_pitch = 1.5 + zan;
            if (zan > 5) {
                target_pitch = balance_P + 3;
            } else if (zan < -5) {
                target_pitch = balance_P - 3;
            }
            zan = temp[3] / 4096.0 * 20;
            //target_roll = -5 - zan;
            if (zan > 5) {
                target_roll = balance_R - 3;
            } else if (zan < -5) {
                target_roll = balance_R + 3;
            }
            //printf("%d, %f, %f, %f\r\n", throttle, target_pitch, target_roll, target_yaw);
            if (throttle >= 50) {
                if (!PID_Start) {
                    PID_Start = true;
                }
            } else {
                throttle = 0;
                Motor_SetSpeed(Channel_1, throttle);
                Motor_SetSpeed(Channel_2, throttle);
                Motor_SetSpeed(Channel_3, throttle);        
                Motor_SetSpeed(Channel_4, throttle); 
                PID_Start = false;
            }
        case 0x03:          //设置平衡点
            
            break;
        default:
            break;
        }
    }
    __H24_Clear_RxBuf();
}
