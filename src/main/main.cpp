#include <iostream>
#include <bitset>
#include <cstdint>

#include "cpu/Cpu.hpp"

int main()
{
    setlocale(LC_ALL, "Pt_BR.UTF-8");

    Cpu cpu;

    cpu.loadProgram("code1.txt");

    cpu.displayState();

    // Número em hexadecimal
    // std::string hexStr;
    // std::cout << "Digite um número hexadecimal: ";
    // std::cin >> hexStr;

    // Converter o hexadecimal para inteiro
    // uint16_t hexValue = static_cast<uint16_t>(std::stoi(hexStr, nullptr, 16));

    // Converter para binário usando bitset
    // std::bitset<16> binValue(hexValue); // 16 bits de comprimento (pode ser ajustado conforme necessário)

    // Exibir o valor binário
    //  std::cout << "O número em binário é: " << binValue << std::endl;

    // std::cout << "O número em hexadecimal é: " << hexStr << std::endl;

    return 0;
}

/**
 *     uint16_t value = 0x731;

    uint16_t mask = 0x71F;

    uint16_t masked = value & mask;

    uint16_t result = (masked >> 3);

    cout << "Value: " << hex << value << endl;

    cout << "Mask: " << hex << mask << endl;

    cout << "Masked: " << hex << masked << endl;

    cout << "Result: " << hex << result << endl;
 */
