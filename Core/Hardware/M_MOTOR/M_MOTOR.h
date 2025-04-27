//
// Created by 董俊乾 on 2025/4/27.
//

#ifndef MYCAR111_M_MOTOR_H
#define MYCAR111_M_MOTOR_H
#define MAXCCR 1000
#include "main.h"
void motor_ctrl(char dirction[4], int speed);
int getspeed(char data[DATA_SIZE]);
void Avoid_obstacle(void);
#endif //MYCAR111_M_MOTOR_H
