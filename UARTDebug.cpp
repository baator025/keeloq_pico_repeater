#include "UARTDebug.hpp"


UARTDebug::UARTDebug(uart_inst_t *_uart, uint _baudrate, uint _rxPin, uint _txPin)
                        :uart(_uart), baudrate(_baudrate), rxPin(_rxPin), txPin(_txPin) {};
    

void UARTDebug::initialize(){
    uart_init(uart, baudrate);
    gpio_set_function(txPin, GPIO_FUNC_UART);
    gpio_set_function(rxPin, GPIO_FUNC_UART);
}

void UARTDebug::sendData(int data){
    char buffer[34] {};
    itoa(data, buffer, 16);
    buffer[32] = '\n';
    uart_puts(uart, buffer);
}