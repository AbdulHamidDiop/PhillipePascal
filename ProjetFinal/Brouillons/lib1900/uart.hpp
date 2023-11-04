#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/twi.h>
#include <string.h>
#include "memoire_24.h"

class UART {
public:
    UART(); 
    void transmissionUART( uint8_t donnee );
    uint8_t receptionUSART(void);
    ~UART();
};


