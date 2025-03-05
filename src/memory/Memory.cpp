#include "memory/Memory.hpp"

Memory::Memory() : memory(0xFFFF, {0, false}) {}

Memory::~Memory() {}

void Memory::write(uint16_t address, uint16_t value)
{
    if (address >= memory.size())
        throw std::range_error("Memory address out of range");

    uint8_t low_byte = value & 0xFF;
    uint8_t high_byte = (value >> 8) & 0xFF;

    memory[address] = {high_byte, false};
    memory[address + 1] = {low_byte, false};
}

uint16_t Memory::read(uint8_t address) const
{
    if (address >= memory.size())
        throw std::range_error("ERROR: Memory address out of range");

    memory[address].second = true;

    return memory[address].first;
}

void Memory::display_accessed() const
{
    if (std::all_of(memory.begin(), memory.end() - 0x8200, [](const auto &m)
                    { return !m.second; }))
    {
        std::cout << "<NO MEMORY ACCESSED>" << std::endl;
        return;
    }

    std::cout << "MEMORY ACCESSED:" << std::endl;

    for (size_t i = 0; i < memory.size(); i += 2)
    {
        if (memory[i].second)
        {
            std::bitset<16> memory_accessed = (memory[i].first << 8) | memory[i + 1].first;

            std::cout << formatHex << i << ": 0x" << formatHex << memory_accessed.to_ulong() << std::endl;
        }
    }
}

void Memory::display_stack(const uint16_t &SP) const
{
    if (std::all_of(memory.begin() + 0x8200, memory.end(), [](const auto &m)
                    { return !m.second; }))
    {
        std::cout << "<STACK NOT ACCESSED>" << std::endl;
        return;
    }

    std::cout << "STACK ACCESSED:" << std::endl;

    for (size_t i = 0x8200; i < (size_t)SP; i += 2)
    {
        if (memory[i].second)
        {
            std::bitset<16> memory_accessed = (memory[i].first << 8) | memory[i + 1].first;

            std::cout << formatHex << i << ": 0x" << formatHex << memory_accessed.to_ulong() << std::endl;
        }
    }
}