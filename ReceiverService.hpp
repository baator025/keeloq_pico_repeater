#ifndef RECEIVER_SERVICE_HPP
#define RECEIVER_SERVICE_HPP

#include <array>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "TransmissionConfig.hpp"

#include "keeloq_rx.pio.h"

using fixedPayload = uint32_t;

enum ReceiverStatus{
    WAITING_FOR_DATA            = 0,
    DATA_READY                  = 1,
    TRANSMISSION_ERROR          = 2
};

enum ReceiverErrors{
    PIO_ERROR                   = 0,
    PARSING_ERROR               = 1
};

enum ReceiverInterrupts{
    DATA_READY_IRQ              = 0,
    TRANSMISSION_ERROR_IRQ      = 1,
    NONE_REPORTED               = 2,
    RX_IRQ_LENGTH               = 3
};

const std::array<ReceiverInterrupts, RX_IRQ_LENGTH> receiverInterruptsArray {
    ReceiverInterrupts::DATA_READY_IRQ,
    ReceiverInterrupts::TRANSMISSION_ERROR_IRQ };

using KeeloqRawFrame = std::array<uint32_t, FRAME_SIZE + 1>;

class ReceiverService
{
private:
    const PIO pio;
    const uint sm;
    const uint pinRx;
    const uint probingPeriod;
    uint offset;
    const uint sideSetPin;               //-DEBUG-

    static constexpr uint8_t NUMBER_OF_ALLOWED_DEVICES {1};
    static constexpr uint8_t PREAMBLE_POS {0};
    static constexpr uint8_t ENCRYPTED_PART_POS {1};
    static constexpr uint8_t FIXED_PART_POS {2};
    static constexpr uint32_t PREAMBLE_BITMASK {0x00000FFF};

    const std::array<fixedPayload, NUMBER_OF_ALLOWED_DEVICES> allowedFixedParts {0x35B95FFB};

    void loadBitCounterValue();
    bool isFrameValid(const KeeloqRawFrame& frame);
    uint getFIFOLevel();

public:
    ReceiverService(PIO pio, uint sm, uint pinRx, uint probingPeriod, uint sideSetPin);      //-DEBUG-
    void initializePIO();
    bool readData(KeeloqRawFrame& rawData);
    
    void clearInterrupt(uint interruptID);
    ReceiverStatus checkInterrupt();
    void faultsHandling(ReceiverErrors error);
    
};

#endif //RECEIVER_SERVICE_HPP