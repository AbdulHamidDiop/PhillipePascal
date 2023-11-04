#include "memoire_24.h"
#include <util/delay.h>

int main(){
    Memoire24CXXX memory;
    uint8_t matrice[4][8];
    for(uint8_t i = 0; i < 32; i++){
        matrice[i/8][i%8] = 0;
    }
    for(uint8_t i = 0; i < 32; i++){
        memory.ecriture(i+4, matrice[i/8][i%8]);
        _delay_ms(5);
    }
        
    memory.ecriture(0x00, 0);
    _delay_ms(5);
    memory.ecriture(0x02, 0);
    _delay_ms(5);
    memory.ecriture(0x03, 0);
    _delay_ms(5);
}