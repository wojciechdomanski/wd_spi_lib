add_library(wd_spi INTERFACE)

pico_generate_pio_header(wd_spi ${CMAKE_CURRENT_LIST_DIR}/spi_main_com.pio)

target_include_directories(wd_spi
    INTERFACE
    ./include
)

target_sources(wd_spi
    INTERFACE
    wd_spi.c
)

target_link_libraries(wd_spi
    INTERFACE
    hardware_clocks
    hardware_dma
    hardware_pio
)