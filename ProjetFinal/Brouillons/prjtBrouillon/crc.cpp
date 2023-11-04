#include <iostream>
#include "crc.hpp"
using namespace std;

CRC32::CRC32(){
    generateTable();
}

void CRC32::update(const void* buff, size_t len){
    value ^=0xFFFFFFFF;
    const uint8_t* u = static_cast<const uint8_t*>(buff);

    for (size_t i = 0; i < len; ++i){
        value = table[(value ^ u[i]) & 0xFF] ^ (value >> 8);
    }
    value ^=0xFFFFFFFF;
}

void CRC32::generateTable(){
    uint32_t polynomial = 0xEDB88320;

    for (uint32_t i = 0; i < 256; i++){
        uint32_t c = i;
        for (size_t j = 0; j < 8; j++){
            if (c & 1) {
                c = polynomial ^ (c >> 1);
            }
            else {
                c >>= 1;
            }
        } 
        table[i] = c;
    }
}

int main(){
    CRC32 crc32 = CRC32();
    uint8_t byte = '\x02';
    uint8_t* bytePtr = &byte;
    crc32.update(bytePtr); 
    cout << crc32.value << endl;

}



