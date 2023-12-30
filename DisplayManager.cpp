#include "DisplayManager.hpp"

DisplayManager::DisplayManager(i2c_inst_t* _i2cInstance, const uint _baudrate, const uint _SDAPin, const uint _SCLPin) :
                                i2cInstance(_i2cInstance), baudrate{_baudrate}, SDAPin{_SDAPin}, SCLPin{_SCLPin} 
                                {
                                    testStr[19] = '\0';
                                }


void DisplayManager::initializeInterface(){
    i2c_init(i2cInstance, baudrate);
    gpio_set_function(SDAPin, GPIO_FUNC_I2C);
    gpio_set_function(SCLPin, GPIO_FUNC_I2C);
    gpio_pull_up(SDAPin);
    gpio_pull_up(SCLPin);

    uint8_t data = 0xFF;
    i2c_write_blocking(i2cInstance, 0x08, &data, sizeof(data), false);
}

void DisplayManager::initializeDisplay(){
    SSD1306_init(32);
}

void DisplayManager::clearDisplay(){
    // clear_display();
    char clearString[128]
    {"                                                                                                                              "};
    print_string(clearString, sizeof(clearString), 0, 0);
    print_string(clearString, sizeof(clearString), 0, 8);
    print_string(clearString, sizeof(clearString), 0, 16);
    print_string(clearString, sizeof(clearString), 0, 24);
}

void DisplayManager::print_string(char* string, uint8_t length, uint8_t start_x, uint8_t start_y){
    print(string, length, start_x, start_y);
}

void DisplayManager::runDisplayManagerCycle(uint8_t dataCounter)
{
    if(displayUpdateCounter >= DISPLAY_UPDATE_COUNTER_VAL)
    {
        snprintf(testStr, sizeof(testStr), "data left: %i", dataCounter);
        print_string(testStr, sizeof(testStr), 0, 0);

        if(dotUpdateCounter >= 1)
        {
            print_string(PROGRESS_CLEAR_SIGN, sizeof(PROGRESS_CLEAR_SIGN), dotPosition, 22);
            dotPosition += 5;
            if(dotPosition >= 127) dotPosition = 0;
            print_string(PROGRESS_DOT_SIGN, sizeof(PROGRESS_DOT_SIGN), dotPosition, 22);
            dotUpdateCounter = 0;
        }

        dotUpdateCounter++;
        displayUpdateCounter = 0;
    }
    displayUpdateCounter++;
}
