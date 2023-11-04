#include "debug.h"
#include <avr/io.h>
#include <stdlib.h>

void initialisationUART ( void ) {

// 2400 bauds. Nous vous donnons la valeur des deux
// premiers registres pour vous éviter des complications.
UBRR0H = 0;
UBRR0L = 0xCF;
// permettre la réception et la transmission par le UART0
//UCSR0A |= 1 << U2X0;
//UCSR0A |= (1 << RXC0) | (1 <<TXC0) | (1 << UDRE0);
UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
// Format des trames: 8 bits, 1 stop bits, sans parité
UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
    }

// Du USART vers le PC
void transmissionUART ( uint8_t donnee ) {
    while ( !( UCSR0A & (1<<UDRE0)) );
    UDR0 = donnee;
    }

void printuart(char message[]){

    uint8_t size = strlen(message);
    for(uint8_t i =0; i < size ; i++){
        transmissionUART(message[i]);
    }
}
