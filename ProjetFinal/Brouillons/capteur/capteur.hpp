#include "can.h"
#include <avr/io.h>
#include <math.h>
#include <util/delay.h>
// #include <algorithm>

class Detector{
public:
    Detector();
    uint16_t getDistanceCm();
    uint16_t getAverageDistance();
    uint16_t getMedianeDistance();

private:
    can detectorCan;
    const double formulaConstant = 2383.5;
    const double exp = -1.038;
    uint16_t lastFiveDist[5];
    uint16_t lastFiveDistSorted[5];
    uint16_t distance1, distance2, distance3, distance4;
};
