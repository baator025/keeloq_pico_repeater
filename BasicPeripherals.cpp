#include "BasicPeripherals.hpp"

LEDNotifier::LEDNotifier(uint _pin, uint _period):
                 LEDPin(_pin),
                 period(_period){};

void LEDNotifier::init(){
    gpio_init(LEDPin);
    gpio_set_dir(LEDPin, GPIO_OUT);
}

void LEDNotifier::notifyBlocking(uint8_t blinks){
    for(auto i = 0; i < blinks; i++){
        gpio_put(LEDPin, 1);
        sleep_ms(period);
        gpio_put(LEDPin, 0);
        sleep_ms(period);
    }
}

Button::Button(uint _pin):
                buttonPin(_pin){};

void Button::init(){
    gpio_init(buttonPin);
    gpio_pull_up(buttonPin);
    gpio_set_dir(buttonPin, GPIO_IN);
};

bool Button::wasPressed(){
    bool retVal {};
    if(buttonStatus == ButtonStatus::WAS_PRESSED){
        retVal = true;
        buttonStatus = ButtonStatus::NOT_PRESSED;
    } else {
        retVal = false;
    }
    return(retVal);
};

void Button::buttonStateMachine(){
    if(!getStatus()){
        buttonStatus = ButtonStatus::PRESSED;
    } else {
        if(buttonStatus == ButtonStatus::PRESSED){
            buttonStatus = ButtonStatus::WAS_PRESSED;
        } else {
            buttonStatus = ButtonStatus::NOT_PRESSED;
        }
    }
};
