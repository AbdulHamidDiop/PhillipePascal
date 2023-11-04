#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>

class Son {
public:
    Son();

    void initTimer2();
    void startTimer2(uint8_t duration);
    void stopTimer();

    void setPwmIndice(double frequency);

    void playSound(uint8_t midiCode, uint8_t time);
private:

    double frequencyTable[40] = {110, 110,116.54, 123.47, 130.81, 138.59, 146.83, 
                                    155.56, 164.81, 174.61, 185, 196, 207.65, 220,
                                    233.08, 246.94, 261.63, 277.18,293.66, 311.13, 
                                    329.63, 349.23, 369.99, 392, 415.3, 440, 466.16, 
                                    493.88, 523.25,554.37, 587.33, 622.25, 659.26, 
                                    698.46, 739.99, 783.99, 830.61, 880 
                                    };
    uint16_t prescaler = 256;
    uint8_t pwmIndice = 0;
    const uint8_t TABLE_FIRST_MIDI_CODE = 45;   
};