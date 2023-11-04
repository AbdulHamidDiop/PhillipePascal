#define F_CPU 8000000 
#include <avr/io.h>
#include "memoire_24.h"

class Table{
public:
    Table(uint8_t i, uint8_t j);
    bool matrice[4][8];
    void addPole(int x, int y);

    void lectureSurMemoire();
    void ecrireSurMemoire();

    uint8_t nLignes;
    uint8_t nColonnes;
private:
    Memoire24CXXX memory;
};