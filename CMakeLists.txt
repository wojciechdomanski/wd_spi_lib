cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(wd_spi_example C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_subdirectory(wd_spi)

add_executable(spi_example spi_example.c)

pico_enable_stdio_uart(spi_example 1)
pico_enable_stdio_usb(spi_example 0)

pico_add_extra_outputs(spi_example)

target_compile_options(spi_example PRIVATE -Wall)

target_link_libraries(spi_example wd_spi pico_stdlib)