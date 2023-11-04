#include "capteur.hpp"

Detector::Detector()
{
    DDRA = ~(1 << PORTA2);
    for(uint8_t i = 0; i < 5; i++){
        // lastFiveDist[i] = 0;
    }
}

uint16_t Detector::getDistanceCm()
{
    uint8_t pos = 2;
    uint16_t valueDetector = ((detectorCan.lecture(pos)));
    // double result = pow(valueDetector, exp);
    // return static_cast<uint16_t>(result * formulaConstant);
    return valueDetector;
}

uint16_t Detector::getAverageDistance()
{
        distance1 = getDistanceCm();
        _delay_ms(10);
        distance2 = getDistanceCm();
        _delay_ms(10);
        distance3 = getDistanceCm();
        _delay_ms(10);
        distance4 = getDistanceCm();

        return (distance1 + distance2 + distance3 + distance4)/4;
}

uint16_t Detector::getMedianeDistance()
{
    for(uint8_t i = 0 ; i < 5 ; i++)
    {
        lastFiveDist[i+1] = lastFiveDist[i];
    }
    lastFiveDist[0] = getDistanceCm();
    _delay_ms(40);

    uint16_t temp[5];
    for(uint8_t i = 0; i < 5; i++){
        lastFiveDistSorted[i] = lastFiveDist[i];
    }
    for (int i = 0; i < 5 - 1; i++) {
        for (int j = 0; j < 5 - i - 1; j++) {
            if (lastFiveDistSorted[j] > lastFiveDistSorted[j + 1]) {
                int temp = lastFiveDistSorted[j];
                lastFiveDistSorted[j] = lastFiveDistSorted[j + 1];
                lastFiveDistSorted[j + 1] = temp;
            }
        }
    }
    return lastFiveDistSorted[1];

}