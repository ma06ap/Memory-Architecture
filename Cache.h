#ifndef MEMORY_ARCHITECTURE_CACHE_H
#define MEMORY_ARCHITECTURE_CACHE_H

#include "Memory.h"
#include "Tools.h"
#include <vector>
#include <stdexcept>

using namespace std;

class Cache : public Memory {
private:
    uint16_t blockSize;
    uint64_t numSets;
    int associativity;
    Replacement policy;
    vector<vector<pair<uint64_t, uint64_t>>> sets; // [Set][Way] -> {Time, Tag}

public:
    Cache(uint64_t size, int baseDelay, uint16_t blockSize, int assoc, Replacement policy) {
        this->size = size;
        this->baseDelay = baseDelay;
        this->blockSize = blockSize;
        this->associativity = assoc;
        this->policy = policy;
        if (size % (blockSize * assoc) != 0) {
            throw std::invalid_argument("Size structure is invalid");
        }
        this->numSets = size / (blockSize * assoc);
        sets.resize(numSets);
    }

    bool getData(uint64_t address) override {
        uint64_t blockAddr = address / blockSize;
        uint64_t setIndex = blockAddr % numSets;
        uint64_t tag = blockAddr / numSets;

        for (int i = 0; i < sets[setIndex].size(); i++) {
            if (sets[setIndex][i].second == tag) {
                if (policy == LRU) {
                    sets[setIndex][i].first = unixTime();
                }
                return true;
            }
        }
        return false;
    }

    void addBlock(uint64_t address) {
        uint64_t blockAddr = address / blockSize;
        uint64_t setIndex = blockAddr % numSets;
        uint64_t tag = blockAddr / numSets;

        if (sets[setIndex].size() < associativity) {
            sets[setIndex].push_back({unixTime(), tag});
        } else {
            replace(address);
        }
    }

    void replaceBlock(uint64_t address) {
        uint64_t blockAddr = address / blockSize;
        uint64_t setIndex = blockAddr % numSets;
        uint64_t tag = blockAddr / numSets;

        int victimIndex = 0;

        if (policy == LRU) {
            uint64_t minTime = sets[setIndex][0].first;
            for (int i = 1; i < sets[setIndex].size(); i++) {
                if (sets[setIndex][i].first < minTime) {
                    minTime = sets[setIndex][i].first;
                    victimIndex = i;
                }
            }
        }
        else if (policy == FIFO) {
            victimIndex = 0;
        }
        else if (policy == RR) {
            victimIndex = random(0, sets[setIndex].size() - 1);
        }
        if (policy == FIFO) {
            sets[setIndex].erase(sets[setIndex].begin() + victimIndex);
            sets[setIndex].push_back({unixTime(), tag});
        } else {
            sets[setIndex][victimIndex] = {unixTime(), tag};
        }
    }

    int getDelay() override {
        return baseDelay + (random(-2, 2) / 100.0 * baseDelay);
    }

    void replace(uint64_t address) override {
        replaceBlock(address);
    }
};

#endif //MEMORY_ARCHITECTURE_CACHE_H