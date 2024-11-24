#ifndef SPI_H
#define SPI_H

#include <hardware/pio.h>
#include "spi_main_com.pio.h"

/**
 * \brief Main structure, used for defining all crucial parameters for SPI communication.
 */
typedef struct 
{
    PIO pio;
    uint8_t sm;
    uint8_t mode;
    uint8_t n_bits;
    uint8_t clkdiv;
    uint8_t pin_cs;
    uint8_t pin_sck;
    uint8_t pin_mosi;
    uint8_t pin_miso;
} SpiStructTypeDef;

/**
 * \brief Union for definition of PIO Tx register variables.
 */
typedef union {
    io_rw_8 *tx_8;
    io_rw_16 *tx_16;
    io_rw_32 *tx_32;
} UnionTxDataTypeDef;

/**
 * \brief Union for definition of PIO Rx register variables.
 */
typedef union {
    io_rw_8 *rx_8;
    io_rw_16 *rx_16;
    io_rw_32 *rx_32;
} UnionRxDataTypeDef;

/**
 * \brief Union for definition of buffer variables.
 */
typedef union {
    uint8_t *buffer_8;
    uint16_t *buffer_16;
    uint32_t *buffer_32;
} UnionBufferTypeDef;

/**
 * \brief Enumerator for choosing message size.
 */
typedef enum {
    SPI_8 = 8,
    SPI_16 = 16,
    SPI_32 = 32,
} BitEnumTypeDef;

/**
 * \brief Reads PIO program offset for specific SPI mode
 *
 * \param spi SPI driver structure.
 * \return Program offset.
 */
uint program_offset(const SpiStructTypeDef *spi);

/**
 * \brief Initializes PIO program
 *
 * \param spi SPI driver structure.
 */
void spi_init(const SpiStructTypeDef *spi);

/**
 * \brief Performs read and write SPI communication while blocking
 *
 * \param spi SPI driver structure.
 * \param src Tx buffer array.
 * \param dst Rx buffer array.
 * \param com_bits Message size.
 * \param buff_size Buffer size.
 * \return Program offset.
 */
void __time_critical_func(spi_write_read_blocking)(const SpiStructTypeDef *spi, void *src, void *dst,
                                                   BitEnumTypeDef com_bits, size_t buff_size);

#endif
