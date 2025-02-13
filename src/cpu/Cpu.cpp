#include "cpu/Cpu.hpp"

Cpu::Cpu() : PC(0), IR(0), SP(0x8200), flags(), memory() {}

Cpu::~Cpu() {}

void Cpu::NOP()
{
    std::cout << "NOP Executado" << std::endl;
    displayState();
}

void Cpu::HALT()
{
    std::cout << "Execução encerrada pelo HALT." << std::endl;
    exit(0);
}
