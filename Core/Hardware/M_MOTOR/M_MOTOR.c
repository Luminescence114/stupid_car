//
// Created by 董俊乾 on 2025/4/27.
//

#include <stdlib.h>
#include <string.h>
#include "M_MOTOR.h"
#include "main.h"
#include "tim.h"

int getspeed(char data[DATA_SIZE])
{
    char temp[3];
    temp[0] = data[5];
    temp[1] = data[6];
    temp[2] = data[7];
    int speed = atoi(temp);
    return speed;
}

void motor_ctrl(char dirction[4], int speed)
{
    if(strcmp(dirction,"goto") == 0){
        ccr_r = ccr_l = speed;
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_RESET);
    }
    else if(strcmp(dirction, "back") == 0){
        ccr_r = ccr_l = speed;
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_SET);
    }
    else if(strcmp(dirction, "stop") == 0){
        ccr_r = ccr_l = 0;
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_RESET);
    }
    else if(strcmp(dirction, "left") == 0){
        ccr_l = speed / 2;
        ccr_r = speed;
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_RESET);
    }
    else if(strcmp(dirction, "righ") == 0){
        ccr_l = speed;
        ccr_r = speed / 2;
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_RESET);
    }
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

        motor_ctrl("goto",500);
    }
    else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7) == GPIO_PIN_RESET)
    {
        motor_ctrl("stop",0);
        HAL_Delay(500);
        motor_ctrl("back",300);
        HAL_Delay(500);
        motor_ctrl("left",400);
        HAL_Delay(1000);
        if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7) == GPIO_PIN_RESET)
        {
            motor_ctrl("left",400);
        }
        else
        {
            motor_ctrl("goto",500);
        }
    }
    if(HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_2) == GPIO_PIN_SET)
    {

        motor_ctrl("goto",500);
    }
    else if(HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_2) == GPIO_PIN_RESET)
    {
        motor_ctrl("stop",0);
        HAL_Delay(500);
        motor_ctrl("back",400);
        HAL_Delay(500);
        motor_ctrl("righ",400);
        HAL_Delay(1000);
        if(HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_2) == GPIO_PIN_RESET)
        {
            motor_ctrl("righ",400);
        }
        else
        {
            motor_ctrl("goto",500);
        }
    }
}