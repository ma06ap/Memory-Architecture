#ifndef MEMORY_ARCHITECTURE_MEMORY_H
#define MEMORY_ARCHITECTURE_MEMORY_H
#include <cstdint>
using namespace std;
enum Replacement {LRU = 0, FIFO = 1, RR = 2};
class Memory {
protected:
    uint64_t size;
    int baseDelay;
public:
    virtual bool getData(uint64_t address) = 0;
    virtual int getDelay() = 0;
    virtual void replace(uint64_t address) = 0;
};
#endif
