#include "gd32f10x_it.h"
#include "system.h"
#include "can_package.h"
#include "decrypt.h"
#include "encrypt.h"

volatile float Curve[4];

void USB_LP_CAN1_RX0_IRQHandler(void) {
    can_receive_message_struct receive_message;
    can_interrupt_flag_clear(CAN0, CAN_INT_FLAG_RFF0);
    can_message_receive(CAN0, CAN_FIFO0, &receive_message);
    CAN_ReceiveCallback(receive_message.rx_sfid, receive_message.rx_data);
}

void USART1_IRQHandler(void) {
    if (RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)) {
        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);
        DTP_Received_CallBack(usart_data_receive(USART0));
    }
}

void TIM2_IRQHandler(void) {
    if (SET == timer_interrupt_flag_get(TIMER1, TIMER_INT_UP)) {
        DTP_Package_t pkg = {0};
        char counter;
        timer_interrupt_flag_clear(TIMER1, TIMER_INT_UP);
        pkg.PID = 0;
        for (counter = 0; counter < 4; counter++) {
            pkg.Data[counter * 2] = FloatToInt16(Curve[counter]) >> 8;
            pkg.Data[counter * 2 + 1] = FloatToInt16(Curve[counter]) & 0x00FF;
        }
        DTP_Transmit(&pkg);
    }
}

void NMI_Handler(void) {}
void HardFault_Handler(void) { while (1); }
void MemManage_Handler(void) { while (1); }
void BusFault_Handler(void) { while (1); }
void UsageFault_Handler(void) { while (1); }
void SVC_Handler(void) {}
void DebugMon_Handler(void) {}
void PendSV_Handler(void) {}
void SysTick_Handler(void) { delay_decrement(); }
