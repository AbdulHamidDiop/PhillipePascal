#include "currentpos.hpp"

CurrentPos::CurrentPos(uint8_t nx = 0, uint8_t ny = 0){
    x = nx;
    y = ny;
}

void CurrentPos::update(uint8_t nx, uint8_t ny) {
    x = nx;
    y = ny;
}
int main(){}