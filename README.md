# Raspberry Pi Pico SPI communication driver
The intention of this project was to learn **C** coding by creating **SPI communication** drivers. It’s a PIO state machine based communication library that handles read and write of 8, 16 and 32 Bytes messages in four SPI modes.

This project makes use of the **Wokwi board simulator**[^1], which can be used in the **Visual Studio Code IDE**[^2].

## Setup

To build this library, you will need **CMake** and **Pico SDK**[^3]. Additionally, the `diagram.json`, `spi-processor.chip.json`, `custom_processor8.c`, `custom_processor16.c`, `custom_processor32.c` and `wokwi.toml` files are included as option for board simulation. Custom chip requires creating `.wasm` file out of `custom_processorX.c`. In order to do it you could use docker:

`docker run -v C:\Path\to\your\folder:/src wokwi/builder-clang-wasm clang --target=wasm32-unknown-wasi --sysroot /opt/wasi-libc -nostartfiles --% -Wl,--export-table -Wl,--no-entry -Werror -o spi-processor.chip.wasm src/custom_processor8.c`

## User manual

To use the driver follow these steps:
1. Define buffers for Tx and Rx.
2. Define SPI struct.
3. Define CS PIN as output.
4. Initialize SPI with struct defined earlier.
5. Set CS PIN to 0.
6. Start read/write SPI communication.
7. Set CS PIN to 1.

For more details please have a look at example file.

## Wiring

Based on Wokwi diagram

![wiring](https://i.imgur.com/al2FLt1.png)

| Cable | Custom Processor Pinout | Pico Pinout |
| :---: | :---: | :---: |
| Blue | SCK | GP6 |
| Yellow | MOSI | GP7 |
| Orange | MISO | GP8 |
| Green | CS | GP9 |

## References
[^1]: **Wokwi** https://docs.wokwi.com/?utm_source=wokwi
[^2]: **Visual Studio Code** https://code.visualstudio.com/
[^3]: **Pico SDK** https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf
