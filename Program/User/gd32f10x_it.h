#ifndef GD32F10X_IT_H
#define GD32F10X_IT_H

#include "gd32f10x.h"

/* function declarations */
/* this function handles NMI exception */
void NMI_Handler(void);
/* this function handles HardFault exception */
void HardFault_Handler(void);
/* this function handles MemManage exception */
void MemManage_Handler(void);
/* this function handles BusFault exception */
void BusFault_Handler(void);
/* this function handles UsageFault exception */
void UsageFault_Handler(void);
/* this function handles SVC exception */
void SVC_Handler(void);
/* this function handles DebugMon exception */
void DebugMon_Handler(void);
/* this function handles PendSV exception */
void PendSV_Handler(void);
/* this function handles SysTick exception */
void SysTick_Handler(void);
/* this function handles CAN1_RX0 exception */
void USB_LP_CAN1_RX0_IRQHandler(void);
/* this function handles USART1 exception */
void USART1_IRQHandler(void);

#endif /* GD32F10X_IT_H */
