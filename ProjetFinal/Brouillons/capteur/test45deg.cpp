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

///////////////////FLAGS&CONSTS///////////////////////////////////////////////////////////////////////

#define CLOSE_POLE_VERT 677//460 //545 //
#define FAR_POLE_VERT 234 //295   //
#define CLOSE_POLE_DIAG 334//280 //380 //
#define FAR_POLE_DIAG 140//130 //215   //
#define SENSOR_ERROR_MARGIN 30 //10 //
#define TURN_45DEG_TIME 12

volatile bool gInterruptButtonClicked = 0;
volatile bool gBreadButtonClicked = 0;
volatile bool gWaitingForDirection = 1;
volatile bool gIsOrientationNorth = 0;
volatile uint16_t gTurnCounter = 0;

bool gIsPoleClose = 1;
LED led = LED();
volatile bool ledBool = 0;
void changeLed(){
    if(ledBool){
        led.lightGreen();
        ledBool = 0;
    }
    else{
        led.lightOff();
        ledBool = 1;
    }
}
/////////////////////////////MODES//////////////////////////////////////////////////////////

enum class Mode{Detection, Transmission};
volatile Mode gCurrentMode = Mode::Detection;
///////////////////ORIENTATION///////////////////////////////////////////////////////////////////////

enum class Orientation{No,NoE,Es,SuE,Su,SuO,Os,NoO};
volatile Orientation gCurrentOrientation = Orientation::No;
void changeOrientation(){
    switch(gCurrentOrientation){

        case Orientation::No:
            gCurrentOrientation = Orientation::NoE; 
            break; 

        case Orientation::NoE:
            gCurrentOrientation = Orientation::Es; 
            break;

        case Orientation::Es:
            gCurrentOrientation = Orientation::SuE; 
            break;
            
        case Orientation::SuE:
            gCurrentOrientation = Orientation::Su; 
            break;

        case Orientation::Su:
            gCurrentOrientation = Orientation::SuO; 
            break;

        case Orientation::SuO:
            gCurrentOrientation = Orientation::Os; 
            break;  

        case Orientation::Os:
            gCurrentOrientation = Orientation::NoO; 
            break; 

        case Orientation::NoO:
            gCurrentOrientation = Orientation::No; 
            break; 
    }
}
void faceNorth(){}
//////////////////////////////INTERRUPTS&BUTTONS////////////////////////////////////////////

bool interruptButtonClicked(){
    _delay_ms(10);
    if (PIND & 0x04){
        return true;
    }
    return false;
}

ISR ( INT0_vect ) {
    if(interruptButtonClicked()) {
        if(gWaitingForDirection) {
            gWaitingForDirection = 0;
            gIsOrientationNorth = 1;
        }
    }
    EIFR |= (1 << INTF0) ;   
}
ISR ( TIMER0_COMPA_vect ) {
    if(gCurrentMode == Mode::Detection){
        gTurnCounter++;
        if( gTurnCounter >= TURN_45DEG_TIME){
            gTurnCounter = 0;
            changeOrientation();
            //changeLed();
        }
    }
    else {

    }
}

///////////////////////////////DISTANCE////////////////////////////////////////////////////

bool isCloseVertDistanceValid(uint16_t& distance){
    if (((CLOSE_POLE_VERT-SENSOR_ERROR_MARGIN) <= distance) && (distance<= (CLOSE_POLE_VERT+SENSOR_ERROR_MARGIN))) {

        return true;
    }
    return false;
}
bool isCloseDiagDistanceValid(uint16_t& distance){
    if (((CLOSE_POLE_DIAG-SENSOR_ERROR_MARGIN) <= distance) && (distance <= (CLOSE_POLE_DIAG+SENSOR_ERROR_MARGIN))) {

        return true;
    }
    return false;
}
bool isFarVertDistanceValid(uint16_t& distance){
    if (((FAR_POLE_VERT-SENSOR_ERROR_MARGIN) <= distance) && (distance<= (FAR_POLE_VERT+SENSOR_ERROR_MARGIN))) {

        return true;
    }
    return false;
}
bool isFarDiagDistanceValid(uint16_t& distance){
    if (((FAR_POLE_DIAG-SENSOR_ERROR_MARGIN) <= distance) && (distance<= (FAR_POLE_DIAG+SENSOR_ERROR_MARGIN))) {

        return true;
    }
    return false;
}

bool isPoleDetected(uint16_t& distance){
    switch(gCurrentOrientation){

        case Orientation::No:
            if(isCloseVertDistanceValid(distance)) {
                gIsPoleClose = 1;
                return true;
            }
            else if(isFarVertDistanceValid(distance)) {
                gIsPoleClose = 0;
                return true;
            }
            return false;
            break; 
            
        case Orientation::NoE:
            if(isCloseDiagDistanceValid(distance)) {
                gIsPoleClose = 1;
                return true;
            }
            else if(isFarVertDistanceValid(distance)){
                gIsPoleClose = 0;
            }
            return false;            
            break;

        case Orientation::Es:
            if(isCloseVertDistanceValid(distance)) {
                gIsPoleClose = 1;
                return true;
            }
            else if(isFarVertDistanceValid(distance)) {
                gIsPoleClose = 0;
                return true;
            }
            return false;
            break;

        case Orientation::SuE:
            if(isCloseDiagDistanceValid(distance)) {
                gIsPoleClose = 1;
                return true;
            }
            else if(isFarVertDistanceValid(distance)){
                gIsPoleClose = 0;
            }
            return false;            
            break;

        case Orientation::Su:
            if(isCloseVertDistanceValid(distance)) {
                gIsPoleClose = 1;
                return true;
            }
            else if(isFarVertDistanceValid(distance)) {
                gIsPoleClose = 0;
                return true;
            }
            return false;
            break; 

        case Orientation::SuO:
            if(isCloseDiagDistanceValid(distance)) {
                gIsPoleClose = 1;
                return true;
            }
            else if(isFarVertDistanceValid(distance)){
                gIsPoleClose = 0;
            }
            return false;            
            break;

        case Orientation::Os:
            if(isCloseVertDistanceValid(distance)) {
                gIsPoleClose = 1;
                return true;
            }
            else if(isFarVertDistanceValid(distance)) {
                gIsPoleClose = 0;
                return true;
            }
            return false;
            break; 

        case Orientation::NoO:
            if(isCloseDiagDistanceValid(distance)) {
                gIsPoleClose = 1;
                return true;
            }
            else if(isFarVertDistanceValid(distance)){
                gIsPoleClose = 0;
            }
            return false;            
            break;
    }
}

////////////////////////////DETECTION MOVEMENTS//////////////////////////////////////////

void startRotation(Motor& motor, Timer0& timer){
    motor.adjustMotor(110,110,false,true);
    timer.startTimer(0xFF);
}
void stopRotation(Motor& motor, Timer0& timer){
    timer.stopTimer();
    motor.adjustMotor(110,110,true,false);
    _delay_ms(200);
    motor.stop();
    sei();

}
void goTowardsPole(Motor& motor){}

///////////////////////////MAIN////////////////////////////////////////////////////

int main(){

    Motor motor = Motor();
    Detector detector = Detector();
    Timer0 turnTimer = Timer0();
    Button button = Button();
    button.useRisingEdge();
    uint16_t distance;


    distance = detector.getMedianeDistance();
    distance = 0;

    turnTimer.stopTimer();
    sei();
    while(gWaitingForDirection){}
    led.lightGreen();
    _delay_ms(500);
    led.lightOff();

    if(!gIsOrientationNorth){faceNorth();}
    
    while(1){

        startRotation(motor,turnTimer);
        distance = detector.getMedianeDistance();

        if (isPoleDetected(distance))
        {
       
            stopRotation(motor, turnTimer);
            _delay_ms(1000);         ////DEBOUNCE///////////
            distance = detector.getMedianeDistance();

            if(isPoleDetected(distance)){
                led.lightGreen();
                break;
            }
        }   
    }
//     startRotation(motor, turnTimer);
//     while(1){}
}
