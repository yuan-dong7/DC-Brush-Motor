#include "can.h"
#include "can_package.h"
#include "gd32f10x.h"
#include "Config.h"

void CAN_Transmit(unsigned int StandardID, unsigned char *buf) {
    unsigned char counter;
    can_trasnmit_message_struct transmit_message;
    transmit_message.tx_sfid = StandardID;
    transmit_message.tx_efid = 0x00;
    transmit_message.tx_ft = CAN_FT_DATA;
    transmit_message.tx_ff = CAN_FF_STANDARD;
    transmit_message.tx_dlen = 8;
    for (counter = 0; counter < 8; counter++)
        transmit_message.tx_data[counter] = buf[counter];
    can_message_transmit(CAN0, &transmit_message);
}

void CAN_Config(void) {
    can_parameter_struct can_init_parameter;
    can_filter_parameter_struct can_filter_parameter;

    rcu_periph_clock_enable(RCU_CAN0);
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_AF);

    /* configure CAN0 GPIO */
    gpio_init(GPIOA, GPIO_MODE_IPU, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12);

    can_struct_para_init(CAN_INIT_STRUCT, &can_init_parameter);
    can_struct_para_init(CAN_INIT_STRUCT, &can_filter_parameter);
    can_deinit(CAN0);

    can_init_parameter.time_triggered = DISABLE;
    can_init_parameter.auto_bus_off_recovery = DISABLE;
    can_init_parameter.auto_wake_up = DISABLE;
    can_init_parameter.no_auto_retrans = DISABLE;
    can_init_parameter.rec_fifo_overwrite = DISABLE;
    can_init_parameter.trans_fifo_order = DISABLE;
    can_init_parameter.working_mode = CAN_NORMAL_MODE;
    can_init_parameter.resync_jump_width = CAN_BT_SJW_1TQ;
    can_init_parameter.time_segment_1 = CAN_BT_BS1_5TQ;
    can_init_parameter.time_segment_2 = CAN_BT_BS2_3TQ;
    can_init_parameter.prescaler = 6;
    can_init(CAN0, &can_init_parameter);
    can_wakeup(CAN0);
    /* initialize filter */
    can_filter_parameter.filter_number = 0;
    can_filter_parameter.filter_mode = CAN_FILTERMODE_MASK;
    can_filter_parameter.filter_bits = CAN_FILTERBITS_32BIT;
    can_filter_parameter.filter_list_high = 0x0000;
    can_filter_parameter.filter_list_low = 0x0000;
    can_filter_parameter.filter_mask_high = 0x0000;
    can_filter_parameter.filter_mask_low = 0x0000;
    can_filter_parameter.filter_fifo_number = CAN_FIFO0;
    can_filter_parameter.filter_enable = ENABLE;
    can_filter_init(&can_filter_parameter);

    nvic_irq_enable(USBD_LP_CAN0_RX0_IRQn, CAN_PRIORITY, 0);
    can_interrupt_enable(CAN0, CAN_INT_RFNE0);
}
