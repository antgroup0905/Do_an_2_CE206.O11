#ifndef PHSENSOR_H
#define PHSENSOR_H

#include <Arduino.h>

class pHSensor
{
public:
    pHSensor(int pin, int interval, int arrayLength, float offset);
    float getpHValue();

private:
    int pHPin;
    unsigned long samplingInterval;
    int ArrayLength;
    float Offset;
    unsigned long samplingTime;
    float *pHArray;
    int pHArrayIndex;
    float averageArray(float *arr, int number);
};

#endif // PHSENSOR_H