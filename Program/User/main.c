#include "gd32f10x.h"
#include "main.h"
//filter_parameters filter = {0};
pid_parameter pid1 = {0};
pid_parameter *pid = &pid1;

int main(void) {

    filter_parameters filter = {0};
    pid->output_max = 30;
    pid->output_min = -30;
    pid->Integral_max = 30;
    pid->goal_value = 50;
    pid->actual_value = 20;

//    double x[10] = {29.11, 28.98, 27.12, 30.55, 31.21, 32.11, 30.99, 29.55, 29.34, 30.22};
//    char i = 0;
    System_Config();
    CAN_Config();
    GPIO_Config();
    UART_Config();
    SPI_Config();
    DMA_Config();
    ADC_Config();
    TIM_Config();

//    float test = 30.00;

    while (1) {

        pid->kp = Parameters[0];
        pid->ki = Parameters[1];
        pid->kd = Parameters[2];
        pid->actual_value += pid_calculate(pid);
        Curve[0] = pid->goal_value;
        Curve[1] = pid->actual_value;
        Curve[2] = 0;
        Curve[3] = 0;

        Delayms(50);

    }
//

}
