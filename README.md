# Memory Hierarchy Simulator (Qt Framework)

## Overview

This project is a simulation of a **computer memory hierarchy**, implemented in **C++ using the Qt framework**.  
It models the behavior of multi-level cache memories and their interaction with main memory and secondary storage.

The simulator is designed for educational purposes and focuses on demonstrating **computer architecture concepts**, not on emulating real hardware performance.

---

## Simulated Memory Hierarchy

The memory hierarchy is simulated as follows:

CPU
↓
L1 Cache
↓
L2 Cache
↓
L3 Cache
↓
RAM
↓
Disk


- Cache levels are implemented as **set-associative caches**
- RAM and Disk are modeled as memory levels with fixed access latency
- Replacement policies are applied only to cache levels

---

## Supported Cache Replacement Policies

The simulator supports the following cache replacement policies, as required by the project documentation:

- **LRU (Least Recently Used)**  
  Replaces the cache line that has not been accessed for the longest time.

- **FIFO (First In, First Out)**  
  Replaces the cache line that was inserted earliest into the cache set.

- **LFU (Least Frequently Used)**  
  Replaces the cache line with the lowest access frequency.

- **MRU (Most Recently Used)**  
  Replaces the cache line that was accessed most recently.

- **Random Replacement (RR)**  
  Selects a cache line randomly for replacement.

- **Second-Chance (Clock)**  
  A FIFO-based policy that gives recently accessed cache lines a second chance.

- **LFRU (Least Frequently Recently Used)**  
  A hybrid policy combining frequency-based and recency-based replacement decisions.

---

## Implementation Notes

- All cache levels use **set-associative organization**
- Replacement policies operate **within individual cache sets**
- Disk dose not use replacement policies
- The simulator prioritizes correctness and architectural clarity

---

## Simulation Output

![Simulation Output]

⚠️ **Performance Notice:**  
This implementation is **not optimized for efficiency**.  
The primary goal of this project is to correctly simulate memory hierarchy behavior rather than to achieve high performance.  
Despite this limitation, the simulator functions correctly and produces valid results.

---


## Purpose

This project was developed as part of a **Computer Architecture course** to study and analyze:
- Memory hierarchy behavior
- Cache organization
- The impact of replacement policies on performance
