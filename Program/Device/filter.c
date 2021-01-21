//
// Created by wu.lin on 2021/1/19.
//

#include "filter.h"

//char a[N] = {1, 1};
//char b[N] = {1, 1};
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      一阶RC低通滤波器
////  @param      filter参数指针
////  @return     滤波后的输出值output
////  Sample usage:
////-------------------------------------------------------------------------------------------------------------------
//float low_pass_filter(filter_parameters *filter) {
//
//    filter->output = filter->low_pass_parameter * filter->sampling_value
//        + (1.0 - filter->low_pass_parameter) * filter->output_last;
//    filter->output_last = filter->output;
//    return filter->output;
//}
//
////-------------------------------------------------------------------------------------------------------------------
////  @brief      一阶贝塞尔低通滤波器
////  @param      filter参数指针
////  @return     滤波后的输出值output
////  Sample usage:
////-------------------------------------------------------------------------------------------------------------------
//float Bessel_filter(filter_parameters *filter) {
//    filter->w[0] = (a[0] * filter->sampling_value) - (a[1] * filter->w[1]);
//    filter->output = (b[0] * filter->w[0]) + (b[1] * filter->w[1]);
//    filter->w[1] = filter->w[0];
//    filter->w[0] = 0;
//    return filter->output;
//}