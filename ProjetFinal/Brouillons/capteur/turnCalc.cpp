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


#define TURN_TIME 120
volatile bool gWaitingForDirection = 1;


volatile uint16_t turnCounter= 0;
bool interruptButtonClicked(){
    _delay_ms(10);
    if (PIND & 0x04){
        return true;
    }
    return false;
}

ISR ( INT0_vect ) {
    if(interruptButtonClicked()) {
            gWaitingForDirection = 0;
    }
    EIFR |= (1 << INTF0) ;   
}
ISR ( TIMER0_COMPA_vect ) {
    turnCounter++;
}
int main(){

    Motor motor = Motor();
    LED led = LED();
    Timer0 turnTimer = Timer0();
    Button button = Button();
    button.useRisingEdge();

    while(gWaitingForDirection){led.lightAmber();}

    _delay_ms(1000);
    
    motor.adjustMotor(110,110,false,true);
    turnTimer.startTimer(0xFF);

    while(turnCounter<=TURN_TIME){};
    led.lightGreen();

    motor.adjustMotor(110,110,true,false);
    _delay_ms(100);
    motor.stop();
    turnCounter = 0 ;





}
