#include <stdio.h>
#include "capteur.hpp"
#include "moteur.hpp"
#include "light.hpp"

void detecterPoteau(uint16_t distanceMin, bool& detectionNotDone, uint8_t& firstOrSecond);
class PoleDetector(){
    public:
        PoleDetector();
        void detecterPoteau(uint16_t distanceMin, bool& detectionNotDone, uint8_t& firstOrSecond);

};