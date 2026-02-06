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



class RAM : public Memory {
private:
    uint16_t pageSize;
    uint64_t numPages;
    Replacement policy;
    vector<pair<uint64_t,uint64_t>> pages;
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
        for (uint64_t i = 0; i < this->pages.size(); i++) {
            if (pages[i].second == pageNumber) {
                pages[i].first = unixTime();
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
            replace(address);
        }
    }
    void replacePage(uint64_t address) {
        uint64_t pageNumber = address / pageSize;
            if (policy == LRU) {
                uint64_t lruTime = pages[0].first;
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
                pages.erase(pages.begin());
                pages.push_back({unixTime(), pageNumber});
            } else if (policy == RR) {
                pages.erase(pages.begin() + random(0, pages.size() - 1));
                pages.push_back({unixTime(), pageNumber});
            }
    }
    int getDelay() override {
        return baseDelay+(random(-10, 10))/100.0*baseDelay;
    }
    void replace(uint64_t address) override {
        replacePage(address);
    }
};


#endif //MEMORY_ARCHITECTURE_RAM_H