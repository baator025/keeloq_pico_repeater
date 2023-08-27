#ifndef TRANSMISSION_SERVICE_HPP
#define TRANSMISSION_SERVICE_HPP

#include <array>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "TransmissionConfig.hpp"

#include "keeloq_tx.pio.h"

class TransmissionService
{
public:
    TransmissionService(const PIO pio, const uint sm, const uint pin_tx, const uint TEPeriod);
    using DataFrame = std::array<uint32_t, FRAME_SIZE>;
    void initializePio();
    void sendFrame(DataFrame data);
private:
    const PIO pio;
    const uint sm;
    const uint pin_tx;
    const uint TEPeriod;
    uint offset;
};

#endif //TRANSMISSION_SERVICE_HPP