//
// Created by matin on 2/6/26.
//

#ifndef MEMORY_ARCHITECTURE_RAM_H
#define MEMORY_ARCHITECTURE_RAM_H

#include "Memory.h"
#include <vector>
#include <algorithm>

#include "Tools.h"

using namespace std;

enum Replacement {LRU = 0, FIFO = 1, Random = 2};

class RAM : public Memory {
private:
    uint16_t pageSize;
    uint64_t numPages;
    Replacement policy;
    vector<pair<uint32_t,uint64_t>> pages;
public:
    RAM(uint64_t size, int baseDelay, uint16_t pageSize, Replacement policy) {
        this->size = size;
        this->baseDelay = baseDelay;
        this->pageSize = pageSize;
        if (size%pageSize != 0) {
            throw std::invalid_argument("size must be a multiple of pageSize");
        }
        this->numPages = size / pageSize;
        this->policy = policy;
    }
    bool getData(uint64_t address) override {
        uint64_t pageNumber = address / pageSize;
        for (uint64_t i = 0; i < this->numPages; i++) {
            if (pages[i].second == pageNumber) {
                return true;
            }
        }
        return false;
    }
    void addPage(uint64_t address) {
        if (pages.size() < numPages) {
            pages.push_back({unixTime(), address/pageSize});
        }
        else {
            // replace
        }
    }
    void replacePage(uint64_t address) {
        uint64_t pageNumber = address / pageSize;
        // switch (policy) {
            if (policy == LRU) {
                // find the least recently used page
                uint32_t lruTime = pages[0].first;
                for (auto& page : pages) {
                    if (page.first < lruTime) {
                        lruTime = page.first;
                    }
                }
                for (auto& page : pages) {
                    if (page.first == lruTime) {
                        page.second = pageNumber;
                        page.first = unixTime();
                        break;
                    }
                }
            } else if (policy == FIFO) {
                // case FIFO:
                // find the first page that was added
                pages.erase(pages.begin());
                pages.push_back({unixTime(), pageNumber});
                // break;
            } else if (policy == Random) {
            // case Random:
                // randomly select a page to replace
                // break;
                pages.erase(pages.begin() + random(0, pages.size() - 1));
                pages.push_back({unixTime(), pageNumber});
            }
    }
    int getDelay() override {
        return baseDelay+(random(-10, 10))/100.0*baseDelay;
    }
    void replace(uint64_t address) override {
        // RAM does not need to replace data, so this function can be left empty
    }
};


#endif //MEMORY_ARCHITECTURE_RAM_H