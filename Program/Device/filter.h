//
// Created by lin.wu on 2021/1/19.
//

#ifndef PROGRAM_DEVICE_FILTER_H_
#define PROGRAM_DEVICE_FILTER_H_

#define Fcutoff 0.05

typedef struct {
    float low_pass_parameter;       //一阶低通滤波器滤波系数
    float sampling_value;           //需要滤波的采样值
    float output;                   //当前滤波结果
    float output_last;              //上次滤波结果


    double *as, *bs;                //s域中的参数as/bs列表
    double *az, *bz;                //z域中的参数az/bz列表
    char list_len;                  //参数列表长度
    double *data_buff;               //数据缓冲区


} filter_parameters;

float low_pass_filter(filter_parameters *filter);
float Bessel_filter(filter_parameters *filter, char N);

void Bessel(char N, double Cutoff, double *as, double *bs);
void Bilinear(char N, double *as, double *bs, double *az, double *bz);
float Bessel_Handle(filter_parameters *filter);

double mypow(double num, double n);

#endif //PROGRAM_DEVICE_FILTER_H_
