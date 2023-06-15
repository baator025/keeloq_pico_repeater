#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/uart.h"

#include "ReceiverService.hpp"
#include "TransmissionService.hpp"
#include "TransmissionConfig.hpp"

int main(){
    TransmissionService transmissionService(COMM_PIO, COMM_SM, COMM_PIN, TE_PERIOD);
    transmissionService.initializePio();
    static ReceiverService receiverService();

    // uint32_t data[FRAME_SIZE] {0xAAAAAAAA, 0xCCCCCCCC};
    uint32_t data[FRAME_SIZE] {0x045A0ED4, 0x35B95FFB};
    uint16_t transmissionCounter {0};

    while(true){
        if(transmissionCounter == TRANSMISSION_TRESHOLD){
            transmissionService.sendFrame(data);
            transmissionCounter = 0;
        } else transmissionCounter++;
        sleep_ms(1);
    }
    
}