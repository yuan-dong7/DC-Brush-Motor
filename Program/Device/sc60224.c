//
// Created by Lao·Zhu on 2021/1/14.
//

#include "sc60224.h"
#include "spi.h"
#include "system.h"
#include "gd32f10x.h"

unsigned short SC60224_ReadData(unsigned short TxData) {
    unsigned short data;
    gpio_bit_write(GPIOA, GPIO_PIN_4, RESET);
    data = SPI_ReadWriteHalfWord(TxData);
    gpio_bit_write(GPIOA, GPIO_PIN_4, SET);
    data = data & 0x0fff;
    return data;
}
float SC60224_GetAngle(void) {
    float angle;
    SC60224_ReadData(CMD_CLAER);
    SC60224_ReadData(CMD_ANGLE);
    angle = (SC60224_ReadData(CMD_NOP) / 4096.0f);
}
