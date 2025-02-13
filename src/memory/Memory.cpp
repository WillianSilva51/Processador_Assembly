#include "memory/Memory.hpp"

Memory::Memory() : memory() {}

Memory::~Memory() {}

void Memory::write(uint16_t address, uint16_t value)
{
    memory[address] = value;
}

uint16_t Memory::read(uint16_t address) const
{
    if (memory.find(address) == memory.end())
    {
        throw std::range_error("Memory address out of range");
    }

    return memory.at(address);
}

void Memory::display_accessed() const
{
    if (memory.empty())
    {
        std::cout << "<NO MEMORY ACCESSED>" << std::endl;
        return;
    }

    std::cout << "MEMORY ACCESSED:" << std::endl;

    for (const auto &x : memory)
    {
        std::cout << std::uppercase << std::hex << std::setfill('0') << std::setw(4) << x.first << ": 0x" << std::setw(4) << x.second << std::endl;
    }
}