#ifndef TRANSMISSION_SERVICE_HPP
#define TRANSMISSION_SERVICE_HPP

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "TransmissionConfig.hpp"

#include "keeloq_tx.pio.h"

class TransmissionService
{
private:
    const PIO pio;
    const uint sm;
    const uint pin_tx;
    const uint TEPeriod;
    uint offset;
public:
    TransmissionService(const PIO pio, const uint sm, const uint pin_tx, const uint TEPeriod);
    void initializePio();
    uint8_t sendFrame(uint32_t *data);
    ~TransmissionService();
};

#endif //TRANSMISSION_SERVICE_HPP