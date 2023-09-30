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


bool ReceiverService::readData(KeeloqRawFrame& rawData){
    bool retVal = false;
    for(auto & it : rawData){
        it = pio_sm_get_blocking(pio, sm);
    }

    if(isFrameValid(rawData)){
        retVal = true;
        pio_sm_restart(pio, sm);
        pio_sm_clear_fifos(pio, sm);
        clearInterrupt(ReceiverInterrupts::DATA_READY_IRQ);
        bitloopIRQOccurenceCounter = 0;
        loadBitCounterValue();
    } else {
        retVal = false;
    }

    return(retVal);
}


uint ReceiverService::getFIFOLevel(){
    return(pio_sm_get_rx_fifo_level(pio, sm));
}


ReceiverStatus ReceiverService::checkInterrupt(){
    ReceiverInterrupts interruptID {};
    bool wasAnyInterruptReported {false};
    //need to get all of interrupts statuses, not only one.
    for(auto i : receiverInterruptsArray){
        if(pio_interrupt_get(pio, i)){
            interruptID = i;
            wasAnyInterruptReported = true;
            break;
        }
    }

    ReceiverStatus retVal {};
    uint8_t fifoLen = static_cast<uint8_t>(pio_sm_get_rx_fifo_level(pio, sm));
    if(wasAnyInterruptReported){
        switch(interruptID){
            case ReceiverInterrupts::DATA_READY_IRQ:
                if(fifoLen == FRAME_SIZE+1){
                    retVal = ReceiverStatus::DATA_READY;
                } else {
                    retVal = ReceiverStatus::TRANSMISSION_ERROR;
                }
                break;
            case ReceiverInterrupts::TRANSMISSION_ERROR_IRQ:
                retVal = ReceiverStatus::TRANSMISSION_ERROR;
                break;
            case ReceiverInterrupts::SM_BITLOOP_STARTED:
                if(bitloopIRQOccurenceCounter >= BITLOOP_IRQ_OCCURENCE_LIMIT){
                    retVal = ReceiverStatus::TOO_LONG_BITLOOP;
            
                } else {
                    bitloopIRQOccurenceCounter++;
                }
                break;
        }
    } else {
        retVal = ReceiverStatus::WAITING_FOR_DATA;
    }

    return(retVal);
}


void ReceiverService::clearInterrupt(uint interruptID){
    pio_interrupt_clear(pio, interruptID);
}


void ReceiverService::recoveryActions(){
    pio_sm_clear_fifos(pio, sm);    //TX FIFO will also be cleared
    for(auto interrupt : receiverInterruptsArray){
        clearInterrupt(interrupt);
    }
    pio_sm_restart(pio, sm);
    keeloq_rx_program_init(pio, sm, offset, pinRx, probingPeriod, sideSetPin);                                  //-DEBUG-
    bitloopIRQOccurenceCounter = 0;
    sleep_ms(3);
    loadBitCounterValue();
}


bool ReceiverService::isFrameValid(const KeeloqRawFrame& frame){
    bool retVal = false;

    if((frame[PREAMBLE_POS]&PREAMBLE_BITMASK) == PREAMBLE_BITMASK){
        for(auto fixedPart : allowedFixedParts){
            if(frame[FIXED_PART_POS] == fixedPart){
                retVal = true;
                break;
            } else {
                retVal = false;
            }
        }
    } else {
        retVal = false;
    }

    return(retVal);
}


void ReceiverService::lockReceiver(){
    pio_sm_restart(pio, sm);
}


void ReceiverService::unlockReceiver(){
    loadBitCounterValue();
}
