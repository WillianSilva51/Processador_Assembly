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

void Cpu::loadProgram(const std::string &filename)
{
    std::ifstream file("src/codes/" + filename);

    if (!file || !file.is_open())
    {
        throw std::runtime_error("Erro ao abrir o arquivo");
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

std::string Cpu::formatHex(const uint16_t &value) const
{
    std::stringstream ss;
    ss << std::uppercase << std::hex << std::setfill('0') << std::setw(4) << value;
    return ss.str();
}

void Cpu::displayState() const
{
    std::cout << "----------------------------" << std::endl;
    for (size_t i = 0; i < 8; i++)
        std::cout << "R[" << i << "]: 0x" << formatHex(REG[i]) << std::endl;

    std::cout << "PC: 0x" << formatHex(PC) << std::endl;
    std::cout << "IR: 0x" << formatHex(IR) << std::endl;
    std::cout << "SP: 0x" << formatHex(SP) << std::endl;

    std::cout << "----------------------------" << std::endl;

    std::cout << "Memory:" << std::endl;
    memory.display_accessed();

    std::cout << "----------------------------" << std::endl;

    std::cout << "Stack:" << std::endl;
    // memory.display_stack();

    std::cout << "----------------------------" << std::endl;

    std::cout << "Flags:" << std::endl;
    flags.printFlags();
}
