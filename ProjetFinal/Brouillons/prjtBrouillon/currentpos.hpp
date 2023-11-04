#define F_CPU 8000000 
#include <avr/io.h>

class CurrentPos{
public:
    CurrentPos(uint8_t nx, uint8_t ny);

    void update(uint8_t nx, uint8_t ny);
private:
    uint8_t x, y;

};