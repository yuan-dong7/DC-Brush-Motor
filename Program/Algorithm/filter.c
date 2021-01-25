//
// Created by wu.lin on 2021/1/19.
//

#include "filter.h"
butterworth IIR_Filter;
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
//  @brief      N阶巴特沃斯滤波器
//  @param      巴特沃斯结构体参数
//  @return
//  function:   用于计算出离散系数数组az和bz
//-------------------------------------------------------------------------------------------------------------------
void butterworth_init(pass_parameters *parameter) {
    float Fcutoff;//转化截止频率

    IIR_Filter.
        Passband = (float) (2 * pi * 1);   //[rad]
    IIR_Filter.
        Stopband = (float) (2 * pi * 10);   //[rad]
    IIR_Filter.
        Passband_attenuation = 0;        //[dB]
    IIR_Filter.
        Stopband_attenuation = 20;       //[dB]


    IIR_Filter.
        Passband = 2 * XTan((IIR_Filter.Passband) / 2);   //[rad/sec]
    IIR_Filter.
        Stopband = 2 * XTan((IIR_Filter.Stopband) / 2);   //[rad/sec]
    Fcutoff =
        IIR_Filter.Passband / (mypow((mypow(10, 0.1 * IIR_Filter.Passband_attenuation) - 1), 1.0 / (2 * parameter->N)));
    parameter->Cutoff = Fcutoff;

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
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      N阶巴特沃斯滤波器
//  @param      巴特沃斯结构体参数
//  @return     滤波后的输出值dOut
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
float butterworth_calculate(pass_parameters *parameter) {
    int i = 0;
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

//-------------------------------------------------------------------------------------------------------------------
//  @brief      N阶贝塞尔滤波器
//  @param      filter参数指针;阶数N;截止频率F_cutoff
//  @return     滤波后的输出值output
//  Sample usage:   用户直接调用即可(阶数N最大取到5)
//-------------------------------------------------------------------------------------------------------------------
float bessel_filter(filter_parameters *filter, char N, float F_cutoff) {
    float as[N+1] , bs[N+1] ;
    float az[N+1] , bz[N+1] ;
    char Count = 0,Count_1 = 0,Count_2 = 0,Count_Z = 0;
    /*----------------------------贝塞尔滤波器在s中系数的计算--------------------------------*/
    /*通过N匹配不同的as以及bs*/
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
    }
    *(bs+N) = *(as+N);

    /*s域频率变换*/
    for(Count = 0; Count <=N; Count++)
    {
        *(as + Count) = *(as + Count)/mypow(F_cutoff, N - Count);
    }

/*------------------------双线性转换以及求贝塞尔滤波器在z域上的系数az/bz---------------------------*/
    float Res[N+1], Res_Save[N+1];

    for(Count_Z = 0;Count_Z <= N;Count_Z++)
    {
        *(az+Count_Z)  = 0;
        *(bz+Count_Z)  = 0;
    }

    for(Count = 0;Count<=N;Count++)
    {
        for(Count_Z = 0;Count_Z <= N;Count_Z++)
        {
            Res[Count_Z] = 0;
            Res_Save[Count_Z] = 0;
        }
        Res_Save [0] = 1;

        for(Count_1 = 0; Count_1 < N-Count;Count_1++)//计算（1-Z^-1）^N-Count的系数,
        {												//Res_Save[]=Z^-1多项式的系数，从常数项开始
            for(Count_2 = 0; Count_2 <= Count_1+1;Count_2++)
            {
                if(Count_2 == 0)
                {
                    Res[Count_2] += Res_Save[Count_2];
                }

                else if((Count_2 == (Count_1+1))&&(Count_1 != 0))
                {
                    Res[Count_2] += -Res_Save[Count_2 - 1];
                }

                else
                {
                    Res[Count_2] += Res_Save[Count_2] - Res_Save[Count_2 - 1];
                }
            }

            for(Count_Z = 0;Count_Z<= N;Count_Z++)
            {
                Res_Save[Count_Z]  =  Res[Count_Z] ;
                Res[Count_Z]  = 0;
            }
        }

        for(Count_1 = (N-Count); Count_1 < N;Count_1++)//计算(1-Z^-1)^N-Count*（1+Z^-1）^Count的系数,
        {												//Res_Save[]=Z^-1多项式的系数，从常数项开始
            for(Count_2 = 0; Count_2 <= Count_1+1;Count_2++)
            {
                if(Count_2 == 0)
                {
                    Res[Count_2] += Res_Save[Count_2];
                }

                else if((Count_2 == (Count_1+1))&&(Count_1 != 0))
                {
                    Res[Count_2] += Res_Save[Count_2 - 1];
                }

                else
                {
                    Res[Count_2] += Res_Save[Count_2] + Res_Save[Count_2 - 1];
                }
            }

            for(Count_Z = 0;Count_Z<= N;Count_Z++)
            {
                Res_Save[Count_Z]  =  Res[Count_Z] ;
                Res[Count_Z]  = 0;
            }
        }

        for(Count_Z = 0;Count_Z<= N;Count_Z++)
        {
            *(az+Count_Z) +=  mypow(2,N-Count)  *  (*(as+Count)) * Res_Save[Count_Z];
            *(bz+Count_Z) +=  (*(bs+Count)) * Res_Save[Count_Z];
        }

    }//最外层for循环


    for(Count_Z = 0;Count_Z < N + 1;Count_Z++)
    {
        *(bz+Count_Z) =  (*(bz+Count_Z))/(*(az));
        *(az+Count_Z) =  (*(az+Count_Z))/(*(az));
    }

    /*---------------------------------------------------------------------------------------*/
    char list_length = N + 1;
    float data_Buffer[list_length];
    char i = 0;
    char nALen, nBLen, nBufLen;

    if( list_length<1 ){
        return 0.0;
    }
    //根据参数,自动求取序列有效长度
    nALen = list_length;
    for( i=list_length - 1; i; --i )
    {
        if( *(az+i) != 0.0 )//从最后一个系数判断是否为0
        {
            nALen = i+1;
            break;
        }
    }
    //printf("%lf ", nALen);
    if( i==0 ) {
        nALen = 0;
    }

    nBLen = list_length;
    for( i=list_length-1; i; --i )
    {
        if( *(bz+i) != 0.0 )
        {
            nBLen = i+1;
            break;
        }
    }
    //printf("%lf ", nBLen);
    if( i==0 ){
        nBLen = 0;
    }
    //计算缓冲区有效长度
    nBufLen = nALen;
    if( nALen < nBLen) {
        nBufLen = nBLen;
    }

    //滤波: 与系数a卷乘
    filter->output = ( *az ) * filter->sampling_value;  // a(0) * x(i)

    for( i=1; i<nALen; i++)	// a(i) * w(n-i),i=1toN
    {
        filter->output -= *(az+i) * *(data_Buffer + (nBufLen - 1) - i);
    }

    //卷乘结果保存为缓冲序列的最后一个
    *(data_Buffer + nBufLen - 1) = filter->output;

    //滤波: 与系数b卷乘
    filter->output = 0.0;
    for( i=0; i<nBLen; i++)	// b(i) * w(n-i)
    {
        filter->output += *(bz+i) * *(data_Buffer + (nBufLen - 1) - i);
    }

    //丢弃缓冲序列中最早的一个数, 最后一个数清零
    for( i=0; i<nBufLen-1; i++)
    {
        *(data_Buffer + i) = *(data_Buffer + i + 1);
    }
    *(data_Buffer + nBufLen - 1) = 0;

    return filter->output;

}



