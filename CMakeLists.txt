cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
project(pilot_pio C CXX ASM)

pico_sdk_init()

add_executable(pilot_pio
        main.cpp
        TransmissionService.cpp
        ReceiverService.cpp
        UARTDebug.cpp
        )

# pull in common dependencies
target_link_libraries(pilot_pio pico_stdlib hardware_pio)

pico_generate_pio_header(pilot_pio ${CMAKE_CURRENT_LIST_DIR}/keeloq_tx.pio)
pico_generate_pio_header(pilot_pio ${CMAKE_CURRENT_LIST_DIR}/keeloq_rx.pio)

target_sources(pilot_pio PRIVATE main.cpp TransmissionService.cpp ReceiverService.cpp)

# create map/bin/hex file etc.
pico_add_extra_outputs(pilot_pio)

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)
