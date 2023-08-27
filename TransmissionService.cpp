#include "TransmissionService.hpp"

TransmissionService::TransmissionService(PIO pio, uint sm, uint pin_tx, uint TEPeriod) :
    pio(pio), sm(sm), pin_tx(pin_tx), TEPeriod(TEPeriod) {}

void TransmissionService::initializePio(){
    offset = pio_add_program(pio, &keeloq_tx_program);
    keeloq_tx_program_init(pio, sm, offset, pin_tx, TEPeriod);
}

void TransmissionService::sendFrame(DataFrame data){
    for(auto it : data){
        pio_sm_put_blocking(pio, sm, it);
    }
}

