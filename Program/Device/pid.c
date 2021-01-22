//
// Created by Lin·Wu on 2021/1/17.
//

#include "pid.h"
filter_parameters filter;
pid_parameter pid;

//-------------------------------------------------------------------------------------------------------------------
//  @brief      位置式pid的计算
//  @param      pid参数指针(&pid)
//  @return     output          返回pid计算出来的输出值
//  Sample usage:   用户直接调用即可
//-------------------------------------------------------------------------------------------------------------------

float pid_calculate(pid_parameter *pid) {
    pid->error = pid->goal_value - pid->actual_value;
    pid->error_l = pid->error;

    /*积分项的计算*/
    pid->Integral = pid->Integral + (float) (pid->error - pid->error_l) / (float) 2;             /*梯形积分的计算*/
    /*抗积分饱和算法*/
    if (Ufabs(pid->Integral) > pid->Integral_max && pid->Integral > 0) {
        pid->Integral = pid->Integral_max;
    } else if (Ufabs(pid->Integral) > pid->Integral_max && pid->Integral < 0) {               /*积分饱和过冲时累加负项  */
        pid->Integral = -1.0f * pid->Integral_max;                                               /*可使积分项快速退出饱和区*/
    }

    /*微分项的计算*/
    filter.sampling_value = pid->error;
    pid->error = low_pass_filter(&filter);                                                      /*运用了一阶RC低通滤波器*/

    pid->Differential = pid->error - pid->error_l;

    pid->output = (float) (pid->kp * pid->error + pid->ki * pid->Integral + pid->kd * pid->Differential);
    /*对pid输出量进行限幅*/
    if (pid->output > pid->output_max) {
        pid->output = pid->output_max;
    } else if (pid->output < pid->output_min) {
        pid->output = pid->output_min;
    }

    return pid->output;
}

