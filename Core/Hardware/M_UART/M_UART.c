//
// Created by Luminescence on 2025/4/27.
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
