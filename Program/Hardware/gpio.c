#include "gpio.h"
#include "gd32f10x.h"

void GPIO_Config(void) {
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    gpio_init(GPIOB, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
    gpio_init(GPIOA, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
    gpio_bit_write(GPIOA, GPIO_PIN_4, SET);
    gpio_bit_write(GPIOB, GPIO_PIN_1, RESET);
    gpio_bit_write(GPIOB, GPIO_PIN_2, SET);
}
