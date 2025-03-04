#include "cpu/Cpu.hpp"

Cpu::Cpu() : PC(0), IR(0), SP(0x8200), flags(), memory() {}

Cpu::~Cpu() {}

void Cpu::PSH(uint16_t instruction)
{
    uint16_t Rn = (instruction & 0x001C) >> 2;

    memory.write(SP, Rn);
    SP--;
}

void Cpu::POP(uint16_t instruction)
{
    uint16_t Rd = (instruction & 0x0700) >> 8;

    SP++;
    REG[Rd] = memory.read(SP);
}

void Cpu::JGT(uint16_t instruction)
{
    int16_t im = (int16_t)(instruction & 0x0FFF);

    if (im & 0x0800)
    {
        im |= 0xF000;
    }

    if (flags.getCarry() == 0 && flags.getZero() == 0)
    {
        std::cout << "JGT para " << (PC + im) << std::endl;
        PC += im;
    }
}

void Cpu::JLT(uint16_t instruction)
{
    int16_t im = (int16_t)(instruction & 0x0FFF);

    if (im & 0x0800)
    {
        im |= 0xF000;
    }

    if (flags.getZero() == 0 && flags.getCarry() == 1)
    {
        std::cout << "JLT para " << (PC + im) << std::endl;
        PC += im;
    }
}

void Cpu::JEQ(uint16_t instruction)
{
    int16_t im = (int16_t)(instruction & 0x0FFF);

    if (im & 0x0800)
    {
        im |= 0xF000;
    }

    if (flags.getZero() == 1 && flags.getCarry() == 0)
    {
        std::cout << "JEQ para " << (PC + im) << std::endl;
        PC += im;
    }
}

void Cpu::JMP(uint16_t instruction)
{
    int16_t im = (int16_t)(instruction & 0x0FFF);

    if (im & 0x0800)
    {
        im |= 0xF000;
    }

    std::cout << "JMP para " << (PC + im) << std::endl;
    std::cout << "Deslocamento: " << im << std::endl;

    int16_t nextPC = PC + im;
    if (nextPC == PC)
    {
        std::cout << "Loop detectado! Salto inválido. PC não mudou." << std::endl;
        return; // Não altera o PC se o salto for inválido.
    }

    std::cout << "JMP para " << nextPC << std::endl;
    PC = nextPC;
}

void Cpu::CMP(uint16_t instruction)
{
    uint16_t Rm = (instruction & 0x00E0) >> 5; // Registrador de origem (Rm)
    uint16_t Rn = (instruction & 0x001C) >> 2; // Registrador de destino (Rn)

    uint16_t val_rm = REG[Rm]; // Valor de Rm
    uint16_t val_rn = REG[Rn]; // Valor de Rn

    // Verificando a condição de igualdade (Z = 1 se Rm == Rn)
    flags.setZeroFlag(val_rm == val_rn);

    // Verificando a condição de carry (C = 1 se Rm < Rn)
    flags.setCarryFlag(val_rm < val_rn);
}

void Cpu::ADD(uint16_t instruction)
{
    uint16_t regd = (instruction & 0x0700) >> 8; // Valor do Registrador de destino
    uint16_t regm = (instruction & 0x00E0) >> 5; // Valor do primeiro registrador
    uint16_t regn = (instruction & 0x001C) >> 2; // Valor do segundo registrador

    REG[regd] = REG[regm] + REG[regn];
    flags.setFlags(REG[regn], REG[regm], REG[regd], '+');
}

void Cpu::MOV(uint16_t instruction)
{
    uint16_t im_or_reg = (instruction & 0x0800) >> 11;
    uint16_t reg_dest = (instruction & 0x0700) >> 8;

    if (im_or_reg == 0)
    {
        uint16_t regm = (instruction & 0x01E0) >> 5;
        REG[reg_dest] = REG[regm];
    }
    else if (im_or_reg == 1)
    {
        uint16_t src = (instruction & 0x00FF);
        REG[reg_dest] = src;
    }
}

void Cpu::AND(uint16_t instruction)
{
    uint16_t regd = (instruction & 0x0700) >> 8;
    uint16_t regm = (instruction & 0x00E0) >> 7;
    uint16_t regn = (instruction & 0x003C) >> 2;

    REG[regd] = REG[regm] & REG[regn];
}

void Cpu::OR(uint16_t instruction)
{
    uint16_t regd = (instruction & 0x0700) >> 8;
    uint16_t regm = (instruction & 0x00E0) >> 7;
    uint16_t regn = (instruction & 0x003C) >> 2;

    REG[regd] = REG[regm] | REG[regn];
}

void Cpu::NOT(uint16_t instruction)
{
    uint16_t regd = (instruction & 0x0700) >> 8;
    uint16_t regm = (instruction & 0x00E0) >> 7;

    REG[regd] = ~REG[regm];
}

void Cpu::XOR(uint16_t instruction)
{
    uint16_t regd = (instruction & 0x0700) >> 8;
    uint16_t regm = (instruction & 0x00E0) >> 7;
    uint16_t regn = (instruction & 0x003C) >> 2;

    REG[regd] = REG[regm] ^ REG[regn];
}

void Cpu::SHR(uint16_t instruction)
{
    uint16_t regd = (instruction >> 8) & 7;
    uint16_t regm = (instruction >> 5) & 7;
    uint16_t n = instruction & 31;

    REG[regd] = REG[regm] >> n;
}

void Cpu::SHL(uint16_t instruction)
{
    uint16_t regd = (instruction >> 8) & 7;
    uint16_t regm = (instruction >> 5) & 7;
    uint16_t n = instruction & 31;

    REG[regd] = REG[regm] << n;
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

void Cpu::execute()
{
    while (true)
    {
        IR = memory.read(PC++);
        std::bitset<16> instruction = (IR << 8) | (memory.read(PC));

        uint16_t opcode = (IR >> 4);

        switch (opcode)
        {
        case ORTHERS:
        {
            uint8_t sub_opcode = (instruction[11] << 2) | (instruction[1] << 1) | (instruction[0]);

            switch (sub_opcode)
            {
            case 0:
                NOP();
                break;

            case 1: // PSH
                PSH(instruction.to_ullong());
                break;

            case 2:
                POP(instruction.to_ullong());
                break;

            case 3:
                CMP(instruction.to_ullong());
                PC++;
                break;

            case 4:
                JMP(instruction.to_ullong());
                break;

            case 5:
                JEQ(instruction.to_ullong());
                break;

            case 6:
                JLT(instruction.to_ullong());
                break;

            case 7:
                JGT(instruction.to_ullong());
                break;
            }
            break;
        }
        case MOVE:
        {
            MOV(instruction.to_ullong());
            break;
        }
        case STORE:
            break;

        case LOAD:
            break;

        case ULA_ADD:
        {
            ADD(instruction.to_ullong());
            break;
        }

        case ULA_SUB:
            break;

        case ULA_MUL:
            break;

        case ULA_AND:
        {
            AND(instruction.to_ullong());
            PC++;
            break;
        }
        case ULA_OR:
        {
            OR(instruction.to_ullong());
            PC++;
            break;
        }
        case ULA_NOT:
        {
            NOT(instruction.to_ullong());
            PC++;
            break;
        }
        case ULA_XOR:
        {
            XOR(instruction.to_ullong());
            PC++;
            break;
        }
        case SHIFT_R:
        {
            SHR(instruction.to_ullong());
            PC++;
            break;
        }
        case SHIFT_L:
        {
            SHR(instruction.to_ullong());
            PC++;
            break;
        }
        case ROTATE_R:
            break;

        case ROTATE_L:
            break;

        case EXIT:
            HALT();
            break;

        default:
        {
            std::stringstream ss;
            ss << std::setfill('0') << std::setw(4) << std::hex << IR;
            throw std::runtime_error("ERROR: Unknown instruction: 0x" + ss.str() + ".");
        }
        break;
        }
    }
}