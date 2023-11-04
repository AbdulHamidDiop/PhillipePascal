#include "table.hpp"

Table::Table(uint8_t i =4, uint8_t j =8){
    nLignes = i;
    nColonnes = j;

    for (int ligne = 0; ligne < nLignes; ligne++) {
        for (int colonne = 0; colonne < nColonnes; colonne++) {
            matrice[ligne][colonne] = false;
        }
    }
}

void Table::addPole(int x, int y){
    matrice[x][y] = true;
}

void Table::lectureSurMemoire(){
    uint8_t y;
    for(uint8_t i = 0; i < nColonnes*nLignes; i++){
        y = i/8;
        memory.lecture(i+3, &matrice[i%8][y]);
    }
}

void Table::ecrireSurMemoire(){
    uint8_t y;
    for(uint8_t i = 0; i < nColonnes*nLignes; i++){
        y = i/8;
        memory.ecriture(i+3, matrice[i%8][y]);
    }
}
