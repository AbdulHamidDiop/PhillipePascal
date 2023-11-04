#include "SVG.hpp"

SVG::SVG(){};

void SVG::addPointToPolygon(uint8_t pointPos, uint8_t matrice[4][8]){
    for (int k = 0; k < 4; k++) {
        for (int j = 0; j < 8; j++) {
            if (matrice[k][j] == 1) {
                for (int i = pointPos + 50; i > pointPos; i--) {
                    polygon[i + 8] = polygon[i];
            }
               polygon[pointPos] = rectangleX[j][0];
               polygon[pointPos + 1] = rectangleX[j][1];
               polygon[pointPos + 2] = rectangleX[j][2];
               polygon[pointPos + 3] = ',';
               polygon[pointPos + 4] = rectangleY[k][0];
               polygon[pointPos + 5] = rectangleY[k][1];
               polygon[pointPos + 6] = rectangleY[k][2];
               polygon[pointPos + 7] = ' ';
                pointPos += 8;
            }
        }
    }

}

void SVG::transmitBackground(){
    for(uint16_t i = 0; i < 92; i++){            
        uart.transmissionUART(background[i]);
    };
    uart.transmissionUART('\n');
    for(uint16_t i = 0; i < 92; i++){            
        uart.transmissionUART(canva[i]);
    };
    uart.transmissionUART('\n');

    for(uint16_t i = 0; i < 230; i++){            
        uart.transmissionUART(polygon[i]);
    };
    uart.transmissionUART('\n');
}

void SVG::transmitFront(uint8_t table[4][8]){
    for(uint8_t k = 0; k < 4; k++){
        uart.transmissionUART('\n');
        for(uint8_t j = 0; j < 8; j++){
            if(table[k][j] == 1){
                circle[12] = rectangleX[j][0];
                circle[13] = rectangleX[j][1];
                circle[14] = rectangleX[j][2];
                circle[21] = rectangleY[k][0];
                circle[22] = rectangleY[k][1];
                circle[23] = rectangleY[k][2];
                for(uint8_t i = 0; i < 79; i++){
                    uart.transmissionUART(circle[i]);
                }
            }
            else{

                rectangle[9] = rectangleX[j][0];
                rectangle[10] = rectangleX[j][1];
                rectangle[11] = rectangleX[j][2];
                rectangle[17] = rectangleY[k][0];
                rectangle[18] = rectangleY[k][1];
                rectangle[19] = rectangleY[k][2];
                
                for(uint16_t i = 0; i < 92; i++){
                    
                    uart.transmissionUART(rectangle[i]);
                }
            }
            uart.transmissionUART('\n');
        }
    }
}

void SVG::transmitControlByte(uint8_t byte){

    uart.transmissionUART('0');
    uart.transmissionUART('x');
    uart.transmissionUART('0');
    if(byte == 2){
        uart.transmissionUART('2');
    }
    else if(byte == 3){
        uart.transmissionUART('3');
    }
    else if(byte == 4){
        uart.transmissionUART('4');
    }
    uart.transmissionUART('\n');

}