#include "tim.h"
#include "Config.h"
#include "gd32f10x.h"

unsigned int Period = 0;

void TIM_SetDutyCycle(float DutyCycle, unsigned int Channel) {
    switch (Channel) {
        case 0:TIMER_CH0CV(TIMER0) = (uint32_t) ((Period * DutyCycle) / 1);
            break;
        case 1:TIMER_CH1CV(TIMER0) = (uint32_t) ((Period * DutyCycle) / 1);
            break;
        default:break;
    }
}

void TIM_Config(void) {
    Period = (54000 / PWM_FREQUENCY) - 1;
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_AF);

    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;
    rcu_periph_clock_enable(RCU_TIMER0);
    timer_deinit(TIMER0);

    /* TIMER0 configuration */
    timer_initpara.prescaler = 1;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = Period;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER0, &timer_initpara);

    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_ENABLE;
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER0, TIMER_CH_0, &timer_ocintpara);
    timer_channel_output_config(TIMER0, TIMER_CH_1, &timer_ocintpara);

    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0,
                                            (unsigned short) ((Period + 1) * PWM_INITDUTY) - 1);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_1,
                                            (unsigned short) ((Period + 1) * PWM_INITDUTY) - 1);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_1, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);

    timer_primary_output_config(TIMER0, ENABLE);

    timer_auto_reload_shadow_enable(TIMER0);
    timer_enable(TIMER0);

    timer_deinit(TIMER1);
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler = 107;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = (1000 / TIM_FREQUENCY) - 1;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_init(TIMER1, &timer_initpara);

    timer_interrupt_enable(TIMER1, TIMER_INT_UP);
    timer_enable(TIMER1);
    nvic_irq_enable(TIMER1_IRQn, TIM_PRIORITY, 0);
}
