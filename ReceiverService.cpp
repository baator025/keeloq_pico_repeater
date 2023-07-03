#include "ReceiverService.hpp"


ReceiverService::ReceiverService(PIO pio, uint sm, uint pinRx, uint probingPeriod, uint sideSetPin) :           //-DEBUG-
                                pio{pio}, 
                                sm{sm},
                                pinRx{pinRx},
                                probingPeriod{probingPeriod},
                                sideSetPin(sideSetPin) {}                                                       //-DEBUG-

void ReceiverService::initializePIO(){
    offset = pio_add_program(pio, &keeloq_rx_program);
    keeloq_rx_program_init(pio, sm, offset, pinRx, probingPeriod, sideSetPin);                                  //-DEBUG-
    loadBitCounterValue();
}

void ReceiverService::loadBitCounterValue(){           //it has to be used after readout
    pio_sm_put_blocking(pio, sm, 64);
}

std::array<uint32_t, FRAME_SIZE + 1> ReceiverService::readData(){
    std::array<uint32_t, FRAME_SIZE + 1> rawData {};
    for(auto & it : rawData){
        it = pio_sm_get_blocking(pio, sm);
    }
    loadBitCounterValue();
    return(rawData);
}

uint ReceiverService::getFIFOLevel(){
    return(pio_sm_get_rx_fifo_level(pio, sm));
}