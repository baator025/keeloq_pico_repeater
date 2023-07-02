#ifndef UART_DEBUG
#define UART_DEBUG

#include <cstdlib>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/uart.h"

class UARTDebug
{
private:
    uart_inst_t * const uart;
    const uint baudrate;
    const uint rxPin;
    const uint txPin;
public:
    UARTDebug(uart_inst_t *_uart, uint _baudrate, uint _rxPin, uint _txPin);
    void initialize();
    void sendData(int data);
};

#endif //UART_DEBUG