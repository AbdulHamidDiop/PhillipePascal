#define F_CPU 8000000
#include "moteur.hpp"
#include "capteur.hpp"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include "light.hpp"
#include "Minuteries.hpp"
#include "Boutton.hpp"
#include "son.hpp"
    if(detectionNotDone){
        motor.adjustMotor(0, 0, false, true);
        _delay_ms(100);
        motor.adjustMotor(160, 150, false, true);
    }
int main(){
while (true) {
    distance = detector.getMedianeDistance();
    if (distance <= 15) {
        motor.stop();
        break;
    } else if (distance == 100) {  // pole is lost, realign
        motor.left();
        delay(500);
        motor.forward();
    } else {
        motor.forward();
    }
    delay(100);  // check distance every 100ms
}
}


