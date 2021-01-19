#include "gd32f10x.h"
#include "main.h"

int main(void) {
    filter_parameters filter = {0};
    unsigned char i = 0;
    const float x[10] = {29.11, 28.98, 27.12, 30.55, 31.21, 32.11, 30.99, 29.55, 29.34, 30.22};
    System_Config();
    CAN_Config();
    GPIO_Config();
    UART_Config();
    SPI_Config();
    DMA_Config();
    ADC_Config();
    TIM_Config();

    float test = 30.00;
    while (1) {
        filter.sampling_value = x[i];
        if (i >= 9) {
            i = 0;
        }
//        Parameters[0] = pid->goal_value;
//        Parameters[1] = pid->actual_value;
        //filter.low_pass_parameter = Parameters[0];
//        Parameters[3] = filter_low_pass(pid);
        Curve[0] = test;
        Curve[1] = filter.sampling_value;
        Curve[2] = Bessel_filter(&filter);
        Curve[3] = 0;
        i++;
        Delayms(50);
    }
}
