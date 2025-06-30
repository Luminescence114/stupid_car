//
// Created by Luminescence on 2025/4/27.
//

#ifndef MYCAR111_M_IR_H
#define MYCAR111_M_IR_H

#include <stdint-gcc.h>

void IR_IntHandler(void);

#define COMM_STOP  'I'
#define COMM_UP    'A'
#define COMM_DOWN  'B'
#define COMM_LEFT  'C'
#define COMM_RIGHT 'D'
#define COMM_CHANGE 'E'

extern char ctrl_comm;
extern uint8_t ir_rec_flag;
#endif //MYCAR111_M_IR_H
