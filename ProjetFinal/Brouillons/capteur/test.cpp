#define F_CPU 8000000
#include "moteur.hpp"
#include "capteur.hpp"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "light.hpp"
#include "Minuteries.hpp"
#include "Boutton.hpp"
#include "son.hpp"


int main(){
    
    Motor motor;
    Detector detector;

    uint16_t distance = 0;
    motor.adjustMotor(0,0,true,false);
    while(distance < 300){
        distance = detector.getMedianeDistance();
    }
    motor.stop();

}