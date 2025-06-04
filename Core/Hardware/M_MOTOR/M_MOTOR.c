//
// Created by 董俊乾 on 2025/4/27.
//

#include <stdlib.h>
#include <string.h>
#include "M_MOTOR.h"
#include "main.h"
#include "tim.h"
#include "M_IR.h"

int outline_flag = 0;
int timeout_count = 0;


void motor_ctrl(int speed_l, int speed_r)
{
    if(speed_l > 0)
    {
        MOTOR_L_GO();
    }
    else if(speed_l == 0)
    {
        MOTOR_L_STOP();
    }
    else if(speed_l < 0)
    {
        MOTOR_L_BACK();
    }

    if(speed_r > 0)
    {
        MOTOR_R_GO();
    }
    else if(speed_r == 0)
    {
        MOTOR_R_STOP();
    }
    else if(speed_r < 0)
    {
        MOTOR_R_BACK();
    }
    ccr_l = abs(speed_l) >= 1000 ? 1000 : abs(speed_l);
    ccr_r = abs(speed_r) >= 1000 ? 1000 : abs(speed_r);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim == &htim7)
    {
        count++;
        if(count == ccr_l)
        {
            HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_RESET);
        }
        else if(count == ccr_r)
        {
            HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);

        }
        else if (count >= MAXARR)
        {
            HAL_GPIO_WritePin(GPIOD,GPIO_PIN_4,GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
            count = 0;
        }
    }
}
void control_menu(char mode)
{
    switch (mode)
    {
        case COMM_UP:
            if(Car_Mode == control)
            {
                motor_ctrl(FORWORDSPEED - COMPEN,FORWORDSPEED + COMPEN);
                USART1_Print("up");
            }
            break;
        case COMM_DOWN:
            if(Car_Mode == control)
            {
                motor_ctrl(-(FORWORDSPEED - COMPEN),-(FORWORDSPEED + COMPEN));
                USART1_Print("down");
            }
            break;
        case COMM_LEFT:
            if(Car_Mode == control)
            {
                motor_ctrl(FORWORDSPEED - COMPEN - 150,FORWORDSPEED + COMPEN + 150);
                USART1_Print("left");
            }
            break;
        case COMM_RIGHT:
            if(Car_Mode == control)
            {
                motor_ctrl(FORWORDSPEED - COMPEN + 150,FORWORDSPEED + COMPEN - 150);
                USART1_Print("right");
            }
            break;
        case COMM_STOP:
            if(Car_Mode == control)
            {
                motor_ctrl(0,0);
                USART1_Print("stop");
            }
            break;
        case COMM_CHANGE:
            USART1_Print("change");
            if(Car_Mode == control)
            {
                Car_Mode++;
                motor_ctrl(0,0);
            }
            else if(Car_Mode == trace)
            {
                Car_Mode++;
                motor_ctrl(0,0);
            }
            else if(Car_Mode >= avoid)
            {
                Car_Mode = control;
                motor_ctrl(0,0);
            }
            break;
        default :
            break;
    }
}
void avoid_l(void)
{
    motor_ctrl(-(400 - COMPEN),-(400 + COMPEN));
    HAL_Delay(300);
    motor_ctrl(500,0);
    HAL_Delay(500);
    motor_ctrl(500 - COMPEN,500 + COMPEN);
    HAL_Delay(600);
    motor_ctrl(0,700);
    HAL_Delay(600);
    motor_ctrl(300 - COMPEN,300 + COMPEN);
    HAL_Delay(500);
    while(HAL_GPIO_ReadPin(TRACE_L_GPIO_Port,TRACE_L_Pin) == GPIO_PIN_RESET) ;
    motor_ctrl(500,-200);
    HAL_Delay(300);

}

void avoid_r(void)
{
    motor_ctrl(-(400 - COMPEN),-(400 + COMPEN));
    HAL_Delay(300);
    motor_ctrl(0,500);
    HAL_Delay(500);
    motor_ctrl(500 - COMPEN,500 + COMPEN);
    HAL_Delay(600);
    motor_ctrl(700,0);
    HAL_Delay(600);
    motor_ctrl(300 - COMPEN,300 + COMPEN);
    HAL_Delay(500);
    while(HAL_GPIO_ReadPin(TRACE_R_GPIO_Port,TRACE_R_Pin) == GPIO_PIN_RESET) ;
    motor_ctrl(-200,500);
    HAL_Delay(300);
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
        motor_ctrl(-(300 - COMPEN),-(300 + COMPEN));
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
    int pos[sum] = {
            HAL_GPIO_ReadPin(TRACE_L_GPIO_Port,TRACE_L_Pin),
            HAL_GPIO_ReadPin(TRACE_M_GPIO_Port,TRACE_M_Pin),
            HAL_GPIO_ReadPin(TRACE_R_GPIO_Port,TRACE_R_Pin)
    };
    int obs[obs_sum] = {
            HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_2),
            HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)
    };
    if(pos[l] == GPIO_PIN_RESET && pos[m] == GPIO_PIN_SET && pos[r] == GPIO_PIN_RESET)
    {
        motor_ctrl(FORWORDSPEED - 100 - COMPEN,FORWORDSPEED - 100 + COMPEN);//中间
        outline_flag = timeout_count = 0;
    }
    else if(pos[l] == GPIO_PIN_RESET && pos[m] == GPIO_PIN_RESET && pos[r] == GPIO_PIN_SET)
    {
        motor_ctrl(FORWORDSPEED - 100 - COMPEN + 500,FORWORDSPEED - 100 + COMPEN - 500);//很左
        outline_flag = 1;
        timeout_count = 0;
    }
    else if(pos[l] == GPIO_PIN_RESET && pos[m] == GPIO_PIN_SET && pos[r] == GPIO_PIN_SET)
    {
        motor_ctrl(FORWORDSPEED - 100 - COMPEN + 300,FORWORDSPEED - 100 + COMPEN - 300);//偏左
        outline_flag = 1;
        timeout_count = 0;

    }
    else if(pos[l] == GPIO_PIN_SET && pos[m] == GPIO_PIN_RESET && pos[r] == GPIO_PIN_RESET)
    {
        motor_ctrl(FORWORDSPEED - 100 - COMPEN - 600,FORWORDSPEED - 100 + COMPEN + 600);//很右
        outline_flag = -1;
        timeout_count = 0;
    }
    else if(pos[l] == GPIO_PIN_SET && pos[m] == GPIO_PIN_SET && pos[r] == GPIO_PIN_RESET)
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
    if(obs[obs_l] == GPIO_PIN_RESET)
    {
        HAL_Delay(10);
        if(!HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_2))
            avoid_l();
    }
    else if(obs[obs_r] == GPIO_PIN_RESET)
    {
        HAL_Delay(10);
        if(!HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7))
            avoid_r();
    }
}