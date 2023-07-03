#include <array>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/uart.h"

#include "ReceiverService.hpp"
#include "TransmissionService.hpp"
#include "TransmissionConfig.hpp"
#include "UARTDebug.hpp"

int main(){
    TransmissionService transmissionService(TX_PIO, TX_SM, TX_PIN, TE_PERIOD);
    transmissionService.initializePio();
    static ReceiverService receiverService{RX_PIO, RX_SM, RX_PIN, PROBING_PERIOD, SIDE_SET_PIN};        //-DEBUG-
    receiverService.initializePIO();

    static UARTDebug uartService(uart0, 115200, 1, 0);
    uartService.initialize();
    uartService.sendData(2137);

    uint16_t transmissionCounter {0};
    uint32_t data[FRAME_SIZE] {0x045A0ED4, 0x35B95FFB};
    std::array<uint32_t, FRAME_SIZE + 1> receivedData {};

    while(true){
        if(transmissionCounter == TRANSMISSION_TRESHOLD){
            transmissionService.sendFrame(data);
        } else if(transmissionCounter == RECEIVER_TRESHOLD){
            uartService.sendData(receiverService.getFIFOLevel());
            receivedData = receiverService.readData();
            for(auto it : receivedData){
                uartService.sendData(it);
            }
        } else if(transmissionCounter == COUNTER_LIMIT){
            transmissionCounter = 0;
        }
        
        transmissionCounter++;
        sleep_ms(1);
    }
    
}