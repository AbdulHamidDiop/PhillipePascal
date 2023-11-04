#include "uart.hpp"
#include "SVG.hpp"
// #include "crc.hpp"
#include "light.hpp"
#include "Minuteries.hpp"
volatile bool gIsTransLedOn = 0;
volatile uint8_t transLedCounter = 0;
LED led;

void initialisationUART ( void ) 
{
    UBRR0H = 0;
    UBRR0L = 0xCF;
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

void transmissionUART ( uint8_t donnee ) 
{
    while ( !( UCSR0A & (1<<UDRE0)) );
    UDR0 = donnee;
}

ISR(TIMER0_COMPA_vect){
    if(transLedCounter == 3){
        if(gIsTransLedOn){
            led.lightOff();
            gIsTransLedOn = 0;
        }
        else{
            led.lightGreen();
            gIsTransLedOn = 1;
        }
        transLedCounter = 0;
    }
    transLedCounter++;
}

uint8_t orientation(uint8_t a[2], uint8_t b[2], uint8_t c[2]){
    
    int val = (b[0] - a[0]) * (c[1] - b[1]) - (b[1] - a[1]) * (c[0] - b[0]);

    if (val == 0)
        return 0; // colinear
    return (val > 0) ? 1 : 2; // clock or counterclock wise
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


uint16_t calculateArea(uint8_t points[9][2], uint8_t amountOfPoint){
    uint8_t area =0;
    for(uint8_t i =0; i < amountOfPoint; i++){
        area += points[i][1] * points[(i+1)%amountOfPoint][0] - points[i][0] - points[(i+1)%amountOfPoint][1];
    }
    return area/2;
}
//////////////////////////////////////////////////
void separate_uint32(uint32_t value, uint8_t& byte1, uint8_t& byte2, uint8_t& byte3, uint8_t& byte4) {
    byte1 = (value >> 24) & 0xFF;
    byte2 = (value >> 16) & 0xFF;
    byte3 = (value >> 8) & 0xFF;
    byte4 = value & 0xFF;
}
///////////////////////////////////////////////////
void TransmissionMode(uint8_t tableMatrice[4][8]){
    //LED led;
    led.lightGreen();
    _delay_ms(2000);
    SVG svg;
    UART uart;
    //crc32 //crc;
    //uint8_t byteToTransmit = 0;
    // uint8_t* bytePtr = &//byteToTransmit;
    uint8_t amountOfPeak = 0;
    uint8_t matricePolygon[4][8];

    tableMatrice[1][2] = 1;
    getPolygonMatrice(tableMatrice, matricePolygon, amountOfPeak);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////3


    uint8_t PolygonPointPos = 17;
    char rectangleX[8][3] = {{'1', '8', '6'}, {'2', '9', '6'}, {'4', '0', '6'}, {'5', '1', '6'}, {'6', '2', '6'}, {'7', '3', '6'}, {'8', '4', '6'}, {'9', '5', '6'}};
    char rectangleY[4][3] = {{'1', '2', '3'}, {'2', '2', '9'}, {'3', '3', '5'}, {'4', '4', '1'}};

    uart.transmissionUART('\x02');
    uart.transmissionUART('\n');

    for(int i = 0; i < 90 ; i++){
        //byteToTransmit = svg.beginingSVG[i];            
        uart.transmissionUART(svg.beginingSVG[i]);
        //crc.update(bytePtr);
        _delay_ms(5);
    }
    //byteToTransmit = '\n';
    uart.transmissionUART('\n');
    //crc.update(bytePtr);


    for (int k = 0; k < 4; k++) {
        for (int j = 0; j < 8; j++) {
            if (matricePolygon[k][j] == 1) {
                for (int i = PolygonPointPos + 100; i > PolygonPointPos; i--) {
                    svg.polygon[i + 8] = svg.polygon[i];
                }
               svg.polygon[PolygonPointPos] = rectangleX[j][0];
               svg.polygon[PolygonPointPos + 1] = rectangleX[j][1];
               svg.polygon[PolygonPointPos + 2] = rectangleX[j][2];
               svg.polygon[PolygonPointPos + 3] = ',';
               svg.polygon[PolygonPointPos + 4] = rectangleY[k][0];
               svg.polygon[PolygonPointPos + 5] = rectangleY[k][1];
               svg.polygon[PolygonPointPos + 6] = rectangleY[k][2];
               svg.polygon[PolygonPointPos + 7] = ' ';
                PolygonPointPos += 8;
            }
        }
    }
///////////////////////////
    for(uint16_t i = 0; i < 91; i++){
        //byteToTransmit = svg.background[i];            
        uart.transmissionUART(svg.background[i]);
        //crc.update(bytePtr);
    };

    //byteToTransmit = '\n';
    uart.transmissionUART('\n');
    //crc.update(bytePtr);

    for(uint16_t i = 0; i < 91; i++){   
        //byteToTransmit = svg.canva[i];         
        uart.transmissionUART(svg.canva[i]);
        //crc.update(bytePtr);
    };

    //byteToTransmit = '\n';
    uart.transmissionUART('\n');
    //crc.update(bytePtr);

    for(uint16_t i = 0; i < (67 + 8*amountOfPeak); i++){  
        //byteToTransmit = svg.polygon[i];          
        uart.transmissionUART(svg.polygon[i]);
        //crc.update(bytePtr);
    };

    _delay_ms(5);
    //byteToTransmit = '\n';
    uart.transmissionUART('\n');
    //crc.update(bytePtr);

    for(uint8_t k = 0; k < 4; k++){
        //byteToTransmit = '\n';
        uart.transmissionUART('\n');
        //crc.update(bytePtr);

        for(uint8_t j = 0; j < 8; j++){
            if(tableMatrice[k][j] == 1){
                svg.circle[12] = rectangleX[j][0];
                svg.circle[13] = rectangleX[j][1];
                svg.circle[14] = rectangleX[j][2];
                svg.circle[21] = rectangleY[k][0];
                svg.circle[22] = rectangleY[k][1];
                svg.circle[23] = rectangleY[k][2];

                for(uint8_t i = 0; i < 78; i++){
                    //byteToTransmit = svg.circle[i];
                    uart.transmissionUART(svg.circle[i]);
                    //crc.update(bytePtr);
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
                    //byteToTransmit = svg.rectangle[i];    
                    uart.transmissionUART(svg.rectangle[i]);
                    //crc.update(bytePtr);
                }
            }
            //byteToTransmit = '\n';
            uart.transmissionUART('\n');
            //crc.update(bytePtr);
        }
    }
    for(uint8_t i = 0; i < 79; i++){
        uart.transmissionUART(svg.text[i]);
    }

        //byteToTransmit = '\n';
        uart.transmissionUART('\n');
        //crc.update(bytePtr);

    for(uint8_t i = 0; i < 6; i++){

        //byteToTransmit = svg.endSVG[i];
        uart.transmissionUART(svg.endSVG[i]);
        //crc.update(bytePtr);
    }

    uart.transmissionUART('\n');

    uart.transmissionUART('\x03');

    // uint8_t byte1, byte2, byte3, byte4;
    // separate_uint32(//crc.value, byte1, byte2, byte3, byte4);
    // uart.transmissionUART(byte1);
    // uart.transmissionUART(byte2);
    // uart.transmissionUART(byte3);
    // uart.transmissionUART(byte4);

    uart.transmissionUART('\x04');
    led.lightRed();
    _delay_ms(500);
}

// void TransmissionMode(uint8_t tableMatrice[4][8]){
//     UART uart;
//     SVG svg;
//     // //crc32 //crc;
//     // uint8_t //byteToTransmit = 0;
//     // uint8_t* bytePtr = &//byteToTransmit;

// //         //byteToTransmit = svg.beginingSVG[i];            
// //         uart.transmissionUART(//byteToTransmit);
// //         //crc.update(bytePtr);
//     while(1){
//         //byteToTransmit = svg.rectangle[14];
//         uart.transmissionUART(svg.rectangle[14]);
//         //crc.update(bytePtr);
//     }

// }

int main(){
    Timer0 timer0;
    UART uart;
    initialisationUART();
    timer0.startTimer(0xFF);
    while(true){
        uart.transmissionUART('I');
    }
}