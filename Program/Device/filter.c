//
// Created by wu.lin on 2021/1/19.
//

#include "filter.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      一阶RC低通滤波器
//  @param      filter参数指针
//  @return     滤波后的输出值output
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
float low_pass_filter(filter_parameters *filter) {

    filter->output = filter->low_pass_parameter * filter->sampling_value
        + (1.0 - filter->low_pass_parameter) * filter->output_last;
    filter->output_last = filter->output;
    return filter->output;
}