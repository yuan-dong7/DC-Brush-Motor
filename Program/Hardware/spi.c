#include "spi.h"
#include "gd32f10x.h"

#define SPIX SPI0

unsigned short SPI_ReadWriteHalfWord(unsigned short data) {
    unsigned short buffer;
    while (RESET == spi_i2s_flag_get(SPIX, SPI_FLAG_TBE));
    spi_i2s_data_transmit(SPIX, data);
    while (RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE));
    buffer = spi_i2s_data_receive(SPIX);
    return buffer;
}

void SPI_Config(void) {
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_SPI0);

    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_7);
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

    spi_parameter_struct spi_init_struct;
    spi_i2s_deinit(SPIX);
    spi_struct_para_init(&spi_init_struct);

    spi_init_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode = SPI_MASTER;
    spi_init_struct.frame_size = SPI_FRAMESIZE_16BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss = SPI_NSS_SOFT;
    spi_init_struct.prescale = SPI_PSC_64;
    spi_init_struct.endian = SPI_ENDIAN_MSB;
    spi_init(SPIX, &spi_init_struct);
    spi_nss_internal_high(SPIX);
    spi_enable(SPIX);
    spi_crc_off(SPIX);
}
