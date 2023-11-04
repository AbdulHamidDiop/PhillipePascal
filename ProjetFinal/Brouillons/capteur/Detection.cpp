#include <stdio.h>
#include <util/delay.h>
#include "capteur.hpp"
#include "moteur.hpp"
#include "light.hpp"
#include "Minuteries.hpp"
#include <stdlib.h>

// volatile uint16_t localTimerCounter = 0;
// volatile bool on = 0;


// ISR(TIMER0_COMPA_vect){
//     LED led;
//     if(on){
//         led.lightGreen();
//         on = 0;
//     }
//     else{
//         led.lightRed();
//         on = 1;
//     }
//     localTimerCounter++;
// }

// void transmissionUART ( uint8_t donnee ) 
// {
//     while ( !( UCSR0A & (1<<UDRE0)) );
//     UDR0 = donnee;
// }

// void initialisationUART ( void ) 
// {
//     UBRR0H = 0;
//     UBRR0L = 0xCF;
//     UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
//     UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
// }

// void detecterPoteau(uint16_t distanceMax, bool& detectionNotDone, uint8_t& firstOrSecond, LED led, uint16_t timerCounterRef, uint16_t& timerCounter){
//     initialisationUART();
//     Motor motor;
//     Detector detector;
//     Timer0 timer0;
//     if(detectionNotDone){
//         motor.adjustMotor(100, 100, false, true);
//         _delay_ms(100);
//         motor.adjustMotor(130,130, false, true);
//         timer0.startTimer(0xFF); // Value a definir et preescaler maybe to change
//     }
//     while(detectionNotDone && localTimerCounter < timerCounterRef){
//         uint16_t distance = detector.getMedianeDistance();
//         if(distance >= distanceMax)       
//         {
//                 motor.adjustMotor(130, 130, true, false);
//                 _delay_ms(100);
//                 if(distance >= 400)
//                 {
//                     led.lightGreen(); //
//                     timer0.stopTimer();
//                     sei();
//                     motor.stop();
//                     _delay_ms(200);
//                     motor.forward(20, 30);
//                     _delay_ms(300);
//                     while(distance <= 200){
//                         led.lightOff();
//                         motor.adjustMotor(160, 160, false, true);
//                         distance = detector.getMedianeDistance();
//                     }
//                     motor.stop();
//                     detectionNotDone = false;
//                 } 
//                 else if (distance >= 120 && distance < 210) //28
//                 {
//                     led.lightGreen();
//                     timer0.stopTimer();
//                     sei();
//                     motor.stop();
//                     _delay_ms(200);
//                     motor.forward(20, 30);
//                     _delay_ms(1000);
//                     while(distance <= 200){
//                         motor.adjustMotor(160, 160, false, true);
//                         distance = detector.getMedianeDistance();
//                     }
//                     // motor.adjustMotor(160, 160, true, false);
//                     motor.stop();
//                     _delay_ms(200);
//                     motor.forward(20,30);
//                     _delay_ms(500);
//                     motor.stop();
//                     detectionNotDone = false;
//                 }
//                 else if (distance >= 210 && distance < 400)//??
//                 {   
//                     // led.lightRed();
//                     timer0.stopTimer();
//                         sei();
//                     motor.stop();
//                     motor.forward(20, 30);
//                     _delay_ms(500);
//                     while(distance <= 200){
//                         motor.adjustMotor(160, 160, false, true);
//                         distance = detector.getMedianeDistance();
//                     }
//                     motor.stop();
//                     detectionNotDone = false;
//                 }
//                 else if (distance >= 95 && distance < 150)//23
//                 {
//                     // led.lightRed();
//                     // timer0.stopTimer();
//                     motor.stop();
//                     motor.forward(20, 30);
//                     _delay_ms(1700);
//                     motor.stop();
//                     _delay_ms(200);
//                     while(distance <= 200){
//                         motor.adjustMotor(160, 160, false, true);
//                         distance = detector.getMedianeDistance();
//                     }
//                     motor.adjustMotor(160, 160, true, false);
//                     _delay_ms(50);
//                     motor.stop();
//                     _delay_ms(200);
//                     motor.forward(20,30);
//                     _delay_ms(700);
//                     motor.stop();
//                     _delay_ms(200);
//                     while(distance <= 200){
//                         motor.adjustMotor(160, 160, false, true);
//                         distance = detector.getMedianeDistance();
//                     }
//                     motor.adjustMotor(160, 160, true, false);
//                     _delay_ms(50);
//                     motor.stop();
//                     detectionNotDone = false;
//                 }
//         }
//     }
//     timerCounter += localTimerCounter;
//     timer0.stopTimer();
//         sei();

//         char str[80];
//         itoa(timerCounter, str, 10);
//         transmissionUART(str[0]);
//         transmissionUART(str[1]);
//         transmissionUART(str[2]);
//         _delay_ms(500);
//         transmissionUART('\n');
// }
// int main(){
//     LED led = LED();
//     uint16_t turningCounter = 0;
//     bool detectionNotDone = true;
//     uint16_t turningCounterRef = 99;
//     uint16_t turningCounterRef2 = 1000000;
//     uint16_t turningCounterRefDepart = turningCounterRef2 / 2;
//     uint8_t x = 3;
//     uint8_t y = 2;
//     uint16_t distancesMax[8] = {1,1,1,1,1,1,1,1};
//     uint8_t a =1;
//     detecterPoteau(distancesMax[0], detectionNotDone, a, led, turningCounterRefDepart, turningCounter);
// }

int main(){
    Motor motor = Motor();
    LED led = LED();
    Detector detector = Detector();

    uint16_t dist;
    bool notDone = 1;
        motor.adjustMotor(100, 100, false, true);
        _delay_ms(100);
        motor.adjustMotor(110,110, false, true);
    while(notDone){

        dist = detector.getMedianeDistance();

        if(dist >= 200){
            motor.stop();
            cli();
            _delay_ms(500);
            notDone = 0;
        }

    }
}