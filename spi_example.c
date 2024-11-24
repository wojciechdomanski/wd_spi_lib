#include <stdlib.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "wd_spi.h"

#define STATE_MACHINE 0 // This driver uses 0 State Machine
#define SPI_MODE 3      // SPI mode to be used
#define COM_BITS SPI_8  // Message size (should be changed if message size is changed)
#define CLK_DIV 31.25   // 1 MHz @ 125 clk_sys
#define PIN_CS 9
#define PIN_SCK 6
#define PIN_MOSI 7
#define PIN_MISO 8
#define BUF_SIZE 8      // Message buffer size

int main() {
    static uint8_t txbuf[BUF_SIZE]; // should be changed if message size is changed (uint8_t, uint16_t, uint32_t)
    static uint8_t rxbuf[BUF_SIZE]; // should be changed if message size is changed (uint8_t, uint16_t, uint32_t)

    stdio_init_all();
    printf("Hello from Test Program!\n");

    const SpiStructTypeDef spi = {
    .pio = pio0,
    .sm = STATE_MACHINE,
    .mode = SPI_MODE,
    .n_bits = COM_BITS,
    .clkdiv = CLK_DIV, 
    .pin_cs = PIN_CS,
    .pin_sck = PIN_SCK,
    .pin_mosi = PIN_MOSI,
    .pin_miso = PIN_MISO
    };

    // Initialize CS GPIO and set to output
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, 1);

    // Generate Tx buffer
    printf("TX:");
    for (int i = 0; i < BUF_SIZE; ++i) {
        txbuf[i] = rand();
        rxbuf[i] = 0;
        printf(" %02x", (int) txbuf[i]);
    }
    printf("\n");

    spi_init(&spi);          // Initialize PIO
    gpio_put(PIN_CS, 0);     // Enable communication
    spi_write_read_blocking(&spi, txbuf, rxbuf, COM_BITS, BUF_SIZE);
    gpio_put(PIN_CS, 1);     // Disable communication

    printf("RX:");
    for (int i = 0; i < BUF_SIZE; ++i) {
        printf(" %02x", (int) rxbuf[i]);
    }
}
