//
// Created by 鸿鹄 on 2021-01-20.
//

#include "filter1.h"

//-------------------------------------------------------------------------------------------------------------------
//  @brief      计算s域滤波系数数组a,b
//  @param      阶数，截止频率Ωc，s域滤波系数数组a，s域滤波系数数组b
//  @return     1
//  Sample usage:               无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------------------------------------------

int s_filter_coefficient(int N, double Cutoff, double *a, double *b) {
    double dk = 0;
    int k = 0;
    int i = 0;
    int count = 0, count_1 = 0;

    complex poles[N], Res[N + 1], Res_Save[N + 1];
    for (; i < N; i++) {
        poles[i].x = poles[i].y = 0;
        Res[i].x = Res[i].y = 0;
        Res_Save[i].x = Res[i].y = 0;
    }
    Res[N].x = Res[N].y = 0;
    Res_Save[N].x = Res[N].y = 0;
    if ((N % 2) == 0) dk = 0.5;
    else dk = 0;

    for (k = 0; k <= ((2 * N) - 1); k++) {              //求出极点
        if (Cutoff * XCos((2 * k + N - 1) * (pi / (2 * N))) < 0.0) {
            poles[count].x = -Cutoff * XCos((2 * k + N - 1) * (pi / (2 * N)));//求出极点的实部
            poles[count].y = -Cutoff * XSin((2 * k + N - 1) * (pi / (2 * N)));//求出极点的虚部
            count++;
            if (count == N) break;
        }
    }
    Res[0].x = poles[0].x;
    Res[0].y = poles[0].y;

    Res[1].x = 1;
    Res[1].y = 0;

    for (count_1 = 0; count_1 < N - 1; count_1++)//N个极点相乘次数
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
    for (count = 0; count <= N; count++)//Res[i]=a(i),i越大次数越高
    {
        Res[count].x = Res_Save[count].x;
        Res[count].y = Res_Save[count].y;

        *(a + N - count) = Res[count].x;
    }
    *(b + N) = *(a + N);
    return (int) 1;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      计算z域滤波系数数组az,bz
//  @param      阶数，s域滤波系数数组as，s域滤波系数数组bs，z域滤波系数数组az,z域滤波系数数组bz
//  @return     1
//  Sample usage:               无需用户调用，用户请使用h文件中的宏定义
//-------------------------------------------------------------------------------

int z_bilinear(int N, double *as, double *bs, double *az, double *bz) {
    int Count = 0, Count_1 = 0, Count_2 = 0, Count_Z = 0;
    double Res[N + 1], Res_Save[N + 1];
    int i = 0;
    for (; i <= N; i++) {
        Res[i] = Res_Save[i] = 0;
    }
    for (Count_Z = 0; Count_Z <= N; Count_Z++) {
        *(az + Count_Z) = 0;
        *(bz + Count_Z) = 0;
    }

    for (Count = 0; Count <= N; Count++) {
        for (Count_Z = 0; Count_Z <= N; Count_Z++) {
            Res[Count_Z] = 0;
            Res_Save[Count_Z] = 0;
        }
        Res_Save[0] = 1;

        for (Count_1 = 0; Count_1 < N - Count; Count_1++)//计算（1-Z^-1）^N-Count的系数,
        {                                                //Res_Save[]=Z^-1多项式的系数，从常数项开始
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

        for (Count_1 = (N - Count); Count_1 < N; Count_1++)//计算(1-Z^-1)^N-Count*（1+Z^-1）^Count的系数,
        {                                                //Res_Save[]=Z^-1多项式的系数，从常数项开始
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

    }//最外层for循环

    for (Count_Z = N; Count_Z >= 0; Count_Z--) {
        *(bz + Count_Z) = (*(bz + Count_Z)) / (*(az + 0));
        *(az + Count_Z) = (*(az + Count_Z)) / (*(az + 0));
    }
    return (int) 1;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      计算输出值
//  @param      z域滤波系数数组az,z域滤波系数数组bz，阶数+1，输入数据，中间数组
//  @return     dout
//  Sample usage:               无需用户调用，用户请使用h文件中的宏定义
//------------------------------------------------------------------------------
double butterworth_output(double *pdAz, double *pdBz, int nABLen, double dDataIn, double *pdBuf) {
    int i;
    int nALen;
    int nBLen;
    int nBufLen;
    double dOut;

    if (nABLen < 1)return 0.0;
    //根据参数,自动求取序列有效长度
    nALen = nABLen;
    for (i = nABLen - 1; i; --i) {
        if (*(pdAz + i) != 0.0)//从最后一个系数判断是否为0
        {
            nALen = i + 1;
            break;
        }
    }
    //printf("%lf ", nALen);
    if (i == 0) nALen = 0;

    nBLen = nABLen;
    for (i = nABLen - 1; i; --i) {
        if (*(pdBz + i) != 0.0) {
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
    dOut = (*pdAz) * dDataIn;  // a(0) * x(i)

    for (i = 1; i < nALen; i++)    // a(i) * w(n-i),i=1toN
    {
        dOut -= *(pdAz + i) * *(pdBuf + (nBufLen - 1) - i);
    }

    //卷乘结果保存为缓冲序列的最后一个
    *(pdBuf + nBufLen - 1) = dOut;

    //滤波: 与系数b卷乘
    dOut = 0.0;
    for (i = 0; i < nBLen; i++)    // b(i) * w(n-i)
    {
        dOut += *(pdBz + i) * *(pdBuf + (nBufLen - 1) - i);
    }

    //丢弃缓冲序列中最早的一个数, 最后一个数清零
    for (i = 0; i < nBufLen - 1; i++) {
        *(pdBuf + i) = *(pdBuf + i + 1);
    }
    *(pdBuf + nBufLen - 1) = 0;

    //返回输出值
    return dOut;
}

