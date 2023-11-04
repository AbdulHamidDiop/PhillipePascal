#include "Minuteries.hpp"

Timer0::Timer0(){
    cli();
    TCCR0A = (1 << WGM02);
    TCCR0B = (1 << WGM02) | (1 << CS02) | (1 << CS00) ;
    TIMSK0 |= (1 << OCIE0A);
    sei();
}
void Timer0::startTimer(uint8_t duration){
    cli();
    TIMSK0 |= (1 << OCIE0A);
    TCNT0 = 0;
    OCR0A = duration;
    sei();
}
void Timer0::stopTimer(){
    cli();
    TIMSK0 &= ~(1 << OCIE0A);
}
Timer0::~Timer0(){
    cli();
    TCCR0A &= ~(1 << WGM02);
    TCCR0B &= ~(1 << WGM02) & ~(1 << CS02) & ~(1 << CS00);
    TIMSK0 &= ~(1 << OCIE0A);
    sei();
}


Timer1::Timer1(){
    cli();
    TCCR1A = (1 << WGM12);
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    TIMSK1 |= (1 << OCIE1A);
    sei();
}
void Timer1::startTimer(uint16_t duration){
    cli();
    TIMSK1 |= (1 << OCIE1A);
    TCNT1 = 0;
    OCR1A = duration;
    sei();
}
void Timer1::stopTimer(){
    cli();
    TIMSK1 &= ~(1 << OCIE1A);
}
Timer1::~Timer1(){
    TCCR1A &= ~(1 << WGM12);
    TCCR1B &= ~(1 << WGM12) & ~(1 << CS12) & ~(1 << CS10);
    TIMSK1 &= ~(1 << OCIE1A);
}


Timer2::Timer2(){
    cli();
    TCCR2A = (1 << WGM22);
    TCCR2B = (1 << WGM22) | (1 << CS22) | (1 << CS20) | (1 << CS21);
    TIMSK2 |= (1 << OCIE2A);
    sei();
}
void Timer2::startTimer(uint8_t duration){
    cli();
    TIMSK2 |= (1 << OCIE2A);
    TCNT2 = 0;
    OCR2A = duration;
    sei();
}
void Timer2::stopTimer(){
    cli();
    TIMSK2 &= ~(1 << OCIE2A);
}
Timer2::~Timer2(){
    cli();
    TCCR2A &= ~(1 << WGM22);
    TCCR2B &= ~(1 << WGM22) & ~(1 << CS22) & ~(1 << CS20);
    TIMSK2 &= ~(1 << OCIE2A);
    sei();
}


