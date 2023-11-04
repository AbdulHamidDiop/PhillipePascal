#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#include <avr/io.h>
// #include "memoire_24.h"

class Table{
public:
    Table(uint8_t i, uint8_t j);
    uint8_t matrice[4][8];
    void addPole(int x, int y);

    // void ecrireSurMemoire();
    // void lectureSurMemoire();

    uint8_t nLignes;
    uint8_t nColonnes;
private:
    // Memoire24CXXX memory;
};