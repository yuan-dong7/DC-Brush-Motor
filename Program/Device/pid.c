//
// Created by Lin·Wu on 2021/1/17.
//

#include "pid.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      积分项的计算（运用包含抗积分饱和、梯形积分）
//  @param      pid参数指针
//  @return     none
//  Sample usage:               无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
void pid_Integral(pid_parameter *pid) {
    pid->Integral = pid->Integral + (float) (pid->error - pid->error_l) / (float) 2;           /*梯形积分的计算*/
    /*抗积分饱和算法*/
    if (Ufabs(pid->Integral) > pid->Integral_max && pid->Integral > 0) {
        pid->Integral = pid->Integral_max;
    } else if (Ufabs(pid->Integral) > pid->Integral_max && pid->Integral < 0) {               /*积分饱和过冲时累加负项  */
        pid->Integral = -1.0f * pid->Integral_max;                                       /*可使积分项快速退出饱和区*/
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      微分项的计算（运用一阶低通滤波器）
//  @param      pid参数指针
//  @return     none
//  Sample usage:           无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
void pid_Differential(pid_parameter *pid) {
    pid->Differ_l = pid->Differ;
    pid->Differ = (float) ((pid->filter_parameter) * (pid->error)
        + (1.0 - pid->filter_parameter) * pid->Differ_l);
    pid->Differential = pid->Differ - pid->Differ_l;

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      位置式pid的计算
//  @param      pid参数指针
//  @return     output          返回pid计算出来的输出值
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
float pid_calculate(pid_parameter *pid) {
    pid->error = pid->goal_value - pid->actual_value;
    pid_Integral(pid);
    pid_Differential(pid);
    pid->output = (float) (pid->kp * pid->error + pid->ki * pid->Integral + pid->kd * pid->Differential);
    if (pid->output > pid->output_max) {
        pid->output = pid->output_max;
    } else if (pid->output < pid->output_min) {
        pid->output = pid->output_min;
    }
    pid->error_l = pid->error;
    return pid->output;
}
