#include <iostream>
#include <unordered_map>
#include <chrono>

#include "cpu/Cpu.hpp"

using namespace std;

int main()
{
    setlocale(LC_ALL, "Pt_BR.UTF-8");

    Cpu cpu;

    cout << "Bem-vindo ao simulador de CPU!" << endl;

    while (true)
    {
        cout << "Digite o nome do arquivo de entrada: ";
        string fileName;
        cin >> fileName;
        cin.ignore();

        try
        {
            cpu.loadProgram(fileName);
            cpu.displayState();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            time_t logTime = chrono::system_clock::to_time_t(chrono::system_clock::now());
            ofstream logFile("log/log.txt", ios::app);
            logFile << put_time(localtime(&logTime), "%Y-%m-%d %H:%M:%S") << " - " << e.what() << '\n';
            logFile.close();
        }

        cout << "Deseja carregar outro arquivo? (s/n): ";

        char response;
        cin >> response;

        switch (response)
        {
        case 's':
        case 'S':
            break;

        case 'n':
        case 'N':
            return 0;

        default:
            cout << "Opção inválida!" << endl;
            cin.ignore();
            break;
        }
    }
}

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
