#include <stdio.h>
#include <util/delay.h>
#include "capteur.hpp"
#include "moteur.hpp"
#include "transmission.cpp"
#include "Minuteries.hpp"
#include "stdlib.h"
volatile uint16_t localTimerCounter = 0;
volatile bool on = 0;
volatile bool isSoundPWMHigh = true;

ISR(TIMER0_COMPA_vect){
    LED led;
    localTimerCounter++;
    if(modeTransmit){
        transLedCounter ++;
        if(transLedCounter == 3){
            if(on){
                on = 0;
                led.lightGreen();
            }
            else{
                on = 1;
                led.lightOff();
            }
            transLedCounter = 0;
        }
    }
}

void moveTowardsPole(Motor& motor, Detector& detector,uint16_t& distance){

    while(distance<=450){
        motor.forward(120,100);
        _delay_ms(300);
        distance = detector.getMedianeDistance();
        if(distance<=120){
            distance = detector.getMedianeDistance();
            while(distance<=120){
                motor.adjustMotor(110,110,true,false);
                distance = detector.getMedianeDistance();
            }
            motor.adjustMotor(110,110,false,true);
            _delay_ms(200);
            motor.stop();
            distance = detector.getMedianeDistance();
        }
    }
    motor.adjustMotor(110,110,true,false);
    _delay_ms(100);
    motor.stop();
}
void faceNorth(){
    Motor motor = Motor();
    motor.adjustMotor(130,130,true,false);
    _delay_ms(1330);
    motor.stop();
    _delay_ms(700);

}
void detecterPoteau(uint16_t distanceMax, bool& detectionNotDone, uint8_t& firstOrSecond, uint16_t timerCounterRef, uint8_t& timerCounter){
    Motor motor;
    Detector detector;
    Timer0 timer0;
    timer0.startTimer(0xFF);
    uint16_t distance = detector.getMedianeDistance(); // nous faisons une premiere lecture inutile de la valeur de detecteur pour surpasser un bug de notre can
    distance = 0;
    if(detectionNotDone){
        motor.adjustMotor(100, 100, false, true);
        _delay_ms(100);
        motor.adjustMotor(130,130, false, true);
        timer0.startTimer(0xFF); 
    }
    while(detectionNotDone && localTimerCounter < timerCounterRef){
        distance = detector.getMedianeDistance();
        detectionNotDone = true;
        if(distance >= distanceMax) //////  
        {
            distance = detector.getMedianeDistance();

            if(distance >= 450)
            {
                motor.adjustMotor(130, 130, true, false);
                _delay_ms(100);
                timer0.stopTimer();
                motor.stop();
                _delay_ms(200);

                motor.stop();
                detectionNotDone = false;
                firstOrSecond = 1;
            } 
            else if (distance >= 210 && distance < 280)
            {
                motor.adjustMotor(130, 130, true, false);
                _delay_ms(100);
                timer0.stopTimer();
                motor.stop();
                _delay_ms(200);
                moveTowardsPole(motor,detector,distance);
                motor.stop();
                detectionNotDone = false;
                firstOrSecond = 2;
            }
            else if (distance >= 280 && distance < 450)
            {   
                motor.adjustMotor(130, 130, true, false);
                _delay_ms(100);
                timer0.stopTimer();
                motor.stop();
                _delay_ms(200);
                moveTowardsPole(motor,detector,distance);
                motor.stop();
                detectionNotDone = false;
                firstOrSecond = 1;
            }
            else if (distance >= 120 && distance < 210)
            {
                motor.adjustMotor(130, 130, true, false);
                _delay_ms(100);
                timer0.stopTimer();
                motor.stop();
                _delay_ms(200);
                moveTowardsPole(motor,detector,distance);
                motor.stop();
                detectionNotDone = false;
                firstOrSecond = 2;
            }
        }
    }
    timer0.stopTimer();
    localTimerCounter = 0;
    if(detectionNotDone){
        motor.adjustMotor(100,100,true,false);
        _delay_ms(80);
        motor.stop();
        _delay_ms(500);
        timerCounter++;
    }
}