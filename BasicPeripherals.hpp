#ifndef LED_NOTIFIER_
#define LED_NOTIFIER_

#include "pico/stdlib.h"

class LEDNotifier
{
public:
    LEDNotifier(uint _pin, uint _period);
    void init();
    void notifyBlocking(uint8_t blinks);
private:
    const uint LEDPin;
    const uint period;
};

enum ButtonStatus{
    NOT_PRESSED,
    PRESSED,
    WAS_PRESSED
};

class Button
{
public:
    Button(uint _pin);
    void init();
    bool wasPressed();
    void buttonStateMachine();
private:
    const uint buttonPin {};
    ButtonStatus buttonStatus = NOT_PRESSED;
    inline bool getStatus()
        {return(gpio_get(buttonPin));};
};

#endif //LED_NOTIFIER_
