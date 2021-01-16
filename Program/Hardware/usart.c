#include "usart.h"
#include "gd32f10x.h"
#include "Config.h"

void UART_Config(void) {
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_USART0);
    nvic_irq_enable(USART0_IRQn, UART_PRIORITY, 0);
    gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
    gpio_pin_remap_config(GPIO_USART0_REMAP, ENABLE);
    usart_deinit(USART0);
    usart_baudrate_set(USART0, UART_BAUDRATE);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
    usart_interrupt_enable(USART0, USART_INT_RBNE);
}
