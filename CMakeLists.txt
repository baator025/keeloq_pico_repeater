cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
project(pilot_pio C CXX ASM)

set(SSD1306_LIB_LOCATION "/Users/bartoszbator/Documents/libs/SSD1306_library")
set(SSD1306_BINARY_LOCATION ${CMAKE_CURRENT_LIST_DIR}/build/ssd1306)
add_subdirectory(${SSD1306_LIB_LOCATION} ${SSD1306_BINARY_LOCATION})

pico_sdk_init()

add_executable(pilot_pio main.cpp)

# pull in common dependencies
target_include_directories(pilot_pio PUBLIC ${SSD1306_LIB_LOCATION} ${SSD1306_BINARY_LOCATION})
target_link_directories(pilot_pio PUBLIC ${SSD1306_LIB_LOCATION} ${SSD1306_BINARY_LOCATION})
# find_library(SSD1306_LIB ssd1306 HINTS ${SSD1306_LIB_LOCATION})
target_link_libraries(pilot_pio PUBLIC pico_stdlib hardware_pio hardware_i2c ssd1306)

pico_generate_pio_header(pilot_pio ${CMAKE_CURRENT_LIST_DIR}/keeloq_tx.pio)
pico_generate_pio_header(pilot_pio ${CMAKE_CURRENT_LIST_DIR}/keeloq_rx.pio)

target_sources(pilot_pio PRIVATE main.cpp
                                 TransmissionService.cpp
                                 ReceiverService.cpp
                                 UARTDebug.cpp
                                 BasicPeripherals.cpp
                                 DisplayManager.cpp
                                 DataManager.cpp)

# create map/bin/hex file etc.
pico_add_extra_outputs(pilot_pio)

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)
