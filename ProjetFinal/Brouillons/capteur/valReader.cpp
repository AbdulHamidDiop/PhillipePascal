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

volatile uint8_t gTimerExpired = 0;
volatile uint8_t gTimerCounter = 0;
volatile uint8_t gInterruptButtonClicked = 0;
volatile uint8_t gWhiteButtonClicked = 0;
volatile bool isSoundPWMHigh = true;

void initialiseWhiteButton () {
    cli ();
    DDRB |= (1 << DDB0) | (1 << DDB1);
    EIMSK |= (1 << INT2);
    EICRA |=  (1 << ISC21); 
    sei ();
}

bool isButtonReleased(){
    if(PINB & (1 << PINB3))
        return true;
    return false;
}
void soundPWMLow(){
    PORTA &= ~(1 << PORTA0) & ~(1 << PORTA1);
}

void soundPWMHigh(){
    soundPWMLow();
    PORTA |= (1<< PORTA1);
}

bool interruptButtonIsClicked() 
{
    if ( PIND & 0x04 )
    {
        _delay_ms(10);
        if (PIND & 0x04)
        {
            return true;
        }
    }
    return false;
}

ISR( TIMER0_COMPA_vect ) 
{
     gTimerCounter++;

     if(gTimerCounter == 260)
     {
          gTimerCounter = 0;
          gTimerExpired = 1;
     }
}

ISR ( INT0_vect ) {
    
    _delay_ms(10);
    if(interruptButtonIsClicked())
    {
        gInterruptButtonClicked = 1;
    }
    else
    {
        gInterruptButtonClicked = 0;
    }
    EIFR |= (1 << INTF0) ;   
}

ISR( TIMER2_COMPA_vect ){
    if(isSoundPWMHigh){
        soundPWMLow();
        isSoundPWMHigh = 0;
    }
    else{
        soundPWMHigh();
        isSoundPWMHigh = 1;
    } 
}
ISR (INT2_vect) {
    _delay_ms(15);
    if(!isButtonReleased()){
        gWhiteButtonClicked = 1;
    }
}

uint8_t chooseNote(uint8_t operand){
    uint8_t operandMinimumNote = 45;
    uint8_t indice = operand - operandMinimumNote;
    uint16_t prescaler = 256;
    double frequencyTable[40] = {110, 110,116.54, 123.47, 130.81, 138.59, 146.83, 
                                    155.56, 164.81, 174.61, 185, 196, 207.65, 220, 233.08, 246.94, 261.63, 277.18,
                                    293.66, 311.13, 329.63, 349.23, 369.99, 392, 415.3, 430, 466.16, 493.88, 523.25,
                                    554.37, 587.33, 622.25, 659.26, 698.46, 739.99, 783.99, 830.61, 880 
                                    };
    uint8_t pwmIndice = (F_CPU/(2*prescaler*frequencyTable[indice])-1);
    return pwmIndice;
}

void transmissionUART ( uint8_t donnee ) 
{
    while ( !( UCSR0A & (1<<UDRE0)) );
    UDR0 = donnee;
}

void initialisationUART ( void ) 
{
    UBRR0H = 0;
    UBRR0L = 0xCF;
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}


int main()
{
    initialiseWhiteButton();
    Button button;
    LED led = LED();
    Motor motor = Motor();
    Detector detector = Detector();
    Timer0 timer0;
    // Son son;
    button.useRisingEdge();
    initialisationUART();
//////////////////////////////////////////////////////////////////////////////3
    bool detectionNotDone = true;
    uint8_t rotationSevenSeconds = 0xFF;
    gTimerExpired = 0;
//////////////////////////////////////////////////////////////////////////////3    
    while(gInterruptButtonClicked == 0 && gWhiteButtonClicked == 0){led.lightAmber();}

    if(gInterruptButtonClicked){
        led.lightGreen();
        _delay_ms(2000);
        led.lightOff();
    }
    else if (gWhiteButtonClicked)
    {
        led.lightRed();
        _delay_ms(2000);
        led.lightOff();    
    }
//////////////////////////////////////////////////////////////////////////////3    
    timer0.startTimer(rotationSevenSeconds);
    // motor.forward(0,0);
    // while(true){}


// Detection du poteau en face au départ
//////////////////////////////////////////////////////////////////////////////3    
    uint16_t distance = detector.getMedianeDistance();

// Commence à tourner  
///////////////////////////////////////////////////////////////////////////////3
    if(detectionNotDone){
        motor.adjustMotor(0, 0, false, true);
        _delay_ms(100);
        motor.adjustMotor(0, 0, false, true);
    }
        uint16_t distanceImmediate = detector.getDistanceCm();
    uint8_t compteur = 0;

while(true){
        distance = detector.getMedianeDistance();
            char str[80];
           itoa(distance, str, 10);
           transmissionUART(str[0]);
           transmissionUART(str[1]);
           transmissionUART(str[2]);
           transmissionUART(str[3]);
           transmissionUART(str[4]);
           transmissionUART(str[5]);
           transmissionUART('\n');
           _delay_ms(500);
}           

}
