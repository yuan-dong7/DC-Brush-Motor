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

//    //butterworth_test
//    int ii = 0;
//    float x[10] = {2, 1, 3, 1, 5, 2, 1, 5, 1, 0};
//    pass_parameters a;
//    butterworth_init(&a);
//
//    while (1) {
//        a.dDataIn = x[ii++];
//        if (ii = 10) {
//            ii = 0;
//        }
//        Curve[0]=butterworth_calculate(&a);
//        Delayms(50);
//    }
}