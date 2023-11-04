class CRC32{

public:
    uint32_t value = 0;
    CRC32();
    void update(const void* buff, size_t len = 1);

private:
    uint32_t table[256];
    void generateTable();
};