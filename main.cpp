#include <array>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/uart.h"

#include "ReceiverService.hpp"
#include "TransmissionService.hpp"
#include "TransmissionConfig.hpp"
#include "BasicPeripherals.hpp"
#include "UARTDebug.hpp"


int main(){
    static TransmissionService transmissionService(TX_PIO, TX_SM, TX_PIN, TE_PERIOD);
    transmissionService.initializePio();
    static ReceiverService receiverService{RX_PIO, RX_SM, RX_PIN, PROBING_PERIOD, SIDE_SET_PIN};        //-DEBUG-
    receiverService.initializePIO();

    static constexpr uint LEDPin {PICO_DEFAULT_LED_PIN};
    static constexpr uint period {200U};
    LEDNotifier ledNotifier{LEDPin, period};
    ledNotifier.init();
    ledNotifier.notifyBlocking(2);

    static UARTDebug uartService(uart0, 115200, 1, 0);
    uartService.initialize();

    uint16_t transmissionCounter {0};
    ReceiverService::KeeloqRawFrame receivedData {};
    bool transmitFlag = false;

    auto packData = [](ReceiverService::KeeloqRawFrame receivedData){
                            TransmissionService::DataFrame data {receivedData[1], receivedData[2]};
                            return(data);};

    Button button {BUTTON_PIN};
    button.init();

    static std::array<TransmissionService::DataFrame, 100> dataLog {};
    static uint8_t dataCounter {};

    auto storeData = [](TransmissionService::DataFrame data){
                            dataLog[dataCounter] = data;
                            dataCounter++;};

    auto popData = [](){dataCounter--; return(dataLog[dataCounter+1]);};

    while(true){
        ReceiverStatus receiverStatus = receiverService.checkInterrupt();
        switch (receiverStatus){
        case ReceiverStatus::DATA_READY:
            if(receiverService.readData(receivedData)){
                for(auto i : receivedData){
                    uartService.sendData(i);
                }
                receiverService.clearInterrupt(ReceiverInterrupts::DATA_READY_IRQ);
                auto data = packData(receivedData);
                storeData(data);
                
                ledNotifier.notifyBlocking(1);
            } else {
                receiverService.faultsHandling(ReceiverErrors::PARSING_ERROR);
            }
            break;

        case ReceiverStatus::TRANSMISSION_ERROR:
            receiverService.faultsHandling(ReceiverErrors::PIO_ERROR);

        case ReceiverStatus::WAITING_FOR_DATA:
            break;
        default:
            break;
        }

        button.buttonStateMachine();
        if(button.wasPressed()){
            receiverService.lockReceiver();
            transmissionService.sendFrame(popData());
            sleep_ms(100);
            receiverService.unlockReceiver();
        };

        sleep_ms(1);
    }                            
}

// int main(){
//     static TransmissionService transmissionService(TX_PIO, TX_SM, TX_PIN, TE_PERIOD);
//     transmissionService.initializePio();
//     static ReceiverService receiverService{RX_PIO, RX_SM, RX_PIN, PROBING_PERIOD, SIDE_SET_PIN};        //-DEBUG-
//     receiverService.initializePIO();

//     static constexpr uint LEDPin {PICO_DEFAULT_LED_PIN};
//     static constexpr uint period {200U};
//     LEDNotifier ledNotifier{LEDPin, period};
//     ledNotifier.init();
//     ledNotifier.notifyBlocking(2);

//     static UARTDebug uartService(uart0, 115200, 1, 0);
//     uartService.initialize();

//     uint16_t transmissionCounter {0};
//     ReceiverService::KeeloqRawFrame receivedData {};
//     bool transmitFlag = false;

//     auto packData = [](ReceiverService::KeeloqRawFrame receivedData){
//                             TransmissionService::DataFrame data {receivedData[1], receivedData[2]};
//                             return(data);};

//     while(true){
//         ReceiverStatus receiverStatus = receiverService.checkInterrupt();
//         switch (receiverStatus){
//         case ReceiverStatus::DATA_READY:
//             if(receiverService.readData(receivedData)){
//                 for(auto i : receivedData){
//                     uartService.sendData(i);
//                 }
//                 receiverService.clearInterrupt(ReceiverInterrupts::DATA_READY_IRQ);
//                 auto data = packData(receivedData);
//                 transmissionService.sendFrame(data);
//                 ledNotifier.notifyBlocking(1);
//             } else {
//                 receiverService.faultsHandling(ReceiverErrors::PARSING_ERROR);
//             }
//             break;

//         case ReceiverStatus::TRANSMISSION_ERROR:
//             receiverService.faultsHandling(ReceiverErrors::PIO_ERROR);

//         case ReceiverStatus::WAITING_FOR_DATA:
//             break;
//         default:
//             break;
//         }
//         sleep_ms(1);
//     }
    
// }
