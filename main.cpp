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

    uint16_t transmissionCounter {0};
    uint32_t data[FRAME_SIZE] {0x045A0ED4, 0x35B95FFB};
    KeeloqRawFrame receivedData {};
    bool transmitFlag = false;

    while(true){
        ReceiverStatus receiverStatus = receiverService.checkInterrupt();
        switch (receiverStatus){
        case ReceiverStatus::DATA_READY:
            if(receiverService.readData(receivedData)){
                for(auto i : receivedData){
                    uartService.sendData(i);
                }
                receiverService.clearInterrupt(ReceiverInterrupts::DATA_READY_IRQ);
                //transmit data
            } else {
                receiverService.faultsHandling(ReceiverErrors::PARSING_ERROR);
            }
            break;

        case ReceiverStatus::TRANSMISSION_ERROR:
            receiverService.faultsHandling(ReceiverErrors::PIO_ERROR);

        case ReceiverStatus::WAITING_FOR_DATA:
            break;
        default:
            break;
        }
        sleep_ms(1);
    }
    
}
