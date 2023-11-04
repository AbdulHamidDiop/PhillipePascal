#include "crc.hpp"

CRC32::CRC32(){}

void CRC32::update(const void* data, uint32_t size){

    for (uint32_t i = 0; i < size; ++i) {

        const uint8_t* bytes = static_cast<const uint8_t*>(data);
        value ^= bytes[i];
        for (int j = 0; j < 8; ++j) {
            value = (value >> 1) ^ (0xEDB88320 & (-(value & 1)));
        }
    }
    value = ~value;
}

uint32_t CRC32::getValue(){
    return value;
}
