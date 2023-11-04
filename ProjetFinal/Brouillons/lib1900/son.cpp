#include "son.hpp"

Son::Son(){
    DDRA |= (1 << DDA0) | (1 << DDA1);
}

void Son::initTimer2(){
    //A modifier selon le setup WaveForm Gen
    cli();
    TCCR2A = (1 << WGM21);
    TCCR2B = (1 << WGM22) |  (1 << CS21) | (1 << CS22);
    TIMSK2 |= (1 << OCIE2A);
    sei();
}

void Son::startTimer2(uint8_t duration){
    cli();
    TIMSK2 |= (1 << OCIE2A);
    TCNT2 = 0;
    OCR2A = duration;
    sei();    
}

void Son::stopTimer(){
    cli();
    TIMSK2 &= ~(1 << OCIE2A);
    sei();
}

void Son::setPwmIndice(double frequency){
    pwmIndice = (F_CPU/(2*prescaler*frequency)-1);  
}

void Son::playSound(uint8_t midiCode, uint8_t duration){

    setPwmIndice(frequencyTable[midiCode-TABLE_FIRST_MIDI_CODE]);

    startTimer2(pwmIndice);

    duration = duration/25;
    while(static_cast<bool>(duration--)){ //////////////////////////////Faire une fonction myDelayMS
    _delay_ms(25);
  }
    stopTimer();
}