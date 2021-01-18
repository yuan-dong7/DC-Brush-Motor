#include "gd32f10x.h"
#include "main.h"

pid_parameter pid1;
pid_parameter *pid = &pid1;
int i;

int main(void) {
    System_Config();
    CAN_Config();
    GPIO_Config();
    UART_Config();
    SPI_Config();
    DMA_Config();
    ADC_Config();
    TIM_Config();

    pid->goal_value = 30.00;
    float x[10] = {29.11, 28.98, 27.12, 30.55, 31.21, 32.11, 30.99, 29.55, 29.34, 30.22};
    pid->filter_parameter = 0.1;
    while (1) {
        pid->actual_value = x[i];
        if (i >= 9) {
            i = 0;
        }
//        Parameters[0] = pid->goal_value;
//        Parameters[1] = pid->actual_value;
//        pid->filter_parameter = Parameters[2];
//        Parameters[3] = filter_low_pass(pid);
        Curve[0] = pid->goal_value;
        Curve[1] = pid->actual_value;
        Curve[2] = 0;
        Curve[3] = 0;
//        Curve[2] = Parameters[2];
//        Curve[3] = Parameters[3];
        i++;
        Delayms(20);
    }
}
