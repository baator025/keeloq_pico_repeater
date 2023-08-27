#ifndef CONFIG_HPP
#define CONFIG_HPP


#include "hardware/pio.h"

#define TX_PIN 17U
#define TX_PIO pio0
#define TX_SM 0U
#define TE_PERIOD 2622U                 //not exactly a period, its a frequency allowing 
                                        //to send 1 pulse of 1 TE width

#define FRAME_SIZE 2U                   //size of frame in uint32s
#define TRANSMISSION_TRESHOLD 1500U
#define RECEIVER_TRESHOLD 2000U
#define COUNTER_LIMIT 2001U

#define RX_PIN 3U
#define RX_PIO pio1
#define RX_SM 0U
#define PROBING_PERIOD 2622U            //the same as above (original val 2500)
#define SIDE_SET_PIN 4U                 //-DEBUG-

#define BUTTON_PIN 16U


#endif //CONFIG_HPP