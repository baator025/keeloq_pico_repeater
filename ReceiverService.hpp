#ifndef RECEIVER_SERVICE_HPP
#define RECEIVER_SERVICE_HPP

#include <array>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "TransmissionConfig.hpp"

#include "keeloq_rx.pio.h"

class ReceiverService
{
private:
    const PIO pio;
    const uint sm;
    const uint pinRx;
    const uint probingPeriod;
    uint offset;
    const uint sideSetPin;
    void loadBitCounterValue();          //it has to be used after readout                                                                   //-DEBUG-
public:
    ReceiverService(PIO pio, uint sm, uint pinRx, uint probingPeriod, uint sideSetPin);      //-DEBUG-
    void initializePIO();
    std::array<uint32_t, FRAME_SIZE + 1> readData();
    uint getFIFOLevel();
};

#endif //RECEIVER_SERVICE_HPP