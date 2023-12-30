#include "DataManager.hpp"


void DataManager::storeData(TransmissionService::DataFrame data)
{
    if(data != dataLog[dataCounter - 1]){
        dataLog[dataCounter] = data;
        dataCounter++;
    }
}


TransmissionService::DataFrame DataManager::popData(void)
{
    TransmissionService::DataFrame tempData {dataLog[0]};
    for(uint8_t i = 0; i < dataCounter; i++)
    {
        dataLog[i] = dataLog[i+1];
    }
    dataCounter--;
    return(tempData);
}


uint8_t DataManager::getDataCounter()
{
    return(dataCounter);
}