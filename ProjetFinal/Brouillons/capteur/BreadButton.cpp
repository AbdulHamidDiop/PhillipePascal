#define F_CPU 8000000 
#include <avr/io.h>
#include <util/delay.h> 
#include <avr/interrupt.h>
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
