#pragma once

#include <iostream>
#include <unordered_map>
#include <cstdint>

class Memory
{
private:
    std::unordered_map<uint16_t, uint16_t> memory; // Memory

public:
    Memory();

    ~Memory();

    void write(uint16_t address, uint16_t value);

    uint16_t read(uint16_t address);

    void display_accessed() const;
};
