#include "light.hpp"
#define AMBER_DELAY_RED 10
#define AMBER_DELAY_GREEN 20
LED::LED()
{
    DDRB = (1 << PORTB0) | (1 << PB1);
}
void LED::lightGreen()  
{
    PORTB&= ~(1 << PB1);
    PORTB|= (1 << PB0);
}

void LED::lightRed()      
{ 
    PORTB&=~ (1 << PB0);
    PORTB|= (1 << PB1);
}

void LED::lightOff()  
{
    PORTB&=~ (1 << PB0);
    PORTB&=~ (1 << PB1);
}

void LED::lightAmber()
{
    lightGreen();
    _delay_ms(AMBER_DELAY_GREEN);
    lightRed();
    _delay_ms(AMBER_DELAY_RED);
}

// void LED::blinkRed()
// {
//     led.lightAmber();
//     _delay_ms(20);
//     led.lightOff();
//     _delay_ms(480);
//     led.lightAmber();
//     _delay_ms(20);
//     led.lightOff();
//     _delay_ms(480);
// }

LED::~LED(){}
