#include "uart.hpp"
#include "table.hpp"
#include "memoire_24.h"
#include "SVG.hpp"
#include <stdio.h>
#include "light.hpp"
#include <stdlib.h>


uint8_t orientation(uint8_t a[2], uint8_t b[2], uint8_t c[2]){
    
    int val = (b[0] - a[0]) * (c[1] - b[1]) - (b[1] - a[1]) * (c[0] - b[0]);

    if (val == 0)
        return 0; // colinear
    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

uint16_t calculateArea(uint8_t points[9][2], uint8_t amountOfPoint){
    uint8_t area =0;
    for(uint8_t i =0; i < amountOfPoint; i++){
        area += points[i][1] * points[(i+1)%amountOfPoint][0] - points[i][0] - points[(i+1)%amountOfPoint][1];
    }
    return area/2;
}

void getPolygonMatrice(const uint8_t tableMatrice[4][8], uint8_t matricePolygon[4][8], uint8_t& amountOfPeak){

    LED led;
    uint8_t points[9][2] ;
    uint8_t validPoint[9][2];
    uint8_t amountOfPoints = 0;


    for(uint8_t i = 0; i < 4; i++){
        for(uint8_t j = 0; j < 8; j++){
            if(tableMatrice[i][j] == 1){
                points[amountOfPoints][0] = i;
                points[amountOfPoints][1] = j;
                amountOfPoints++;
            }
        }
    }

    uint8_t leftMostPoint = 0;
    for(uint8_t i = 0; i< amountOfPoints; i++){
        if(points[i][1] < points[leftMostPoint][1]){
            leftMostPoint = i;
        }
    }

    uint8_t a = leftMostPoint, b;
    do
    {
        b = (a + 1) % amountOfPoints; // Prend le point suivant (Le modulo permet de reprendre au premier pts après avoir vérif le dernier )
        for( uint8_t i = 0; i < amountOfPoints; i++){
            if (orientation(points[a], points[i], points[b]) == 2){
                b = i;
            }
        }
        a = b;
        validPoint[amountOfPeak][0] = points[b][0];
        validPoint[amountOfPeak][1] = points[b][1];
        amountOfPeak++;
    }while(a != leftMostPoint);

    validPoint[amountOfPeak][0] = points[leftMostPoint][0];
    validPoint[amountOfPeak][1] = points[leftMostPoint][1];

    for(uint8_t i = 0; i < amountOfPeak; i++){
        matricePolygon[validPoint[i][0]][validPoint[i][1]] = 1;          
        
    }
}


int main(){
    Table table = Table(8, 4);
    LED led;
    UART uart;
    Memoire24CXXX memory;
    SVG svg;
    uint8_t numberOfPoint =0;
    uint8_t amountOfPeak = 0;
    uint8_t matricePolygon[4][8];


    uint8_t val = 1;

    uint8_t points[9][2] ;
    uint8_t validPoint[9][2];
    uint8_t r = 0;

    table.matrice[1][5] = 1;
    table.matrice[3][6] = 1;
    // table.matrice[2][5] = 1;
    table.matrice[2][6] = 1;
    table.matrice[1][0] = 1;
     table.matrice[0][7] = 1;
    table.matrice[3][2] = 1;

    for(uint8_t i = 0; i < 4; i++){
        for(uint8_t j = 0; j < 8; j++){

            if(table.matrice[i][j] == 1){
                points[numberOfPoint][0] = i;
                points[numberOfPoint][1] = j;
                numberOfPoint++;
            }

        }

    }

    uint8_t leftMostPoint = 0;
    for(uint8_t i = 0; i< numberOfPoint; i++){
        if(points[i][1] < points[leftMostPoint][1]){

            leftMostPoint = i;
            led.lightGreen();
        }
    }

    uint8_t a = leftMostPoint, b;
    do
    {

        b = (a + 1) % numberOfPoint; // Prend le point suivant (Le modulo permet de reprendre au premier pts après avoir vérif le dernier )

        for( uint8_t i = 0; i < numberOfPoint; i++){
            if (orientation(points[a], points[i], points[b]) == 2){
                b = i;
            }
        }
        a = b;
        validPoint[r][0] = points[b][0];
        validPoint[r][1] = points[b][1];
        r++;
    }while(a != leftMostPoint);



    validPoint[r][0] = points[leftMostPoint][0];
    validPoint[r][1] = points[leftMostPoint][1];

    
    for(uint8_t i = 0; i < r; i++){
        matricePolygon[validPoint[i][0]][validPoint[i][1]] = 1;          
        
    }


//Faire une nouvelle matrice où seulement les poteaux sommets de l'envelope sont à 1 et mettre cette matrice dans les prochains if

//Transmit 0x02, 0x03 et faire le calcul de l'aire

    uart.transmissionUART('\x02');
    uart.transmissionUART('\n');

    // table.lectureSurMemoire();
        char rectangleX[8][3] = {{'1', '8', '6'}, {'2', '9', '6'}, {'4', '0', '6'}, {'5', '1', '6'}, {'6', '2', '6'}, {'7', '3', '6'}, {'8', '4', '6'}, {'9', '5', '6'}};
        char rectangleY[4][3] = {{'1', '2', '3'}, {'2', '2', '9'}, {'3', '3', '5'}, {'4', '4', '1'}};

    for(int i = 0; i < 90 ; i++){
        uart.transmissionUART(svg.beginingSVG[i]);
    }

    uart.transmissionUART('\n');

    uint8_t pointPos = 17;
    for (int k = 0; k < 4; k++) {
        for (int j = 0; j < 8; j++) {
            if (matricePolygon[k][j] == 1) {
                for (int i = pointPos + 100; i > pointPos; i--) {
                    svg.polygon[i + 8] = svg.polygon[i];
                }
               svg.polygon[pointPos] = rectangleX[j][0];
               svg.polygon[pointPos + 1] = rectangleX[j][1];
               svg.polygon[pointPos + 2] = rectangleX[j][2];
               svg.polygon[pointPos + 3] = ',';
               svg.polygon[pointPos + 4] = rectangleY[k][0];
               svg.polygon[pointPos + 5] = rectangleY[k][1];
               svg.polygon[pointPos + 6] = rectangleY[k][2];
               svg.polygon[pointPos + 7] = ' ';
                pointPos += 8;
            }
        }
    }

    for(uint16_t i = 0; i < 91; i++){            
        uart.transmissionUART(svg.background[i]);
    };
    uart.transmissionUART('\n');
    for(uint16_t i = 0; i < 91; i++){            
        uart.transmissionUART(svg.canva[i]);
    };
    uart.transmissionUART('\n');

    for(uint16_t i = 0; i < (67 + 8*r); i++){            
        uart.transmissionUART(svg.polygon[i]);
    };
    uart.transmissionUART('\n');

    
    for(uint8_t k = 0; k < 4; k++){
        uart.transmissionUART('\n');

        for(uint8_t j = 0; j < 8; j++){

            if(table.matrice[k][j] == 1){
                svg.circle[12] = rectangleX[j][0];
                svg.circle[13] = rectangleX[j][1];
                svg.circle[14] = rectangleX[j][2];
                svg.circle[21] = rectangleY[k][0];
                svg.circle[22] = rectangleY[k][1];
                svg.circle[23] = rectangleY[k][2];
                for(uint8_t i = 0; i < 78; i++){
                    uart.transmissionUART(svg.circle[i]);
                }
            }
            else{

                svg.rectangle[9] = rectangleX[j][0];
                svg.rectangle[10] = rectangleX[j][1];
                svg.rectangle[11] = rectangleX[j][2];
                svg.rectangle[17] = rectangleY[k][0];
                svg.rectangle[18] = rectangleY[k][1];
                svg.rectangle[19] = rectangleY[k][2];
                
                for(uint16_t i = 0; i < 91; i++){
                    
                    uart.transmissionUART(svg.rectangle[i]);
                }
            }
            uart.transmissionUART('\n');
        }
    }

    for(uint8_t i = 0; i < 79; i++){
        uart.transmissionUART(svg.text[i]);
    }

    uart.transmissionUART('\n');

    for(uint8_t i = 0; i < 6; i++){
        uart.transmissionUART(svg.endSVG[i]);
    }
    uart.transmissionUART('\n');

    uart.transmissionUART(0x03);

    uart.transmissionUART(32);

    uart.transmissionUART(0x04);
    while(1){
        uart.transmissionUART('O');
        _delay_ms(100);
    }

}