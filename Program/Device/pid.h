//
// Created by Lin·Wu on 2021/1/17.
//

#ifndef _ALGORITHM_PID_H_
#define _ALGORITHM_PID_H_

#define Ufabs(x) x > 0 ? x : -1 * x         //Ufabs(x)保证输出的值是一个正值，以此值用来与integral_max判断

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
    float Differ;                           //当前滤波后的输出值
    float Differ_l;                         //用于记录上次的滤波输出值
    float filter_parameter;                 //一阶低通滤波系数

} pid_parameter;

void pid_Integral(pid_parameter *pid);
void pid_Differential(pid_parameter *pid);
float pid_calculate(pid_parameter *pid);

#endif //_DEVICE_PID_H_
