// #include "uart.hpp"
#ifndef F_CPU
#define F_CPU 8000000UL
#endif
#include "table.hpp"
#include "memoire_24.h"
#include "SVG.hpp"
#include <stdio.h>




int main(){
    Table table = Table(8, 4);
    // UART uart;
    Memoire24CXXX memory;
    SVG svg;

    table.matrice[2][4] = 1;
    table.matrice[1][5] = 1;
    table.matrice[1][6] = 1;

//Faire une nouvelle matrice où seulement les poteaux sommets de l'envelope sont à 1 et mettre cette matrice dans les prochains if

//Transmit 0x02, 0x03 et faire le calcul de l'aire
    

    table.lectureSurMemoire();
        // char rectangleX[8][3] = {{'1', '8', '6'}, {'2', '9', '6'}, {'4', '0', '6'}, {'5', '1', '6'}, {'6', '2', '6'}, {'7', '3', '6'}, {'8', '4', '6'}, {'9', '5', '6'}};
        // char rectangleY[4][3] = {{'1', '2', '3'}, {'2', '2', '9'}, {'3', '3', '5'}, {'4', '4', '1'}};
        //std::string rectangle = '<rect x="846" y="123" width="10" height="10" stroke="black" stroke-width="1" fill="black"/>';
    uint8_t pointPos = 17;
    
    // for(uint8_t i = 100; i > pointPos; i--){
    //     svg.polygon[i+8] = svg.polygon[i];
    // }

    svg.addPointToPolygon(pointPos, table.matrice);

    // for (int k = 0; k < 4; k++) {
    //     for (int j = 0; j < 8; j++) {
    //         if (table.matrice[k][j] == 1) {
    //             for (int i = pointPos + 50; i > pointPos; i--) {
    //                 svg.polygon[i + 8] = svg.polygon[i];
    //             }
    //            svg.polygon[pointPos] = rectangleX[j][0];
    //            svg.polygon[pointPos + 1] = rectangleX[j][1];
    //            svg.polygon[pointPos + 2] = rectangleX[j][2];
    //            svg.polygon[pointPos + 3] = ',';
    //            svg.polygon[pointPos + 4] = rectangleY[k][0];
    //            svg.polygon[pointPos + 5] = rectangleY[k][1];
    //            svg.polygon[pointPos + 6] = rectangleY[k][2];
    //            svg.polygon[pointPos + 7] = ' ';
    //             pointPos += 8;
    //         }
    //     }
    // }
    svg.transmitControlByte(0x02);
    svg.transmitBackground();

    // for(uint16_t i = 0; i < 92; i++){            
    //     uart.transmissionUART(svg.background[i]);
    // };
    // uart.transmissionUART('\n');
    // for(uint16_t i = 0; i < 92; i++){            
    //     uart.transmissionUART(svg.canva[i]);
    // };
    // uart.transmissionUART('\n');

    // for(uint16_t i = 0; i < 230; i++){            
    //     uart.transmissionUART(svg.polygon[i]);
    // };
    // uart.transmissionUART('\n');

    svg.transmitFront(table.matrice);
    //calculate and transmit area

    svg.transmitControlByte(0x03);

    //Calculate and print CRC
    svg.transmitControlByte(0x04);




    // for(uint8_t k = 0; k < 4; k++){
    //     uart.transmissionUART('\n');
    //     for(uint8_t j = 0; j < 8; j++){
    //         if(table.matrice[k][j] == 1){
    //             svg.circle[12] = rectangleX[j][0];
    //             svg.circle[13] = rectangleX[j][1];
    //             svg.circle[14] = rectangleX[j][2];
    //             svg.circle[21] = rectangleY[k][0];
    //             svg.circle[22] = rectangleY[k][1];
    //             svg.circle[23] = rectangleY[k][2];
    //             for(uint8_t i = 0; i < 79; i++){
    //                 uart.transmissionUART(svg.circle[i]);
    //             }
    //         }
    //         else{

    //             svg.rectangle[9] = rectangleX[j][0];
    //             svg.rectangle[10] = rectangleX[j][1];
    //             svg.rectangle[11] = rectangleX[j][2];
    //             svg.rectangle[17] = rectangleY[k][0];
    //             svg.rectangle[18] = rectangleY[k][1];
    //             svg.rectangle[19] = rectangleY[k][2];
                
    //             for(uint16_t i = 0; i < 92; i++){
                    
    //                 uart.transmissionUART(svg.rectangle[i]);
    //             }
    //         }
    //         uart.transmissionUART('\n');
    //     }
    // }

}