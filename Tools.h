#ifndef MEMORY_ARCHITECTURE_TOOLS_H
#define MEMORY_ARCHITECTURE_TOOLS_H
#include <cstdint>
#include <random>
#include <stdexcept>
#include <chrono>
inline int random(const int min, const int max)
{
    if (min > max) {
        throw std::invalid_argument("min must be less than or equal to max");
    }
    static std::random_device rd;
    static int gen(rd());

    return gen;
}
inline uint64_t unixTime ()
{
    using namespace std::chrono;
    return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
}
#endif
