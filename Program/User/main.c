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