//
// Created by 董俊乾 on 2025/4/27.
//

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "M_UART.h"
#include "usart.h"
#include "../M_MOTOR/M_MOTOR.h"

void USART1_Print(char *fmt, ...)
{
    uint8_t buf[128];
    uint16_t len;
    va_list ap;
    va_start(ap, fmt);
    len = vsnprintf((char *)buf,128, fmt, ap);
    va_end(ap);
    HAL_UART_Transmit(&huart1, buf, len, 100);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    char info[4];
    info[0] = ReceiveData[0];
    info[1] = ReceiveData[1];
    info[2] = ReceiveData[2];
    info[3] = ReceiveData[3];
    if(strlen(ReceiveData) == 8)
        speed = getspeed(ReceiveData);
    motor_ctrl(info,speed);

    USART1_Print("%s %d",ReceiveData,speed);
    memset(ReceiveData, 0, sizeof(ReceiveData));
    HAL_UARTEx_ReceiveToIdle_DMA ( &huart1, ReceiveData, DATA_SIZE);
}