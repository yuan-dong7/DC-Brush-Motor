//
// Created by Lin·Wu on 2021/1/17.
//

#include "pid.h"

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
//  @return     Integral        返回最终的积分值
//  Sample usage:               无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
float pid_Integral(pid_parameter *pid) {
    float Integral;
    Integral = Integral + (float) (pid->error - pid->error_l) / (float) 2;           /*梯形积分的计算*/
    /*抗积分饱和算法*/
    if (Ufabs(Integral) > pid->Integral_max && Integral > 0) {
        Integral = pid->Integral_max;
    } else if (Ufabs(Integral) > pid->Integral_max && Integral < 0) {               /*积分饱和过冲时累加负项  */
        Integral = -1.0f * pid->Integral_max;                                       /*可使积分项快速退出饱和区*/
    }
    return Integral;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      微分项的计算（运用一阶低通滤波器）
//  @param      pid参数指针
//  @return     Differential    返回最终的微分值
//  Sample usage:           无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
float pid_Differential(pid_parameter *pid) {
    float Differential;                         //本次滤波的输出值
    float Differential_l;                       //上次滤波的输出值
    Differential = (float) ((pid->filter_parameter) * (pid->error - pid->error_l)
        + (1.0 - pid->filter_parameter) * Differential_l);
    Differential_l = Differential;
    return Differential;
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
    pid->Integral = pid_Integral(pid);
    pid->Differential = pid_Differential(pid);
    output = (float) (pid->kp * pid->error + pid->ki * pid->Integral + pid->kd * pid->Differential);
    if (output > pid->output_max) {
        output = pid->output_max;
    } else if (output < pid->output_min) {
        output = pid->output_min;
    }
    return output;
}
