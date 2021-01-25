//
// Created by lin.wu on 2021/1/19.
//

#ifndef PROGRAM_DEVICE_FILTER_H_
#define PROGRAM_DEVICE_FILTER_H_

#include "user_math.h"

typedef struct {
    float low_pass_parameter;       //一阶低通滤波器滤波系数
    float sampling_value;           //需要滤波的采样值
    float output;                   //当前滤波结果
    float output_last;              //上次滤波结果
} filter_parameters;
extern  filter_parameters  filter;

typedef struct {
    int N;
    float Cutoff;
    float *as;
    float *bs;
    float *az;
    float *bz;
    float dDataIn;//以数组的形式一个一个的放入数据
    float *pdBuf;
} pass_parameters;  //给巴特沃斯滤波器进行传参

typedef struct {     //巴特沃兹滤波器参数
    double Passband; //通带截止频率
    double Stopband;//阻带截止频率
    double Passband_attenuation;//通带衰减As
    double Stopband_attenuation;//阻带衰减Ap
} butterworth;

//extern pass_parameters p_parameters = {0};
//extern butterworth butter = {0};

float low_pass_filter(filter_parameters *filter);
void butterworth_init(pass_parameters *parameter);
float butterworth_calculate(pass_parameters *parameter);
float bessel_filter(filter_parameters *filter, char N, float F_cutoff);

#endif //PROGRAM_DEVICE_FILTER_H_
