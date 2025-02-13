#include <iostream>
#include <bitset>
#include <cstdint>
#include <sstream>


int main()
{
    setlocale(LC_ALL, "Pt_BR.UTF-8");

    // Número em hexadecimal
    std::string hexStr;
    std::cout << "Digite um número hexadecimal: ";
    std::cin >> hexStr;

    // Converter o hexadecimal para inteiro
    uint16_t hexValue = static_cast<uint16_t>(std::stoul(hexStr, nullptr, 16));

    // Converter para binário usando bitset
    std::bitset<16> binValue(hexValue); // 16 bits de comprimento (pode ser ajustado conforme necessário)

    // Exibir o valor binário
    std::cout << "O número em binário é: " << binValue << std::endl;

    std::cout << "O número em hexadecimal é: " << hexStr << std::endl;

    return 0;
}
