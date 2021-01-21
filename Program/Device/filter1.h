//
// Created by 鸿鹄 on 2021-01-20.
//

#ifndef PROGRAM_DEVICE_FILTER1_H_
#define PROGRAM_DEVICE_FILTER1_H_
#include "user_math.h"
typedef struct butterworth_filter {     //巴特沃兹滤波器参数
    double Passband; //通带截止频率
    double Stopband;//阻带截止频率
    double Passband_attenuation;//通带衰减As
    double Stopband_attenuation;//阻带衰减Ap
} butterworth;

int s_filter_coefficient(int N, double Cutoff, double *a, double *b);//计算s域滤波系数数组a,b
int z_bilinear(int N, double *as, double *bs, double *az, double *bz);//计算z域滤波系数数组az,bz
double butterworth_output(double *pdAz, double *pdBz, int nABLen, double dDataIn, double *pdBuf);//计算输出值

#endif //PROGRAM_DEVICE_FILTER1_H_
