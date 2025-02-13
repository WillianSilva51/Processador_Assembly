#include "memory/Memory.hpp"

Memory::Memory() : memory() {}

Memory::~Memory() {}

void Memory::write(uint16_t address, uint16_t value)
{
    memory[address] = value;
}

uint16_t Memory::read(uint16_t address)
{
    return memory[address];
}

void Memory::display_accessed() const
{
    for (auto const &x : memory)
        std::cout << std::hex << x.first << ": 0x" << x.second << std::endl;
}