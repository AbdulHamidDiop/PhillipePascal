#include "memoire_24.h"
#include "light.hpp"

int main(){
        LED led = LED();

        Memoire24CXXX memory = Memoire24CXXX();
        
        memory.ecriture(0x00, 0x12);
        _delay_ms(5);

        uint8_t octet;
        memory.lecture(0x00, &octet);
        _delay_ms(5);

        if(octet != 0){
                led.lightGreen();
        }
        

}
