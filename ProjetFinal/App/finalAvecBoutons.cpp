
#include "sensor.hpp"
#include "table.hpp"
#include "memoire_24.h"
#include "virage.hpp"
#include "Boutton.hpp"
#include "son.hpp"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <util/delay.h>
#include "Detection.cpp"

volatile bool waitForStart = true;
volatile bool directionIsTop = true;
volatile uint8_t gInterruptButtonClicked = 0;
volatile uint8_t gWhiteButtonClicked = 0;

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

void soundPWMLow(){
    PORTA &= ~(1 << PORTA0) & ~(1 << PORTA1);
}

void soundPWMHigh(){
    soundPWMLow();
    PORTA |= (1<< PORTA1);
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

ISR (INT2_vect) {
    _delay_ms(15);
    if(!isButtonReleased()){
        gWhiteButtonClicked = 1;
    }
}



Virage timeToVirage(uint8_t turn)
{
    return turn >= 7 ? Virage::SEVEN : 
           turn >= 6 ? Virage::SIX :
           turn >= 5 ? Virage::FIVE : 
           turn >= 4 ? Virage::FOUR :
           turn >= 3 ? Virage::THREE : 
           turn >= 2 ? Virage::TWO :
           turn >= 1 ? Virage::ONE : 
           Virage::ZERO;
}


void initialisationUART ( void ) 
{
    UBRR0H = 0;
    UBRR0L = 0xCF;
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

void transmissionUART ( uint8_t donnee ) 
{
    while ( !( UCSR0A & (1<<UDRE0)) );
    UDR0 = donnee;
}

void getDistanceMax(uint8_t x, uint8_t y, uint16_t distancesMax[8]){
    for(uint8_t i = 0; i < 8; i++){
        distancesMax[i] = 120;
    }
    if(x == 6 ){
        distancesMax[1] =  280; 
        distancesMax[2] =  280;
        distancesMax[3] =  280;
    }
    if(x == 1){
        distancesMax[5] =  280;
        distancesMax[6] =  280;
        distancesMax[7] =  280;
    }
    if(y == 1 ){
        distancesMax[3] =  280;
        distancesMax[4] =  280;
        distancesMax[5] =  280;
    }
    if(y == 2){
        distancesMax[0] =  280;
        distancesMax[1] =  280;
        distancesMax[7] =  280;
    }
    if(x == 0){
        distancesMax[5] = 10000;
        distancesMax[6] = 10000;
        distancesMax[7] = 10000;
    }
    if(x == 7){
        distancesMax[1] = 10000;
        distancesMax[2] = 10000;
        distancesMax[3] = 10000;
    }
    if(y == 3){
        distancesMax[0] = 10000;
        distancesMax[1] = 10000;
        distancesMax[7] = 10000;
    }
    if(y == 0){
        distancesMax[3] = 10000;
        distancesMax[4] = 10000;
        distancesMax[5] = 10000;
    }
}


void detectionMode(uint8_t& turningTime, uint8_t& firstOrSecond, uint8_t& detectionModeEnded, uint8_t x, uint8_t y ){
    uint8_t turningCounter = 0;
    bool detectionNotDone = true;
    uint16_t turningCounterRef = 32;
    uint16_t turningCounterRef2 = 21;
    uint16_t turningCounterRefDepart = 11;
    uint16_t distancesMax[8];
    getDistanceMax(x, y, distancesMax);

    detecterPoteau(distancesMax[0], detectionNotDone, firstOrSecond, turningCounterRefDepart, turningCounter);
    detecterPoteau(distancesMax[0], detectionNotDone, firstOrSecond, turningCounterRef, turningCounter);
    detecterPoteau(distancesMax[1], detectionNotDone, firstOrSecond, turningCounterRef2, turningCounter);
    detecterPoteau(distancesMax[2], detectionNotDone, firstOrSecond, turningCounterRef, turningCounter);
    detecterPoteau(distancesMax[3], detectionNotDone, firstOrSecond, turningCounterRef2, turningCounter);
    detecterPoteau(distancesMax[4], detectionNotDone, firstOrSecond, turningCounterRef, turningCounter);
    detecterPoteau(distancesMax[5], detectionNotDone, firstOrSecond, turningCounterRef2, turningCounter);
    detecterPoteau(distancesMax[6], detectionNotDone, firstOrSecond, turningCounterRef, turningCounter);
    detecterPoteau(distancesMax[7], detectionNotDone, firstOrSecond, turningCounterRef2, turningCounter);

    if(detectionNotDone){
        detectionModeEnded = true;
    }
    else{
        turningTime = turningCounter;
        detectionModeEnded = false;
    }
}

int main(){
    LED led = LED();
    Table table = Table(4, 8);
    Memoire24CXXX memory = Memoire24CXXX();
    Button button;
    Motor motor;
    Son son;

    initialiseWhiteButton();
    button.useRisingEdge();
    initialisationUART();

    uint8_t currentX = 0;
    uint8_t currentY = 0;
    uint8_t detectionModeEnded = 0;
    uint8_t turningTime = 0;
    uint8_t firstOrSecond = 1;
    bool modeNotFinished = true;
    uint8_t y;

    for(uint8_t i = 0; i < table.nColonnes*table.nLignes; i++){
        y = i/8;
        memory.lecture(i+4, &table.matrice[y][i%8]);
        _delay_ms(5);
    }
    memory.lecture(0x00, &currentX);
    _delay_ms(5); 
    memory.lecture(0x02, &currentY);
    _delay_ms(5); 
    memory.lecture(0x03, &detectionModeEnded);
    _delay_ms(5); 

    led.lightOff();

    while(modeNotFinished){
        if(detectionModeEnded){
            while(gWhiteButtonClicked == 0){}
            led.lightGreen();
            _delay_ms(2000);
            led.lightOff();
            TransmissionMode(table.matrice);
            modeNotFinished = 0;
        }
        else
        {
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
                directionIsTop = false;
            }

            if(directionIsTop){
                detectionMode(turningTime, firstOrSecond, detectionModeEnded, currentX, currentY );
            }
            else{
                faceNorth();
                detectionMode(turningTime, firstOrSecond, detectionModeEnded, currentX, currentY );
            }

            if(detectionModeEnded){
                uint8_t note = chooseNote(45);
                son.initTimer2();
                son.startTimer2(note);
                _delay_ms(2000);
                son.stopTimer();
            }
            else if(!detectionModeEnded){
                uint8_t note = chooseNote(79);
                son.initTimer2();
                for (int i = 0; i < 3; i++){
                    son.startTimer2(note);
                    _delay_ms(300);
                    son.stopTimer();
                    _delay_ms(300);
                } 
            }
            led.lightOff();
            if(!detectionModeEnded){
                enum Virage currentOrientation;
                currentOrientation = timeToVirage(turningTime);
                switch(currentOrientation){
                    case Virage::ZERO :
                        currentY += firstOrSecond;
                        break;
                    case Virage::ONE :
                        currentX += firstOrSecond;
                        currentY += firstOrSecond;
                        break;
                    case Virage::TWO :
                        currentX += firstOrSecond;
                        break;
                    case Virage::THREE :
                        currentX += firstOrSecond;
                        currentY -= firstOrSecond;
                        break;
                    case Virage::FOUR :
                        currentY -= firstOrSecond;
                        break;
                    case Virage::FIVE :
                        currentX -= firstOrSecond;
                        currentY -= firstOrSecond;
                        break;
                    case Virage::SIX :
                        currentX -= firstOrSecond;
                        break;
                    case Virage::SEVEN :
                        currentX -= firstOrSecond;
                        currentY += firstOrSecond;
                        break;   
                }
                table.matrice[currentY][currentX] = 1;

                for(uint8_t i = 0; i < table.nLignes*table.nColonnes; i++){
                    y = i/8;
                    memory.ecriture(i+4, table.matrice[y][i%8]);
                    _delay_ms(5);
                }
                memory.ecriture(0x00, currentX);
                _delay_ms(5);
                memory.ecriture(0x02, currentY);
                _delay_ms(5);
            }
            memory.ecriture(0x03, detectionModeEnded);
            _delay_ms(5); 
            modeNotFinished = false;
        }
    }

    if(detectionModeEnded){
        while(true){
            led.lightGreen();
            _delay_ms(20);
            led.lightRed();
            _delay_ms(10);
            led.lightOff();
            _delay_ms(470);
            led.lightGreen();
            _delay_ms(20);
            led.lightRed();
            _delay_ms(10);
            led.lightOff();
            _delay_ms(470);
        }
    }
    else{
        while(true){
            led.lightRed();
            _delay_ms(20);
            led.lightOff();
            _delay_ms(480);
            led.lightRed();
            _delay_ms(20);
            led.lightOff();
            _delay_ms(480);
        }

    }
}


