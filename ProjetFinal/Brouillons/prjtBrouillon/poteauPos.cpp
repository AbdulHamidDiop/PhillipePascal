#define F_CPU 8000000UL
#define DEBUG 
#include <avr/io.h>
#include <light.hpp>
#include <memoire_24.h>
#include <stdio.h>
#include <moteur.hpp>
#include <util/delay.h> 

enum class Virage { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN };

Virage timeToVirage(uint16_t time)
{
    return time >= 65 ? Virage::SEVEN : 
           time >= 55 ? Virage::SIX :
           time >= 45 ? Virage::FIVE : 
           time >= 35 ? Virage::FOUR :
           time >= 25 ? Virage::THREE : 
           time >= 15 ? Virage::TWO :
           time >= 5 ? Virage::ONE : 
           Virage::ZERO;
}

int main() {


    LED led = LED();

    //Get Matrice from memory
    uint8_t exempleMatrice[4][8];

    //Get CurrentX/Y from memory (Already used in detection Code)
    uint8_t currentX = 0;
    uint8_t currentY = 0;

    //Get turningTime & firstOrSecond from detection code 
    uint16_t turningTime = 20;
    uint8_t firstOrSecond = 1;

    enum Virage currentOrientation;
    currentOrientation = timeToVirage(turningTime);
    
    if(currentOrientation == Virage::TWO){
        led.lightRed();
    }
    
    switch(currentOrientation){
        case Virage::ZERO :
            currentY += firstOrSecond;
            break;
        case Virage::ONE :
            currentX += firstOrSecond;
            currentY += firstOrSecond;
            break;
        case Virage::TWO :
            led.lightOff();
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

    if(currentX == 1){
        led.lightGreen();
    }
    exempleMatrice[currentX][currentY] = 1;

    //Write Matrice in memory
    //Write CurrentX/Y in memory

}