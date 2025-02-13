#pragma once

#include <fstream>
#include <sstream>

#include "flags/Flags.hpp"
#include "memory/Memory.hpp"

class Cpu
{
private:
    uint16_t REG[8] = {0}; // R0-R7
    uint16_t PC;           // Program Counter
    uint16_t IR;           // Instruction Register
    uint16_t SP;           // Stack Pointer
    Flags flags;           // Flags Register
    Memory memory;         // Memory

    std::string formatHex(const uint16_t &value) const;

public:
    Cpu();
    ~Cpu();

    void NOP();
    void HALT();
    void MOV();
    void STR();
    void LDR();
    void ADD();
    void SUB();
    void MUL();
    bool AND();
    bool OR();
    bool NOT();
    bool XOR();
    void PSH();
    void POP();
    bool CMP();
    void JMP();
    void JEQ();
    void JLT();
    void JGT();
    void SHR();
    void SHL();
    void ROR();
    void ROL();

    void execute();
    void loadProgram(const std::string &filename);
    void displayState() const;
};
