//
// Created by Lin·Wu on 2021/1/17.
//

#include "pid.h"

//pid_parameter *pid;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      pid参数初始化
//  @param      pid指针
//  @return     void
//  Sample usage:              none
//-------------------------------------------------------------------------------------------------------------------
void pid_init(pid_parameter *pid) {
    pid->kp = 0;
    pid->ki = 0;
    pid->kd = 0;
    pid->goal_value = 0;
    pid->actual_value = 0;
    pid->error_l = 0;
    pid->error = pid->goal_value - pid->actual_value;
    pid->output_max = 0;
    pid->output_min = 0;
    pid->output = 0;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      积分项的计算（运用包含抗积分饱和、梯形积分）
//  @param      pid参数指针
//  @return     integral        返回最终的积分值
//  Sample usage:               无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
float pid_integral(pid_parameter *pid) {
    float integral;
    integral = integral + (float) (pid->error - pid->error_l) / (float) 2;           /*梯形积分的计算*/
    /*抗积分饱和算法*/
    if (Ufabs(integral) > pid->integral_max && integral > 0) {
        integral = pid->integral_max;
    } else if (Ufabs(integral) > pid->integral_max && integral < 0) {               /*积分饱和过冲时累加负项  */
        integral = -1.0f * pid->integral_max;                                       /*可使积分项快速退出饱和区*/
    }
    return integral;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      微分项的计算（运用一阶低通滤波器）
//  @param      pid参数指针
//  @return     differential    返回最终的微分值
//  Sample usage:           无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
float pid_differential(pid_parameter *pid) {
    float differential;
    differential = (float) (pid->error - pid->error_l);
    return differential;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      位置式pid的计算
//  @param      pid参数指针
//  @return     output          返回pid计算出来的输出值
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
float pid_calculate(pid_parameter *pid) {

    float output;
    pid->error_l = pid->error;
    pid->integral = pid_integral(pid);
    pid->differential = pid_differential(pid);
    output = (float) (pid->kp * pid->error + pid->ki * pid->integral + pid->kd * pid->differential);
    if (output > pid->output_max) {
        output = pid->output_max;
    } else if (output < pid->output_min) {
        output = pid->output_min;
    }
    return output;
}
