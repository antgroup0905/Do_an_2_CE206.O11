#include "pHSensor.h"

pHSensor::pHSensor(int pin, int interval, int arrayLength, float offset)
    : pHPin(pin), samplingInterval(interval), ArrayLength(arrayLength), Offset(offset), pHArrayIndex(0)
{
    samplingTime = millis();
    pHArray = new float[ArrayLength];
}

float pHSensor::averageArray(float *arr, int number)
{
    if (number <= 0)
    {
        Serial.println("Error: Invalid array size for averaging!");
        return 0;
    }

    long amount = 0;

    if (number < 5)
    {
        for (int i = 0; i < number; i++)
        {
            amount += arr[i];
        }
        return static_cast<float>(amount) / number;
    }
    else
    {
        float min, max;
        if (arr[0] < arr[1])
        {
            min = arr[0];
            max = arr[1];
        }
        else
        {
            min = arr[1];
            max = arr[0];
        }

        for (int i = 2; i < number; i++)
        {
            if (arr[i] < min)
            {
                amount += min;
                min = arr[i];
            }
            else if (arr[i] > max)
            {
                amount += max;
                max = arr[i];
            }
            else
            {
                amount += arr[i];
            }
        }

        return static_cast<float>(amount) / (number - 2);
    }
}

float pHSensor::getpHValue()
{
    if (millis() - samplingTime > samplingInterval)
    {
        pHArray[pHArrayIndex++] = analogRead(pHPin);
        if (pHArrayIndex == ArrayLength)
            pHArrayIndex = 0;

        float pHVoltage = averageArray(pHArray, ArrayLength) * 5.0 / 1024;
        float pHValue = 3.5 * pHVoltage + Offset;
        samplingTime = millis();
        return pHValue;
    }
    return -1; // Return -1 if not enough time has passed
}
