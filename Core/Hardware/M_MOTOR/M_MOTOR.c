//
// Created by 董俊乾 on 2025/4/27.
//

#include <stdlib.h>
#include <string.h>
#include "M_MOTOR.h"
#include "main.h"
#include "tim.h"

int outline_flag = 0;
int timeout_count = 0;

int getspeed(char data[DATA_SIZE])
{
    char temp[3];
    temp[0] = data[5];
    temp[1] = data[6];
    temp[2] = data[7];
    int speed = atoi(temp);
    return speed;
}

void motor_ctrl(int speed_l, int speed_r)
{
    if(speed_l > 0)
    {
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);
    }
    else if(speed_l == 0)
    {
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);
    }
    else if(speed_l < 0)
    {
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);
    }

    if(speed_r > 0)
    {
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_RESET);
    }
    else if(speed_r == 0)
    {
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_RESET);
    }
    else if(speed_r < 0)
    {
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_SET);
    }
    ccr_l = abs(speed_l);
    ccr_r = abs(speed_r);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim7)
    {
        count++;
        if(count == ccr_l)
        {
            HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);
//            USART1_Print("A%d",count);
        }
        else if(count == ccr_r)
        {
            HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
//            USART1_Print("B%d",count);

        }
        else if (count >= MAXCCR)
        {
            HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
            count = 0;
        }
    }
}

void Avoid_obstacle(void)
{
    if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7) == GPIO_PIN_SET)
    {

        motor_ctrl(500,500);
    }
    else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7) == GPIO_PIN_RESET)
    {
        motor_ctrl(0,0);
        HAL_Delay(500);
        motor_ctrl(-300,-300);
        HAL_Delay(500);
        motor_ctrl(300,500);
        HAL_Delay(1000);
        if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7) == GPIO_PIN_RESET)
        {
            motor_ctrl(300,500);
        }
        else
        {
            motor_ctrl(500,500);
        }
    }
    if(HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_2) == GPIO_PIN_SET)
    {

        motor_ctrl(500,500);
    }
    else if(HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_2) == GPIO_PIN_RESET)
    {
        motor_ctrl(0,0);
        HAL_Delay(500);
        motor_ctrl(-400,-400);
        HAL_Delay(500);
        motor_ctrl(500,300);
        HAL_Delay(1000);
        if(HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_2) == GPIO_PIN_RESET)
        {
            motor_ctrl(500,300);
        }
        else
        {
            motor_ctrl(500,500);
        }
    }
}

void auto_trace(void)
{
    int pos[3] = {
            HAL_GPIO_ReadPin(TRACE_L_GPIO_Port,TRACE_L_Pin),
            HAL_GPIO_ReadPin(TRACE_M_GPIO_Port,TRACE_M_Pin),
            HAL_GPIO_ReadPin(TRACE_R_GPIO_Port,TRACE_R_Pin)
    };
    int obs[2] = {
            HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7),
            HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_2)
    };
    if(pos[0] == GPIO_PIN_RESET && pos[1] == GPIO_PIN_SET && pos[2] == GPIO_PIN_RESET)
    {
        motor_ctrl(FORWORDSPEED - COMPEN,FORWORDSPEED + COMPEN);//中间
        outline_flag = timeout_count = 0;
    }
    else if(pos[0] == GPIO_PIN_RESET && pos[1] == GPIO_PIN_RESET && pos[2] == GPIO_PIN_SET)
    {
        motor_ctrl(FORWORDSPEED - 100 - COMPEN + 500,FORWORDSPEED - 100 + COMPEN - 500);//很左
        outline_flag = 1;
        timeout_count = 0;
    }
    else if(pos[0] == GPIO_PIN_RESET && pos[1] == GPIO_PIN_SET && pos[2] == GPIO_PIN_SET)
    {
        motor_ctrl(FORWORDSPEED - 100 - COMPEN + 300,FORWORDSPEED - 100 + COMPEN - 300);//偏左
        outline_flag = 1;
        timeout_count = 0;

    }
    else if(pos[0] == GPIO_PIN_SET && pos[1] == GPIO_PIN_RESET && pos[2] == GPIO_PIN_RESET)
    {
        motor_ctrl(FORWORDSPEED - 100 - COMPEN - 600,FORWORDSPEED - 100 + COMPEN + 600);//很右
        outline_flag = -1;
        timeout_count = 0;
    }
    else if(pos[0] == GPIO_PIN_SET && pos[1] == GPIO_PIN_SET && pos[2] == GPIO_PIN_RESET)
    {
        motor_ctrl(FORWORDSPEED - 100 - COMPEN - 300,FORWORDSPEED - 100 + COMPEN + 300);//偏右
        outline_flag = -1;
        timeout_count = 0;
    }
    else
    {
        if(outline_flag == 1)
            motor_ctrl(FORWORDSPEED - 100 - COMPEN + 500,FORWORDSPEED - 100 + COMPEN - 500);
        else if(outline_flag == -1)
            motor_ctrl(FORWORDSPEED - 100 - COMPEN - 600,FORWORDSPEED - 100 + COMPEN + 600);
        else
            motor_ctrl(FORWORDSPEED - COMPEN, FORWORDSPEED + COMPEN);
    }
//    else
//    {
//        timeout_count++;
//        if(timeout_count >= 20)
//        {
//            motor_ctrl(0,0);
//        }
//    }
}