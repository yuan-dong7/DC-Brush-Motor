//
// Created by Lao·Zhu on 2021/1/14.
//

#ifndef _USER_CONFIG_H_
#define _USER_CONFIG_H_

#define CAN_PRIORITY    1           //CAN Receive Priority
#define UART_PRIORITY   2           //UART Receive Priority
#define TIM_PRIORITY    0           //Timer Receive Priority

#define UART_BAUDRATE   115200      //UART BaudRate 115200

#define PWM_FREQUENCY   10          //PWM Frequency Is 10kHz
#define PWM_INITDUTY    0           //PWM Initial Duty Cycle Is 50%

#define TIM_FREQUENCY   1           //TIM Interrupt Frequency 1kHz

#endif //_USER_CONFIG_H_
