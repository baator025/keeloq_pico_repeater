#ifndef DISPLAY_MANAGER_HPP
#define DISPLAY_MANAGER_HPP

#include "hardware/i2c.h"
#include "hardware/pio.h"
#include <cstdio>

extern "C" {
    #include "SSD1306.h"
}

class DisplayManager
{
private:
    i2c_inst_t* i2cInstance;
    const uint baudrate;
    const uint SDAPin;
    const uint SCLPin;

    uint16_t displayUpdateCounter {};
    uint8_t dotUpdateCounter {};
    uint8_t dotPosition {};
    char testStr[20] {};

    static constexpr uint16_t DISPLAY_UPDATE_COUNTER_VAL {500};
    char PROGRESS_DOT_SIGN[4] {" . "};
    char PROGRESS_CLEAR_SIGN[4] {"   "};

public:
    DisplayManager(i2c_inst_t* _i2cInstance, const uint _baudrate, const uint _SDAPin, const uint _SCLPin);
    void print_string(char* string, uint8_t length, uint8_t start_x, uint8_t start_y);
    void clearDisplay();
    void initializeInterface();
    void initializeDisplay();
    void runDisplayManagerCycle(uint8_t dataCounter);
};


#endif //DISPLAY_MANAGER_HPP