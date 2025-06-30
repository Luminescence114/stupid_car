//
// Created by Luminescence on 2025/4/27.
//

#ifndef MYCAR111_M_MOTOR_H
#define MYCAR111_M_MOTOR_H
#include "main.h"
#include "M_UART.h"
#define MAXARR 1000
#define MOTOR_L_GO() \
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);\
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET)
#define MOTOR_L_STOP() \
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);\
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET)
#define MOTOR_L_BACK() \
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);\
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_0,GPIO_PIN_SET);

#define MOTOR_R_GO() \
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_SET); \
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_RESET);
#define MOTOR_R_STOP() \
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_RESET); \
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_RESET);
#define MOTOR_R_BACK() \
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_6,GPIO_PIN_RESET); \
    HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_SET);
typedef enum {
    l = 0,
    m,
    r,
    sum
} TraceSensor;
typedef enum {
    obs_l = 0,
    obs_r,
    obs_sum
} ObstacleSensor;
void motor_ctrl( int speed_l, int speed_r);
void control_menu(char mode);
void Avoid_obstacle(void);
void auto_trace(void);
#endif //MYCAR111_M_MOTOR_H
