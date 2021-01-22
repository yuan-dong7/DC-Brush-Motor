#include "main.h"

float Amplitude_Frequency_Curve[128];
void Butterworth_Test(void) {
    for (unsigned char freq = 0; freq < 128; freq++) {
        float tmp_buf[0xff];
        float theta = 0;
        pass_parameters a;
        float as[a.N + 1], bs[a.N + 1], az[a.N + 1], bz[a.N + 1],
            data_buffer[a.N + 1];
        for (unsigned char counter = 0; counter <= a.N; counter++) {
            as[counter] = bs[counter] = az[counter] = bz[counter] = data_buffer[counter] = 0;
        }
        a.N = 2;
        a.as = as;
        a.bs = bs;
        a.az = az;
        a.bz = bz;
        a.pdBuf = data_buffer;
        butterworth_init(&a);
        for (unsigned char gen_cnt = 0; gen_cnt < 0xff; gen_cnt++) {
            theta = theta + 0.0246399f * freq;
            a.dDataIn = XSin(theta);
            tmp_buf[gen_cnt] = butterworth_calculate(&a);
        }
        Amplitude_Frequency_Curve[freq] = user_max(tmp_buf, 0xff);
    }
}

int main(void) {
    System_Config();
    CAN_Config();
    GPIO_Config();
    UART_Config();
    SPI_Config();
    DMA_Config();
    ADC_Config();
    TIM_Config();
    Butterworth_Test();
    for (int counter = 0; counter < 128; ++counter) {
        Curve[0] = Amplitude_Frequency_Curve[counter];
        Delayms(50);
    }
    while (1);
}
