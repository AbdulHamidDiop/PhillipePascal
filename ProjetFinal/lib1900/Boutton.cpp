#include "Boutton.hpp"

Button::Button(){
    cli();
    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC00) | (1 << ISC01); 
    sei ();
}
void Button::usefallingEdge(){
    cli();
    EICRA |= (1 << ISC01);
    EICRA &= ~(1 << ISC00);
    sei();
}
void Button::useAnyEdge(){
    cli();
    EICRA |= (1 << ISC00);
    EICRA &= ~(1 << ISC01);
    sei();
}
void Button::useRisingEdge(){
    cli();
    EICRA |= (1 << ISC00) | (1 << ISC01); 
    sei ();
}
Button::~Button(){
    EIMSK &= ~(1 << INT0);
    EICRA &= ~(1 << ISC00) & ~(1 << ISC01);
}

