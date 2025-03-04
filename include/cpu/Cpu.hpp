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

    enum OPCODE
    {
        ORTHERS = 0x0,
        MOVE = 0x1,
        STORE = 0x2,
        LOAD = 0x3,
        ULA_ADD = 0x4,
        ULA_SUB = 0x5,
        ULA_MUL = 0x6,
        ULA_AND = 0x7,
        ULA_OR = 0x8,
        ULA_NOT = 0x9,
        ULA_XOR = 0xA,
        SHIFT_R = 0xB,
        SHIFT_L = 0xC,
        ROTATE_R = 0xD,
        ROTATE_L = 0xE,
        EXIT = 0xF
    };

public:
    Cpu();
    ~Cpu();

    void NOP();
    void HALT();
    void MOV(uint16_t instruction);
    void STR();
    void LDR();
    void ADD(uint16_t instruction);
    void SUB();
    void MUL();
    void AND(uint16_t instruction);
    void OR(uint16_t instruction);
    void NOT(uint16_t instruction);
    void XOR(uint16_t instruction);
    void SHR(uint16_t instruction);
    void SHL(uint16_t instruction);
    void PSH(uint16_t instruction);
    void POP(uint16_t instruction);
    void CMP(uint16_t instruction);
    void JMP(uint16_t instruction);
    void JEQ(uint16_t instruction);
    void JLT(uint16_t instruction);
    void JGT(uint16_t instruction);
    void ROR();
    void ROL();

    void execute();
    void loadProgram(const std::string &filename);
    void displayState() const;
};
