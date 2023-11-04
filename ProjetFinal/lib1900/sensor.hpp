#include "can.h"
#include <avr/io.h>

class Sensor {
public:

    Sensor();
    uint16_t getDistance();
private:
    can sensorCan;
};
