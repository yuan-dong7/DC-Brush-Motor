#include "gd32f10x.h"
#include "main.h"

int main(void) {

    System_Config();
    CAN_Config();
    GPIO_Config();
    UART_Config();
    SPI_Config();
    DMA_Config();
    ADC_Config();
    TIM_Config();

    while (1) {

        pid.kp = Parameters[0];
        pid.ki = Parameters[1];
        pid.kd = Parameters[2];
        pid.actual_value += pid_calculate(&pid);
        Curve[0] = pid.goal_value;
        Curve[1] = pid.actual_value;
        Curve[2] = 0;
        Curve[3] = 0;

        Delayms(50);

    }

}



//int main(void) {
//    //butterworth test
//
//    System_Config();
//    CAN_Config();
//    GPIO_Config();
//    UART_Config();
//    SPI_Config();
//    DMA_Config();
//    ADC_Config();
//    TIM_Config();
//
//    int ii = 0;
//    int j = 0;
//    float x[10] = {2, 1, 3, 1, 5, 2, 1, 5, 1, 0};
//    pass_parameters a;
//    float as[a.N + 1], bs[a.N + 1], az[a.N + 1], bz[a.N + 1],
//        data_buffer[a.N + 1];
//    for (; j <= a.N; j++) {
//        as[j] = bs[j] = az[j] = bz[j] = data_buffer[j] = 0;
//    }
//    a.N = 2;//用于改阶数
//    a.as = as;
//    a.bs = bs;
//    a.az = az;
//    a.bz = bz;
//    a.pdBuf = data_buffer;
//    butterworth_init(&a);
//    while (1) {
//        a.dDataIn = x[ii++];
//        if (ii = 10) {
//            ii = 0;
//        }
//        Curve[0] = butterworth_calculate(&a);
//        Delayms(50);
//    }
//
//}