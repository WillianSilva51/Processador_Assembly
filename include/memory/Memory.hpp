#pragma once

#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <bitset>

inline std::ostream &formatHex(std::ostream &os)
{
    return os << std::hex << std::setw(4) << std::setfill('0') << std::uppercase;
}

class Memory
{
private:
    mutable std::vector<std::pair<uint8_t, bool>> memory;

public:
    Memory();

    ~Memory();

    void write(uint16_t address, uint16_t value);

    uint16_t read(uint16_t address) const;

    void display_accessed() const;

    void display_stack() const;
};
