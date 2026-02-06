 //
// Created by matin on 2/5/26.
//

#ifndef MEMORY_ARCHITECTURE_TOOLS_H
#define MEMORY_ARCHITECTURE_TOOLS_H

// #include <cstdint>   // uint64_t
#include <random>    // random engine and distribution
#include <stdexcept> // exceptions


inline int random(const int min, const int max)
{
    if (min > max) {
        throw std::invalid_argument("min must be less than or equal to max");
    }
    static std::random_device rd;       // seed source
    static int gen(rd());    // 64-bit Mersenne Twister

    return gen;
}


#endif //MEMORY_ARCHITECTURE_TOOLS_H