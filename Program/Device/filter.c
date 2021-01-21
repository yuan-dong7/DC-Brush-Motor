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

//-------------------------------------------------------------------------------------------------------------------
//  @brief      贝塞尔滤波器在s域中相关系数as/bs的计算
//  @param      阶数N;参数列表a/b用于存放as/bs的值
//  @return     void
//  Sample usage:   无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
void Bessel(char N, double Cutoff, double *as, double *bs) {
    double dFcut = Cutoff;              //cutoff是通带的截至角频率
    char count;

    /*-------------------------------通过N查表匹配不同的As以及Bs--------------------------------------------*/
    if (N == 1) {
        as[0] = 1;
        as[1] = 1;
    } else if (N == 2) {
        as[0] = 1;
        as[1] = 3;
        as[2] = 3;
    } else if (N == 3) {
        as[0] = 1;
        as[1] = 6;
        as[2] = 15;
        as[3] = 15;
    } else if (N == 4) {
        as[0] = 1;
        as[1] = 10;
        as[2] = 45;
        as[3] = 105;
        as[4] = 105;
    } else if (N == 5) {
        as[0] = 1;
        as[1] = 15;
        as[2] = 105;
        as[3] = 420;
        as[4] = 945;
        as[5] = 945;
    } else if (N == 6) {
        as[0] = 1;
        as[1] = 21;
        as[2] = 210;
        as[3] = 1260;
        as[4] = 4725;
        as[5] = 10395;
        as[6] = 10395;
    } else if (N == 7) {
        as[0] = 1;
        as[1] = 28;
        as[2] = 378;
        as[3] = 3150;
        as[4] = 17325;
        as[5] = 62370;
        as[6] = 135135;
        as[7] = 135135;
    } else if (N == 8) {
        as[0] = 1;
        as[1] = 36;
        as[2] = 630;
        as[3] = 6930;
        as[4] = 51975;
        as[5] = 270270;
        as[6] = 945945;
        as[7] = 2027025;
        as[8] = 2027025;
    } else if (N == 9) {
        as[0] = 1;
        as[1] = 45;
        as[2] = 990;
        as[3] = 13860;
        as[4] = 135135;
        as[5] = 945945;
        as[6] = 4729725;
        as[7] = 16216200;
        as[8] = 34459425;
        as[9] = 34459425;
    } else if (N == 10) {
        as[0] = 1;
        as[1] = 55;
        as[2] = 1485;
        as[3] = 25740;
        as[4] = 315315;
        as[5] = 2837835;
        as[6] = 18918900;
        as[7] = 91891800;
        as[8] = 310134825;
        as[9] = 654729075;
        as[10] = 654729075;
    }
    *(bs + N) = *(as + N);
    /*----------------------------------该表中的值无需修改--------------------------------------------------*/

    /*s域频率变换*/
    for (count = 0; count <= N; count++) {
        *(as + count) = *(as + count) / mypow(dFcut, N - count);
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      贝塞尔滤波器在z域中相关系数az/bz的计算
//  @param      阶数N;参数列表a/b用于存放as/bs的值;参数列表a/b用于存放az/bz的值
//  @return     void
//  Sample usage:   无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
void Bilinear(char N, double *as, double *bs, double *az, double *bz) {
    char Count = 0, Count_1 = 0, Count_2 = 0, Count_Z = 0;
    double *Res, *Res_Save;
    double RES[N + 1], RES_SAVE[N + 1];
    Res = RES;
    Res_Save = RES_SAVE;

    char counting = 0;
    for (counting = 0; counting < N + 1; counting++) {
        Res[counting] = 0;
        Res_Save[counting] = 0;
    }

    /*初始化az bz*/
    for (Count_Z = 0; Count_Z <= N; Count_Z++) {
        *(az + Count_Z) = 0;
        *(bz + Count_Z) = 0;
    }

    for (Count = 0; Count <= N; Count++) {
        /*初始化缓存数组*/
        for (Count_Z = 0; Count_Z <= N; Count_Z++) {
            Res[Count_Z] = 0;
            Res_Save[Count_Z] = 0;
        }
        Res_Save[0] = 1;

        for (Count_1 = 0; Count_1 < N - Count; Count_1++) {
            for (Count_2 = 0; Count_2 <= Count_1 + 1; Count_2++) {
                if (Count_2 == 0) {
                    Res[Count_2] += Res_Save[Count_2];
                } else if ((Count_2 == (Count_1 + 1)) && (Count_1 != 0)) {
                    Res[Count_2] += -Res_Save[Count_2 - 1];
                } else {
                    Res[Count_2] += Res_Save[Count_2] - Res_Save[Count_2 - 1];
                }
            }

            for (Count_Z = 0; Count_Z <= N; Count_Z++) {
                Res_Save[Count_Z] = Res[Count_Z];
                Res[Count_Z] = 0;
            }
        }

        for (Count_1 = (N - Count); Count_1 < N; Count_1++) {
            for (Count_2 = 0; Count_2 <= Count_1 + 1; Count_2++) {
                if (Count_2 == 0) {
                    Res[Count_2] += Res_Save[Count_2];
                } else if ((Count_2 == (Count_1 + 1)) && (Count_1 != 0)) {
                    Res[Count_2] += Res_Save[Count_2 - 1];
                } else {
                    Res[Count_2] += Res_Save[Count_2] + Res_Save[Count_2 - 1];
                }
            }

            for (Count_Z = 0; Count_Z <= N; Count_Z++) {
                Res_Save[Count_Z] = Res[Count_Z];
                Res[Count_Z] = 0;
            }
        }

        for (Count_Z = 0; Count_Z <= N; Count_Z++) {
            *(az + Count_Z) += mypow(2, N - Count) * (*(as + Count)) * Res_Save[Count_Z];
            *(bz + Count_Z) += (*(bs + Count)) * Res_Save[Count_Z];
        }
    }

    /*计算出az以及bz的数值*/
    for (Count_Z = N; Count_Z >= 0; Count_Z--) {
        *(bz + Count_Z) = (*(bz + Count_Z)) / (*(az + 0));
        *(az + Count_Z) = (*(az + Count_Z)) / (*(az + 0));
    }
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      由差分方程处理计算计算滤波结果
//  @param      滤波器参数列表Az;滤波器参数列表Bz;参数序列的长度nABLen；输入数据dDataIn;数据缓冲区pdBuf
//  @return     滤波后的输出值
//  Sample usage:   无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------
float Bessel_Handle(filter_parameters *filter) {
    char i;
    char nALen;
    char nBLen;
    char nBufLen;
//    double dOut;
    if (filter->list_len < 1)return 0.0;
    //根据参数，自动求取序列有效长度
    nALen = filter->list_len;
    for (i = filter->list_len - 1; i; --i) {
        if (*(filter->az + i) != 0.0)//从最后一个系数判断是否为0
        {
            nALen = i + 1;
            break;
        }
    }
    if (i == 0) nALen = 0;

    nBLen = filter->list_len;
    for (i = filter->list_len - 1; i; --i) {
        if (*(filter->bz + i) != 0.0) {
            nBLen = i + 1;
            break;
        }
    }
    if (i == 0) nBLen = 0;
    //计算缓冲区有效长度
    nBufLen = nALen;
    if (nALen < nBLen)
        nBufLen = nBLen;

    //滤波：与系数a卷乘
    filter->output = (*filter->az) * filter->sampling_value;  // a(0) * x(n  )
    //dOut = ( *pdA ) * *(pdBuf + 8574);
    for (i = 1; i < nALen; i++)    // a(i) * x(n-i)
    {
        filter->output -= *(filter->az + i) * *(filter->data_buff + (nBufLen - 1) - i);
    }

    //卷乘结果保存为缓冲序列最后一个
    *(filter->data_buff + nBufLen - 1) = filter->output;

    //滤波：与系数b卷乘
    filter->output = 0.0;
    for (i = 0; i < nBLen; i++)    // b(i) * x(n-i)
    {
        filter->output += *(filter->bz + i) * *(filter->data_buff + (nBufLen - 1) - i);
    }

    //丢弃缓冲序列中最早的一个数，最后一个数清0
    for (i = 0; i < nBufLen - 1; i++) {
        *(filter->data_buff + i) = *(filter->data_buff + i + 1);
    }
    *(filter->data_buff + nBufLen - 1) = 0;

    //返回输出值
    return filter->output;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      贝塞尔滤波器
//  @param      filter参数指针；滤波器阶数N
//  @return     滤波后的输出值
//  Sample usage:   用户直接调用即可
//-------------------------------------------------------------------------------------------------------------------
float Bessel_filter(filter_parameters *filter, char N) {
    double as[N + 1], bs[N + 1];
    char count = 0;
    for (count = 0; count < N + 1; count++) {
        as[count] = 0;
        bs[count] = 0;
    }
    filter->as = as;
    filter->bs = bs;

    Bessel(N, Fcutoff, filter->as, filter->bs);

    double az[N + 1], bz[N + 1];
    for (count = 0; count < N + 1; count++) {
        az[count] = 0;
        bz[count] = 0;
    }
    filter->az = az;
    filter->bz = bz;

    Bilinear(N, filter->as, filter->bs, filter->az, filter->bz);

    double data_buff[N + 1];
    for (count = 0; count < N + 1; count++) {
        data_buff[count] = 0;
        data_buff[count] = 0;
    }
    filter->data_buff = data_buff;
    filter->list_len = N + 1;

    filter->output = Bessel_Handle(filter);
    return filter->output;
}

double mypow(double num, double n) {
    double value = 1;
    int i = 1;
    if (n == 0) {
        value = 1;
    } else {
        while (i++ <= n) {
            value *= num;
        }
    }
    return value;
}

