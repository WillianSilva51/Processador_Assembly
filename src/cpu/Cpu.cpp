#include "cpu/Cpu.hpp"

Cpu::Cpu() : PC(0), IR(0), SP(0x8200), flags(), memory() {}

Cpu::~Cpu() {}

void Cpu::JGT(uint16_t instruction) {

    int16_t im = (instruction & 0x0FFF);

    if (im & 0x0800) {
        im |= 0xF000;
    }

    if (flags.getCarry() == 0 && flags.getZero() == 0){
        PC += im;
    }
}

void Cpu::JLT(uint16_t instruction) {
    
    int16_t im = (instruction & 0x0FFF);

    if (im & 0x0800) {
        im |= 0xF000;
    }

    if (flags.getZero() == 0 && flags.getCarry() == 1) {
        PC += im;
    }
}

void Cpu::JEQ(uint16_t instruction) {

    int16_t im = (instruction & 0x0FFF);

    if (im & 0x0800) {
        im |= 0xF000;
    }

    
    if (flags.getZero() == 1 && flags.getCarry() == 0) {
        PC += im;

        /*if (PC < 0 || PC > memory()){
            PC = (PC + MAX_MEMORY) % MAX_MEMORY;
        }*/
    }
}

void Cpu::JMP(uint16_t instruction) {

    int16_t im = (instruction & 0x0FFF);

    if (im & 0x0800) {
        im |= 0xF000;
    }

    PC += im;
}

void Cpu::CMP(uint16_t instruction) {
    uint16_t Rm = (instruction & 0x01C0) >> 6; 
    uint16_t Rn = (instruction & 0x0038) >> 3;

    flags.setFlagsCMP(REG[Rm], REG[Rn]);
}

void Cpu::ADD(uint16_t instruction)
{
    uint16_t Rd = (instruction & 0x0700) >> 8;
    uint16_t Rm = (instruction & 0x00E0) >> 7;
    uint16_t Rn = (instruction & 0x003C) >> 2;

    REG[Rd] = REG[Rm] + REG[Rn];
    flags.setFlags(REG[Rm], REG[Rn], REG[Rd], '+');
}

void Cpu::MOV(uint16_t instruction)
{
    uint16_t im_or_reg = (instruction & 0x0800) >> 11;
    uint16_t reg_dest = (instruction & 0x0700) >> 8;

    if (im_or_reg == 0)
    {
        uint16_t Rg = (instruction & 0x01E0) >> 5;
        REG[reg_dest] = REG[Rg];
    }
    else if (im_or_reg == 1)
    {
        uint16_t src = (instruction & 0x00FF);
        REG[reg_dest] = src;
    }
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
                break;

            case 2: // POP
                break;

            case 3: 
                CMP(instruction.to_ullong());
                PC++;
                break;

            case 4: 
                JMP(instruction.to_ullong());
                PC++;
                break;

            case 5:
                JEQ(instruction.to_ullong());
                PC++;
                break;

            case 6:
                JLT(instruction.to_ullong());
                PC++;
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
            PC++;
            break;
        }
        case STORE:
            break;

        case LOAD:
            break;

        case ULA_ADD:
        {
            ADD(instruction.to_ullong());
            PC++;
            break;
        }

        case ULA_SUB:
            break;

        case ULA_MUL:
            break;

        case ULA_AND:
            break;

        case ULA_OR:
            break;

        case ULA_NOT:
            break;

        case ULA_XOR:
            break;

        case SHIFT_R:
            break;

        case SHIFT_L:
            break;

        case ROTATE_R:
            break;

        case ROTATE_L:
            break;

        case EXIT:
            HALT();
            PC++;
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