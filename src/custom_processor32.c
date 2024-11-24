// Wokwi Custom Chip - For docs and examples see:
// https://docs.wokwi.com/chips-api/getting-started
//
// SPDX-License-Identifier: MIT
// Copyright 2024 Wojciech Domański

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 4

typedef struct {
  pin_t cs_pin;
  uint32_t spi;
  uint8_t  spi_buffer[BUF_SIZE];
} chip_state_t;

static void chip_pin_change(void *user_data, pin_t pin, uint32_t value);
static void chip_spi_done(void *user_data, uint8_t *buffer, uint32_t count);

void chip_init() {
  chip_state_t *chip = malloc(sizeof(chip_state_t));

  chip->cs_pin = pin_init("CS", INPUT_PULLUP);

  const pin_watch_config_t watch_config = {
    .edge = BOTH,
    .pin_change = chip_pin_change,
    .user_data = chip,
  };
  pin_watch(chip->cs_pin, &watch_config);

  const spi_config_t spi_config = {
  .sck = pin_init("SCK", INPUT),
  .mosi = pin_init("MOSI", INPUT),
  .miso = pin_init("MISO", INPUT),
  .mode = 3,
  .done = chip_spi_done,
  .user_data = chip,
  };
  chip->spi = spi_init(&spi_config);

  printf("Hello from custom chip!\n");
}

void chip_pin_change(void *user_data, pin_t pin, uint32_t value) {
  chip_state_t *chip = (chip_state_t*)user_data;
  // Handle CS pin logic
  if (pin == chip->cs_pin) {
    if (value == LOW) {
      printf("SPI chip selected\n");
      for(int i = 0; i < BUF_SIZE; i++) {
        chip->spi_buffer[i] = ' '; // Some dummy data for the first character
      }
      spi_start(chip->spi, chip->spi_buffer, sizeof(chip->spi_buffer));
    } else {
      printf("SPI chip deselected\n");
      spi_stop(chip->spi);
    }
  }
}

void chip_spi_done(void *user_data, uint8_t *buffer, uint32_t count) {
  printf("Count value %d\n", count);
  chip_state_t *chip = (chip_state_t*)user_data;
  if (!count) {
    // This means that we got here from spi_stop, and no data was received
    return;
  }

  // Modify input from master
  for(int i = 0; i < BUF_SIZE; i++) {
    printf("Buffer: %02x\n", buffer[i]);
    buffer[i] = buffer[i] + 1;
  }
  printf("\n");

  // buffer[1] = modify(buffer[1]);
  if (pin_read(chip->cs_pin) == LOW) {
    // Continue with the next character
    spi_start(chip->spi, chip->spi_buffer, sizeof(chip->spi_buffer));
  }
}
