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
public:
    bool getData(uint64_t address) {
        return true;
    }
    int getDelay() {
        return baseDelay*(100+random(-10, 10))/100;
    }
};


#endif //MEMORY_ARCHITECTURE_DISK_H