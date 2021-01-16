#include "gd32f10x.h"
#include "system.h"

volatile static uint32_t delay;

void System_Config(void) {
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    RCU_CTL |= RCU_CTL_HXTALEN;
    do {
        timeout++;
        stab_flag = (RCU_CTL & RCU_CTL_HXTALSTB);
    } while ((0U == stab_flag) && (HXTAL_STARTUP_TIMEOUT != timeout));
    while (0U == (RCU_CTL & RCU_CTL_HXTALSTB));
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV2;
    RCU_CFG0 &= ~(RCU_CFG0_PLLSEL | RCU_CFG0_PREDV0);
    RCU_CFG0 |= (RCU_PLLSRC_HXTAL | RCU_CFG0_PREDV0);
    RCU_CFG0 &= ~(RCU_CFG0_PLLMF | RCU_CFG0_PLLMF_4);
    RCU_CFG0 |= RCU_PLL_MUL27;
    RCU_CTL |= RCU_CTL_PLLEN;
    while (0U == (RCU_CTL & RCU_CTL_PLLSTB));
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLL;
    while (0U == (RCU_CFG0 & RCU_SCSS_PLL));
    while (SysTick_Config(SystemCoreClock / 1000U));
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
}

void Delayus(unsigned int xus) {
    unsigned int Ticks, Time_Old, Time_Now, Time_Count = 0;
    unsigned int Reload = SysTick->LOAD;
    Ticks = xus * 108;
    Time_Old = SysTick->VAL;
    while (1) {
        Time_Now = SysTick->VAL;
        if (Time_Now != Time_Old) {
            if (Time_Now < Time_Old)
                Time_Count += Time_Old - Time_Now;
            else
                Time_Count += Reload - Time_Now + Time_Old;
            Time_Old = Time_Now;
            if (Time_Count >= Ticks)
                break;
        }
    }
}

void Delayms(uint32_t count) {
    delay = count;
    while (0U != delay);
}

void delay_decrement(void) {
    if (0U != delay) {
        delay--;
    }
}

unsigned short FloatToInt16(float data0) {
    if (data0 == 0)
        return 0;
    unsigned short fInt16;
    unsigned int *pfp32 = ((unsigned int *) &data0);
    fInt16 = ((*pfp32 >> 16) & 0x8000) |
        ((((*pfp32 >> 23) - 0x7f + 0x0f) & 0x1f) << 10) |
        ((*pfp32 >> 13) & 0x3ff);
    return fInt16;
}

float Int16ToFloat(unsigned short data0) {
    if (data0 == 0)
        return 0;
    float *fp32;
    unsigned int fInt32 = ((data0 & 0x8000) << 16) |
        (((((data0 >> 10) & 0x1f) - 0x0f + 0x7f) & 0xff) << 23)
        | ((data0 & 0x03FF) << 13);
    fp32 = (float *) &fInt32;
    return *fp32;
}
