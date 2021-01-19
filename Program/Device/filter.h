//
// Created by lin.wu on 2021/1/19.
//

#ifndef PROGRAM_DEVICE_FILTER_H_
#define PROGRAM_DEVICE_FILTER_H_

#define N 1

typedef struct {
    float low_pass_parameter;       //一阶低通滤波器滤波系数
    float sampling_value;           //需要滤波的采样值
    float output;                   //当前滤波结果
    float output_last;              //上次滤波结果
    float w[2];                     //贝塞尔滤波中间缓存状态变量

} filter_parameters;

float low_pass_filter(filter_parameters *filter);
float Bessel_filter(filter_parameters *filter);

#endif //PROGRAM_DEVICE_FILTER_H_
