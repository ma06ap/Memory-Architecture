//
// Created by matin on 2/5/26.
//

#ifndef MEMORY_ARCHITECTURE_DISK_H
#define MEMORY_ARCHITECTURE_DISK_H
#include <cstdint>
#include "Tools.h"

using namespace std;

class Disk {
    int baseDelay;
    uint64_t size;
public:
    Disk (int baseDelay,uint64_t size) {
        this->baseDelay = baseDelay;
        this->size = size;
    }
    bool getData(uint64_t address) {
        return true;
    }
    int getDelay() {
        return baseDelay*(100-random(-5, 90))/100.0;
    }
    uint64_t getSize() { return size; }
};


#endif //MEMORY_ARCHITECTURE_DISK_H