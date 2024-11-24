#include <pico/stdlib.h>
#include <math.h>
#include <string.h>
#include <wd_spi.h>
#include "spi_main_com.pio.h"

uint program_offset(const SpiStructTypeDef *spi) {
    uint prog_offs;
    switch (spi->mode) {
    default:
    case 0:
        prog_offs = pio_add_program(spi->pio, &spi_main_0_program);
        break;
    case 1:
        prog_offs = pio_add_program(spi->pio, &spi_main_1_program);
        break;
    case 2:
        prog_offs = pio_add_program(spi->pio, &spi_main_2_program);
        break;
    case 3:
        prog_offs = pio_add_program(spi->pio, &spi_main_3_program);
        break;
    }
    return(prog_offs);
}

void spi_init(const SpiStructTypeDef *spi) {
    spi_initialize(spi->pio,
                   spi->sm,
                   program_offset(spi),
                   spi->mode,
                   spi->n_bits,
                   spi->clkdiv,
                   spi->pin_sck,
                   spi->pin_mosi,
                   spi->pin_miso);
}

void __time_critical_func(spi_write_read_blocking)(const SpiStructTypeDef *spi, void *src, void *dst,
                                                   BitEnumTypeDef com_bits, size_t buff_size) {
    size_t tx_remain = buff_size, rx_remain = buff_size;
    UnionTxDataTypeDef tx_data_union;
    UnionRxDataTypeDef rx_data_union;
    UnionBufferTypeDef tx_buffer_union;
    UnionBufferTypeDef rx_buffer_union;

    switch (com_bits) {
    default:
    case SPI_8:
        tx_data_union.tx_8 = (io_rw_8 *) &spi->pio->txf[spi->sm];
        rx_data_union.rx_8 = (io_rw_8 *) &spi->pio->rxf[spi->sm];
        tx_buffer_union.buffer_8 = (uint8_t*)src;
        rx_buffer_union.buffer_8 = (uint8_t*)dst;
        break;
    case SPI_16:
        tx_data_union.tx_16 = (io_rw_16 *) &spi->pio->txf[spi->sm];
        rx_data_union.rx_16 = (io_rw_16 *) &spi->pio->rxf[spi->sm];
        tx_buffer_union.buffer_16 = (uint16_t*)src;
        rx_buffer_union.buffer_16 = (uint16_t*)dst;
        break;
    case SPI_32:
        tx_data_union.tx_32 = (io_rw_32 *) &spi->pio->txf[spi->sm];
        rx_data_union.rx_32 = (io_rw_32 *) &spi->pio->rxf[spi->sm];
        tx_buffer_union.buffer_32 = (uint32_t*)src;
        rx_buffer_union.buffer_32 = (uint32_t*)dst;
        break;
    }

    while (tx_remain || rx_remain) {
        if (tx_remain && !pio_sm_is_tx_fifo_full(spi->pio, spi->sm)) {
            switch (com_bits) {
            default:
            case SPI_8:
                *tx_data_union.tx_8 = *tx_buffer_union.buffer_8++;
                break;
            case SPI_16:
                *tx_data_union.tx_16 = *tx_buffer_union.buffer_16++;
                break;
            case SPI_32:
                *tx_data_union.tx_32 = *tx_buffer_union.buffer_32++;
                break;
            }
            --tx_remain;
        }
        if (rx_remain && !pio_sm_is_rx_fifo_empty(spi->pio, spi->sm)) {
            switch (com_bits) {
            default:
            case SPI_8:
                *rx_buffer_union.buffer_8++ = *rx_data_union.rx_8;
                break;
            case SPI_16:
                *rx_buffer_union.buffer_16++ = *rx_data_union.rx_16;
                break;
            case SPI_32:
                *rx_buffer_union.buffer_32++ = *rx_data_union.rx_32;
                break;
            }
            --rx_remain;
        }
    }
}
