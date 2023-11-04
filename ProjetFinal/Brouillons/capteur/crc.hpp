#include <avr/io.h>
class CRC32{
    public:
        CRC32();
        void update(const void* data, uint32_t size);
        uint32_t getValue();
    private:
        uint32_t value = 0xFFFFFFFF;
};