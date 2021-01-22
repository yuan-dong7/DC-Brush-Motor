//
// Created by Lin·Wu on 2021/1/17.
//

#ifndef _ALGORITHM_PID_H_
#define _ALGORITHM_PID_H_

#include "filter.h"

#define Ufabs(x) (x >= 0 ? x : -1.0 * x)      //Ufabs(x)保证输出的值是一个正值，以此值用来与integral_max判断//少个括号就出问题我也不知道为什么

typedef struct {
    float kp, ki, kd;
    float error;                            //当前误差(current error)
    float error_l;                          //上次的误差(last current)
    float goal_value;                       //预期值
    float actual_value;                     //当前值
    float output;                           //pid计算后最终的输出值
    float output_max, output_min;           //输出值的最大/最小值
    float Integral_max;                     //积分项所能累加的最大值(warining:该值应该设为正值)
    float Integral;                         //积分项I
    float Differential;                     //微分项D

} pid_parameter;

//extern pid_parameter pid_list;
extern pid_parameter pid;

float pid_calculate(pid_parameter *pid);

#endif //_DEVICE_PID_H_
