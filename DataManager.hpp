#include "TransmissionService.hpp"
#include <cstdio>

class DataManager
{
public:
    void storeData(TransmissionService::DataFrame data);
    TransmissionService::DataFrame popData(void);
    uint8_t getDataCounter();
private:
    std::array<TransmissionService::DataFrame, 100> dataLog {};
    uint8_t dataCounter {};
};