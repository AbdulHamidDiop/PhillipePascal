#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "can.h"
#include "debug.h"
#include "uart.hpp"

int main(){
    DDRA = 0x00;
    DDRB = 0xFF;
    can mycan;
    UART uart;
    while(true){
        uint8_t testValue = 12;
        uint16_t canValue = mycan.lecture(2);
        canValue = canValue << 2;
        if(testValue == 12){
            PORTB = 0x01;
            DEBUG_PRINT(testValue); 
        }
        else{
            PORTB = 0x02;
        }
    }
}