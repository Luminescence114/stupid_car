//
// Created by 董俊乾 on 2025/4/27.
//

#include <stdint-gcc.h>
#include "M_IR.h"
#include "tim.h"
#include "M_UART.h"
#include "M_MOTOR.h"

#define IRIN_PIN GPIO_PIN_10
#define IRIN_GPIO GPIOD


unsigned char ir_rec_flag = 0;
unsigned char IRCOM[4];
char ctrl_comm = COMM_STOP;
/*
 * 范围�?0-65535us
 */
void Delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim6, 0); // 定时器计数清零
    __HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE); // 清除更新中断标志（重要！！）

    HAL_TIM_Base_Start(&htim6); // 开始计数

    while (__HAL_TIM_GET_COUNTER(&htim6) < us); // 等待时间到

    HAL_TIM_Base_Stop(&htim6);  // 停止定时器
}
void DelayIr(uint8_t x)
{
    while(x--) {
        Delay_us(140);
    }
}
void IRIntIsr(void)
{
    uint8_t j, k, N = 0;

    DelayIr(15);  // 延时大约2ms防抖
    if (HAL_GPIO_ReadPin(IRIN_GPIO, IRIN_PIN) == GPIO_PIN_SET)
    {
        return;
    }

    int continue_time = 40; // 红外连续超时时间设置

    // 等待引导码9ms的低电平结束
    while (HAL_GPIO_ReadPin(IRIN_GPIO, IRIN_PIN) == GPIO_PIN_RESET)
    {
        DelayIr(1);
    }
    // 开始接收4字节数据
    for (j = 0; j < 4; j++)
    {
        for (k = 0; k < 8; k++)
        {
            // 等待低电平开始
            while (HAL_GPIO_ReadPin(IRIN_GPIO, IRIN_PIN) == GPIO_PIN_SET)
            {
                DelayIr(1);
            }

            // 等待高电平到来
            while (HAL_GPIO_ReadPin(IRIN_GPIO, IRIN_PIN) == GPIO_PIN_RESET)
            {
                DelayIr(1);
            }

            N = 0;
            // 计量高电平持续时间
            while (HAL_GPIO_ReadPin(IRIN_GPIO, IRIN_PIN) == GPIO_PIN_SET)
            {
                DelayIr(1);
                N++;
                if (N >= 30)   // 超过30个单位（大概4ms）算错误，退出
                {
                    return;
                }
            }

            IRCOM[j] >>= 1;    // 低位先传
            if (N >= 8)        // 时间长短区分0/1
            {
                IRCOM[j] |= 0x80;
            }
        }
    }

    // 校验：地址反码校验
    k = ~IRCOM[3];
    if (IRCOM[2] != k)
    {
        return;
    }

    // 解析控制命令
    switch (IRCOM[2])
    {
        case 0x46: ctrl_comm = COMM_UP;    break;
        case 0x15: ctrl_comm = COMM_DOWN;  break;
        case 0x44: ctrl_comm = COMM_LEFT;  break;
        case 0x43: ctrl_comm = COMM_RIGHT; break;
        case 0x40: ctrl_comm = COMM_STOP;  break;
        case 0x45: ctrl_comm = COMM_CHANGE;  break;
        case 0x47: Car_Mode = avoid;motor_ctrl(0,0);break;
        default: return;
    }

    control_menu(ctrl_comm);
}

/*
 * HAL库外部中断回调函数（需要在main.c里留着）
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_10)
    {
        IRIntIsr();
    }

}