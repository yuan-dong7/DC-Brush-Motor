#include "gd32f10x.h"
#include "main.h"

#define N 2//经过测试二阶时候巴特沃斯滤波器滤波效果最好（也不一定，二阶时衰减幅度是最大的）

int main(void) {
//    filter_parameters filter = {0};
    int ii = 0;
    double x[10] = {29.11, 28.98, 27.12, 30.55, 31.21, 32.11, 30.99, 29.55, 29.34, 30.22};

    int count = 0;
    double Fcutoff;//转化截止频率
    int j = 0;
    butterworth IIR_Filter;
    IIR_Filter.Passband = (double) (2 * pi * 0.1);   //[rad]
    IIR_Filter.Stopband = (double) (2 * pi * 0.3);   //[rad]
    IIR_Filter.Passband_attenuation = 1;        //[dB]
    IIR_Filter.Stopband_attenuation = 60;       //[dB]


    IIR_Filter.Passband = 2 * XTan((IIR_Filter.Passband) / 2);   //[rad/sec]
    IIR_Filter.Stopband = 2 * XTan((IIR_Filter.Stopband) / 2);   //[rad/sec]
    Fcutoff = IIR_Filter.Passband / (mypow((mypow(10, 0.1 * IIR_Filter.Passband_attenuation) - 1), 1.0 / (2 * N)));
    double as[N + 1], bs[N + 1], az[N + 1], bz[N + 1], data_buffer[N + 1];
    for (; j <= N; j++) {
        as[j] = bs[j] = az[j] = bz[j] = data_buffer[j] = 0;
    }

    s_filter_coefficient(N, Fcutoff, as, bs);
    z_bilinear(N, as, bs, az, bz);//把这些算出来后是固定的，之后直接调用这些系数就行了就行了

    System_Config();
    CAN_Config();
    GPIO_Config();
    UART_Config();
    SPI_Config();
    DMA_Config();
    ADC_Config();
    TIM_Config();
    while (1) {
        Curve[0] = 30;
        Curve[1] = x[ii];
        Curve[2] = butterworth_output(az, bz, N + 1, x[ii], data_buffer);
        Curve[3] = 0;
        if (ii >= 9) {
            ii = 0;
        }
        ii++;
        Delayms(50);
    }
//    float test = 30.00;
//    while (1) {
//        filter.sampling_value = x[i];
//        if (i >= 9) {
//            i = 0;
//        }
////        Parameters[0] = pid->goal_value;
////        Parameters[1] = pid->actual_value;
//        //filter.low_pass_parameter = Parameters[0];
////        Parameters[3] = filter_low_pass(pid);
//        Curve[0] = test;
//        Curve[1] = filter.sampling_value;
//        Curve[2] = Bessel_filter(&filter);
//        Curve[3] = 0;
//        i++;
//        Delayms(50);

//    }


}
