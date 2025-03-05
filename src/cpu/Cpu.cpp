#include "cpu/Cpu.hpp"

Cpu::Cpu() : PC(0), IR(0), SP(0x8200), flags(), memory() {}

Cpu::~Cpu() {}

void Cpu::printRegisters()
{
    std::cout << "----------------------------\n";
    std::cout << "REGISTERS:\n";
    for (int i = 0; i < 8; i++)
    {
        std::cout << "R[" << i << "]: 0x"
                  << std::setw(4) << std::setfill('0') << std::hex << std::uppercase
                  << REG[i] << "\n";
    }
    std::cout << "----------------------------\n";
    std::cout << "PC: 0x" << std::setw(4) << std::setfill('0') << std::hex << PC << "\n";
    std::cout << "IR: 0x" << std::setw(4) << std::setfill('0') << std::hex << IR << "\n";
    std::cout << "SP: 0x" << std::setw(4) << std::setfill('0') << std::hex << SP << "\n";
    std::cout << "----------------------------\n";
}

void Cpu::ROL(uint16_t instruction) {
    uint16_t Rd = (instruction & 0x0700) >> 8;
    uint16_t Rm = (instruction & 0x00E0) >> 5;

    bool carry_in = (REG[Rm] >> 15) & 1;

    REG[Rd] = (REG[Rm] << 1) | (carry_in);

    flags.setCarryFlag (carry_in);
    flags.setZeroFlag(REG[Rd]);
    flags.setSignFlag(REG[Rd]);
}

void Cpu::ROR(uint16_t instruction) {
    uint16_t Rd = (instruction & 0x0700) >> 8;
    uint16_t Rm = (instruction & 0x00E0) >> 5;

    bool carry_in = REG[Rm] & 1;

    REG[Rd] = (REG[Rm] >> 1) | (carry_in << 15);

    flags.setCarryFlag (carry_in);
    flags.setZeroFlag(REG[Rd]);
    flags.setSignFlag(REG[Rd]);
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

void Cpu::PSH(uint16_t instruction)
{

    if (SP <= 0x81F0)
    {
        return;
    }

    uint16_t Rn = (instruction & 0x001C) >> 2;

    memory.write(SP, REG[Rn]);
    SP -= 2;
}

void Cpu::POP(uint16_t instruction)
{
    if (SP >= 0x8200)
    {
        return;
    }

    uint16_t Rd = (instruction & 0x0700) >> 8;

    SP += 2;
    REG[Rd] = memory.read(SP);
}

void Cpu::JMP(uint16_t instruction)
{
    uint16_t im = (instruction & 0x0FFF) >> 2;

    if ((im & (1 << 8)) != 0)
    {
        im |= 0xFE00;
    }

    uint16_t nextPC = PC + im;
    if (nextPC == PC)
    {
        return;
    }

    PC = nextPC;
}

void Cpu::JGT(uint16_t instruction)
{
    if (flags.getCarry() == 0 && flags.getZero() == 0)
    {
        JMP(instruction);
    }
}

void Cpu::JLT(uint16_t instruction)
{
    if (flags.getZero() == 0 && flags.getCarry() == 1)
    {
        JMP(instruction);
    }
}

void Cpu::JEQ(uint16_t instruction)
{
    if (flags.getZero() == 1 && flags.getCarry() == 0)
    {
        JMP(instruction);
    }
}

void Cpu::CMP(uint16_t instruction)
{
    uint16_t Rm = (instruction & 0x00E0) >> 5;
    uint16_t Rn = (instruction & 0x001C) >> 2; 

    uint16_t val_rm = REG[Rm]; 
    uint16_t val_rn = REG[Rn]; 

    flags.setZeroFlag(val_rm == val_rn);
    flags.setCarryFlag(val_rm < val_rn);
}

void Cpu::ADD(uint16_t instruction)
{
    uint16_t regd = (instruction & 0x0700) >> 8; // Valor do Registrador de destino
    uint16_t regm = (instruction & 0x00E0) >> 5; // Valor do primeiro registrador
    uint16_t regn = (instruction & 0x001C) >> 2; // Valor do segundo registrador

    REG[regd] = REG[regm] + REG[regn];
    flags.setFlags(REG[regm], REG[regn], REG[regd], '+');
}

void Cpu::SUB(uint16_t instruction)
{
    uint16_t regd = (instruction & 0x0700) >> 8;
    uint16_t regm = (instruction & 0x00E0) >> 5;
    uint16_t regn = (instruction & 0x001C) >> 2;

    REG[regd] = REG[regm] - REG[regn];

    flags.setFlags(REG[regm], REG[regn], REG[regd], '-');
}

void Cpu::MUL(uint16_t instruction)
{
    uint16_t regd = (instruction & 0x0700) >> 8;
    uint16_t regm = (instruction & 0x00E0) >> 5; 
    uint16_t regn = (instruction & 0x001C) >> 2; 

    REG[regd] = REG[regm] * REG[regn];

    flags.setFlags(REG[regm], REG[regn], REG[regd], '*');
}

void Cpu::MOV(uint16_t instruction)
{
    uint16_t im_or_reg = (instruction & 0x0800) >> 11;
    uint16_t reg_dest = (instruction & 0x0700) >> 8;

    if (im_or_reg == 0)
    {
        uint16_t regm = (instruction & 0x00E0) >> 5;
        REG[reg_dest] = REG[regm];
    }
    else if (im_or_reg == 1)
    {
        int16_t src = (int16_t)(instruction & 0x00FF);
        REG[reg_dest] = src;
    }
}

void Cpu::STR(uint16_t instruction)
{
    uint16_t regm = (instruction & 0x0700) >> 8;
    uint16_t regn = (instruction & 0x00E0) >> 5;

    memory.write(REG[regn], REG[regm]);
}

void Cpu::LDR(uint16_t instruction)
{
    uint16_t regd = (instruction & 0x0700) >> 8;
    uint16_t addr_reg = (instruction & 0x00E0) >> 5;
    uint16_t offset = (instruction & 0x001C) >> 2;

    uint16_t addr = REG[addr_reg] + offset;
    REG[regd] = memory.read(addr);
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
    uint16_t address, value;

    while (std::getline(file, line))
    {
        if (sscanf(line.c_str(), "%hx: %hx", &address, &value) == 2)
            memory.write(address, value);
    }

    final_Address = address;

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
        if (PC >= final_Address)
        {
            std::cout << "Programa finalizado!" << std::endl;
            displayState();
            break;
        }

        IR = memory.read(PC++);
        std::bitset<16> instruction = (IR << 8) | (memory.read(PC++));

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

            case 1:
                PSH(instruction.to_ullong());
                break;

            case 2:
                POP(instruction.to_ullong());
                break;

            case 3:
                CMP(instruction.to_ullong());
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
        {
            STR(instruction.to_ullong());
            break;
        }
        case LOAD:
        {
            LDR(instruction.to_ullong());
            break;
        }
        case ULA_ADD:
        {
            ADD(instruction.to_ullong());
            break;
        }
        case ULA_SUB:
        {
            SUB(instruction.to_ullong());
            break;
        }
        case ULA_MUL:
        {
            MUL(instruction.to_ullong());
            break;
        }
        case ULA_AND:
        {
            AND(instruction.to_ullong());
            break;
        }
        case ULA_OR:
        {
            OR(instruction.to_ullong());
            break;
        }
        case ULA_NOT:
        {
            NOT(instruction.to_ullong());
            break;
        }
        case ULA_XOR:
        {
            XOR(instruction.to_ullong());
            break;
        }
        case SHIFT_R:
        {
            SHR(instruction.to_ullong());
            break;
        }
        case SHIFT_L:
        {
            SHR(instruction.to_ullong());
            break;
        }
        case ROTATE_R:
        {
            // ROR(instruction.to_ullong());
            break;
        }
        case ROTATE_L:
        {
            // ROL(instruction.to_ullong());
            break;
        }
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