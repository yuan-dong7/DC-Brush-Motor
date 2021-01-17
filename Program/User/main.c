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
        Curve[0] = Parameters[0];
        Curve[1] = Parameters[1];
        Curve[2] = Parameters[2];
        Curve[3] = Parameters[3];
        Delayms(20);
    }
}
