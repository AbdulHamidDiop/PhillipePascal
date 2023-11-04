#include "sensor.hpp"

Sensor::Sensor(){
    DDRA = ~(1 << PORTA2);
}

uint16_t Sensor::getDistance(){
    return((sensorCan.lecture(2)) >> 2);
}