#ifndef RECEIVER_SERVICE_HPP
#define RECEIVER_SERVICE_HPP

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "TransmissionConfig.hpp"

#include "keeloq_rx.pio.h"

class ReceiverService
{
private:
    const PIO pio;
    const uint sm;
    const uint pin_tx;
    const uint TEPeriod;
    uint offset;
public:
    ReceiverService();
    ~ReceiverService();
};

#endif //RECEIVER_SERVICE_HPP