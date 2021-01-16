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
        unsigned short Angle, RADC1, RADC2, RADC3;
        DTP_Package_t pkg = {0};
        unsigned char buf[8] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        CAN_Transmit(0x200, buf);

        RADC1 = FloatToInt16(ADC_Result[0] * 3.3f / 4096.0f);
        RADC2 = FloatToInt16(ADC_Result[1] * 3.3f / 4096.0f);
        RADC3 = FloatToInt16(ADC_Result[2] * 3.3f / 4096.0f);
        Angle = FloatToInt16(SC60224_GetAngle());
        pkg.PID = 0;
        pkg.Data[0] = RADC1 >> 8;
        pkg.Data[1] = RADC1 & 0x00FF;
        pkg.Data[2] = RADC2 >> 8;
        pkg.Data[3] = RADC2 & 0x00FF;
        
        pkg.Data[4] = RADC3 >> 8;
        pkg.Data[5] = RADC3 & 0x00FF;
        pkg.Data[6] = Angle >> 8;
        pkg.Data[7] = Angle & 0x00FF;
        DTP_Transmit(&pkg);
        Delayms(20);
    }
}
