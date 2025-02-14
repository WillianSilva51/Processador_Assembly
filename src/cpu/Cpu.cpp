#include "cpu/Cpu.hpp"

Cpu::Cpu() : PC(0), IR(0), SP(0x8200), flags(), memory() {}

Cpu::~Cpu() {}

void Cpu::ADD(uint16_t instruction){

    uint16_t regd = (instruction & 0x0700) >> 8;
    uint16_t regm = (instruction &0x00E0) >> 7;
    uint16_t regn = (instruction &0x000C) >> 2;

    REG[regd] = REG[regm] + REG[regn];
    displayState();
}

void Cpu::MOV(uint16_t instruction){
    
    uint16_t im_or_reg = (instruction & 0x0800) >> 11;
    uint16_t reg_dest = (instruction & 0x0700) >> 8;
    uint16_t src =  (instruction & 0x00FF);

    if(im_or_reg == 0){
        REG[reg_dest] = src;
    } else if (im_or_reg == 1){
        REG[reg_dest] =  REG[src];
    }

    displayState();
}

void Cpu::NOP()
{
    std::cout << "NOP" << std::endl;
    displayState();
}

void Cpu::HALT()
{
    std::cout << "HALT" << std::endl;
    displayState();
    exit(0);
}

void Cpu::loadProgram(const std::string &filename)
{
    std::ifstream file("codes/" + filename);

    if (!file || !file.is_open())
    {
        throw std::runtime_error("ERROR: Could not open file " + filename + ".");
    }

    std::string line;

    while (std::getline(file, line))
    {
        uint16_t address, value;

        if (sscanf(line.c_str(), "%hx: %hx", &address, &value) == 2)
            memory.write(address, value);
    }

    file.close();
}

void Cpu::displayState() const
{
    std::cout << "----------------------------" << std::endl;
    std::cout << "REGISTERS:" << std::endl;
    for (size_t i = 0; i < 8; i++)
        std::cout << "R[" << i << "]: 0x" << formatHex << REG[i] << std::endl;
    std::cout << "----------------------------" << std::endl;

    std::cout << "PC: 0x" << formatHex << PC << std::endl;
    std::cout << "IR: 0x" << formatHex << IR << std::endl;
    std::cout << "SP: 0x" << formatHex << SP << std::endl;

    std::cout << "----------------------------" << std::endl;
    memory.display_accessed();
    std::cout << "----------------------------" << std::endl;

    memory.display_stack(SP);

    std::cout << "----------------------------" << std::endl;
    flags.printFlags();
}
