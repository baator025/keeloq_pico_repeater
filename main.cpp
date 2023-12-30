#include <array>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "cstdio"

#include "ReceiverService.hpp"
#include "TransmissionService.hpp"
#include "TransmissionConfig.hpp"
#include "BasicPeripherals.hpp"
#include "UARTDebug.hpp"
#include "DisplayManager.hpp"
#include "DataManager.hpp"

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

    static DataManager dataManager;

    static DisplayManager displayManager(i2c1, DSPL_BITRATE, DSPL_SDA_PIN, DSPL_SCL_PIN);
    displayManager.initializeInterface();
    displayManager.initializeDisplay();
    displayManager.clearDisplay();

    uint16_t transmissionCounter {0};
    ReceiverService::KeeloqRawFrame receivedData {};
    bool transmitFlag = false;

    auto packData = [](ReceiverService::KeeloqRawFrame receivedData){
                            TransmissionService::DataFrame data {receivedData[1], receivedData[2]};
                            return(data);};

    Button button {BUTTON_PIN};
    button.init();

    static TransmissionService::DataFrame currentData;
    static bool retransmissionOngoing {false};

    while(true){

        //Receiver state machine
        ReceiverStatus receiverStatus = receiverService.checkInterrupt();
        switch (receiverStatus){
        case ReceiverStatus::DATA_READY:
            if(receiverService.readData(receivedData)){
                for(auto i : receivedData){
                    uartService.sendData(i);
                }
                auto data = packData(receivedData);
                if(!retransmissionOngoing){
                    dataManager.storeData(data);
                } else {
                    if(data == currentData){
                        retransmissionOngoing = false;
                        ledNotifier.notifyBlocking(3);
                    }
                }

                ledNotifier.notifyBlocking(1);
            } else {
                receiverService.recoveryActions();
            }
            break;

        case ReceiverStatus::TRANSMISSION_ERROR:
            receiverService.recoveryActions();
            break;
        case ReceiverStatus::TOO_LONG_BITLOOP:
            receiverService.recoveryActions();
            break;

        case ReceiverStatus::WAITING_FOR_DATA:
            break;
        default:
            break;
        }

        //Transmitter state machine
        button.buttonStateMachine();
        if(button.wasPressed()){
            if(!retransmissionOngoing && dataManager.getDataCounter() > 0){
                currentData = dataManager.popData();
                retransmissionOngoing = true;
            }

            transmissionService.sendFrame(currentData);
        };

        displayManager.runDisplayManagerCycle(dataManager.getDataCounter());
        sleep_ms(1);
    }
}
