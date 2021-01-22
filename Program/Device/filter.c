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

void butterworth_init(pass_parameters *parameter) {
    float Fcutoff;//转化截止频率
    int j = 0;
    butterworth IIR_Filter;
    IIR_Filter.
        Passband = (double) (2 * pi * 0.2);   //[rad]
    IIR_Filter.
        Stopband = (double) (2 * pi * 0.25);   //[rad]
    IIR_Filter.
        Passband_attenuation = 2;        //[dB]
    IIR_Filter.
        Stopband_attenuation = 20;       //[dB]


    IIR_Filter.
        Passband = 2 * XTan((IIR_Filter.Passband) / 2);   //[rad/sec]
    IIR_Filter.
        Stopband = 2 * XTan((IIR_Filter.Stopband) / 2);   //[rad/sec]
    Fcutoff =
        IIR_Filter.Passband / (mypow((mypow(10, 0.1 * IIR_Filter.Passband_attenuation) - 1), 1.0 / (2 * parameter->N)));
    parameter->Cutoff = Fcutoff;
    float as[parameter->N + 1], bs[parameter->N + 1], az[parameter->N + 1], bz[parameter->N + 1],
        data_buffer[parameter->N + 1];
    for (; j <= parameter->N; j++) {
        as[j] = bs[j] = az[j] = bz[j] = data_buffer[j] = 0;
    }
    parameter->N = 2;
    parameter->as = as;
    parameter->bs = bs;
    parameter->az = az;
    parameter->bz = bz;
    parameter->pdBuf = data_buffer;
}

float butterworth_calculate(pass_parameters *parameter) {

    //计算s域系数
    double dk = 0;
    int k = 0;
    int i = 0;
    int count = 0, count_1 = 0;

    complex poles[parameter->N], Res[parameter->N + 1], Res_Save[parameter->N + 1];
    for (; i < parameter->N; i++) {
        poles[i].x = poles[i].y = 0;
        Res[i].x = Res[i].y = 0;
        Res_Save[i].x = Res[i].y = 0;
    }
    Res[parameter->N].x = Res[parameter->N].y = 0;
    Res_Save[parameter->N].x = Res[parameter->N].y = 0;
    if ((parameter->N % 2) == 0) dk = 0.5;
    else dk = 0;

    for (k = 0; k <= ((2 * parameter->N) - 1); k++) {              //求出极点
        if (parameter->Cutoff * XCos((2 * k + parameter->N - 1) * (pi / (2 * parameter->N))) < 0.0) {
            poles[count].x = -parameter->Cutoff * XCos((2 * k + parameter->N - 1) * (pi / (2 * parameter->N)));//求出极点的实部
            poles[count].y = -parameter->Cutoff * XSin((2 * k + parameter->N - 1) * (pi / (2 * parameter->N)));//求出极点的虚部
            count++;
            if (count == parameter->N) break;
        }
    }
    Res[0].x = poles[0].x;
    Res[0].y = poles[0].y;

    Res[1].x = 1;
    Res[1].y = 0;

    for (count_1 = 0; count_1 < parameter->N - 1; count_1++)//N个极点相乘次数
    {
        for (count = 0; count <= count_1 + 2; count++) {
            if (0 == count) {
                Res_Save[count] = user_ComplexMul(Res[count], poles[count_1 + 1]);
            } else if ((count_1 + 2) == count) {
                Res_Save[count].x += Res[count - 1].x;
                Res_Save[count].y += Res[count - 1].y;
            } else {
                Res_Save[count] = user_ComplexMul(Res[count], poles[count_1 + 1]);
                Res_Save[count].x += Res[count - 1].x;
                Res_Save[count].y += Res[count - 1].y;
            }
        }
    }
    for (count = 0; count <= parameter->N; count++)//Res[i]=a(i),i越大次数越高
    {
        Res[count].x = Res_Save[count].x;
        Res[count].y = Res_Save[count].y;

        *(parameter->as + parameter->N - count) = Res[count].x;
    }
    *(parameter->bs + parameter->N) = *(parameter->as + parameter->N);


    //双线性z变化计算z域系数
    int Count = 0, Count_1 = 0, Count_2 = 0, Count_Z = 0;
    float res[parameter->N + 1], res_Save[parameter->N + 1];
    i = 0;
    for (; i <= parameter->N; i++) {
        res[i] = res_Save[i] = 0;
    }
    for (Count_Z = 0; Count_Z <= parameter->N; Count_Z++) {
        *(parameter->az + Count_Z) = 0;
        *(parameter->bz + Count_Z) = 0;
    }

    for (Count = 0; Count <= parameter->N; Count++) {
        for (Count_Z = 0; Count_Z <= parameter->N; Count_Z++) {
            res[Count_Z] = 0;
            res_Save[Count_Z] = 0;
        }
        res_Save[0] = 1;

        for (Count_1 = 0; Count_1 < parameter->N - Count; Count_1++)//计算（1-Z^-1）^N-Count的系数,
        {                                                //Res_Save[]=Z^-1多项式的系数，从常数项开始
            for (Count_2 = 0; Count_2 <= Count_1 + 1; Count_2++) {
                if (Count_2 == 0) {
                    res[Count_2] += res_Save[Count_2];
                } else if ((Count_2 == (Count_1 + 1)) && (Count_1 != 0)) {
                    res[Count_2] += -res_Save[Count_2 - 1];
                } else {
                    res[Count_2] += res_Save[Count_2] - res_Save[Count_2 - 1];
                }
            }

            for (Count_Z = 0; Count_Z <= parameter->N; Count_Z++) {
                res_Save[Count_Z] = res[Count_Z];
                res[Count_Z] = 0;
            }
        }

        for (Count_1 = (parameter->N - Count); Count_1 < parameter->N; Count_1++)//计算(1-Z^-1)^N-Count*（1+Z^-1）^Count的系数,
        {                                                //Res_Save[]=Z^-1多项式的系数，从常数项开始
            for (Count_2 = 0; Count_2 <= Count_1 + 1; Count_2++) {
                if (Count_2 == 0) {
                    res[Count_2] += res_Save[Count_2];
                } else if ((Count_2 == (Count_1 + 1)) && (Count_1 != 0)) {
                    res[Count_2] += res_Save[Count_2 - 1];
                } else {
                    res[Count_2] += res_Save[Count_2] + res_Save[Count_2 - 1];
                }
            }

            for (Count_Z = 0; Count_Z <= parameter->N; Count_Z++) {
                res_Save[Count_Z] = res[Count_Z];
                res[Count_Z] = 0;
            }
        }

        for (Count_Z = 0; Count_Z <= parameter->N; Count_Z++) {
            *(parameter->az + Count_Z) +=
                mypow(2, parameter->N - Count) * (*(parameter->as + Count)) * res_Save[Count_Z];
            *(parameter->bz + Count_Z) += (*(parameter->bs + Count)) * res_Save[Count_Z];
        }

    }//最外层for循环

    for (Count_Z = parameter->N; Count_Z >= 0; Count_Z--) {
        *(parameter->bz + Count_Z) = (*(parameter->bz + Count_Z)) / (*(parameter->az + 0));
        *(parameter->az + Count_Z) = (*(parameter->az + Count_Z)) / (*(parameter->az + 0));
    }

    /*********************************************************输出***************************************************/

    int nALen;
    int nBLen;
    int nBufLen;
    float dOut;

    if (parameter->N + 1 < 1)return 0.0;
    //根据参数,自动求取序列有效长度
    nALen = parameter->N + 1;
    for (i = parameter->N; i; --i) {
        if (*(parameter->az + i) != 0.0)//从最后一个系数判断是否为0
        {
            nALen = i + 1;
            break;
        }
    }
    if (i == 0) nALen = 0;

    nBLen = parameter->N + 1;
    for (i = parameter->N; i; --i) {
        if (*(parameter->bz + i) != 0.0) {
            nBLen = i + 1;
            break;
        }
    }
    if (i == 0) nBLen = 0;
    //计算缓冲区有效长度
    nBufLen = nALen;
    if (nALen < nBLen)
        nBufLen = nBLen;

    //滤波: 与系数a卷乘
    dOut = (*parameter->az) * parameter->dDataIn;  // a(0) * x(i)

    for (i = 1; i < nALen; i++)    // a(i) * w(n-i),i=1toN
    {
        dOut -= *(parameter->az + i) * *(parameter->pdBuf + (nBufLen - 1) - i);
    }

    //卷乘结果保存为缓冲序列的最后一个
    *(parameter->pdBuf + nBufLen - 1) = dOut;

    //滤波: 与系数b卷乘
    dOut = 0.0;
    for (i = 0; i < nBLen; i++)    // b(i) * w(n-i)
    {
        dOut += *(parameter->bz + i) * *(parameter->pdBuf + (nBufLen - 1) - i);
    }

    //丢弃缓冲序列中最早的一个数, 最后一个数清零
    for (i = 0; i < nBufLen - 1; i++) {
        *(parameter->pdBuf + i) = *(parameter->pdBuf + i + 1);
    }
    *(parameter->pdBuf + nBufLen - 1) = 0;

    //返回输出值
    return dOut;

}


