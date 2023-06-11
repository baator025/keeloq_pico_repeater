#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "hardware/pio.h"

#define COMM_PIN 2U
#define COMM_PIO pio0
#define COMM_SM 0U
#define TE_PERIOD 2500U

#define FRAME_SIZE 2U    //size of frame in uint32s
#define TRANSMISSION_TRESHOLD 1500U

#endif //CONFIG_HPP