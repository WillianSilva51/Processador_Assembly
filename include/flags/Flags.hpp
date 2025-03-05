#pragma once

#include <iostream>
#include <cstdint>
#include <bitset>
class Flags
{
private:
    std::bitset<4> flags;

    enum FlagsEnum
    {
        CARRY,
        OVERFLOW,
        ZERO,
        SIGN
    };

public:
    Flags() : flags(0) {}

    ~Flags() {}

    void reset()
    {
        flags.reset();
    }

    void setFlags(const uint16_t &a, const uint16_t &b, const uint16_t &result, char operation)
    {
        // Carry ocorre se houver um estouro no bit 16 (soma/subtração)
        flags[CARRY] = (operation == '+' && result < a) || (operation == '-' && a < b) || (operation == '*' && result < a);
        // 8000  0 8000
        // Overflow ocorre se dois números positivos resultam em negativo ou vice-versa
        flags[OVERFLOW] = ((a & 0x8000) != (b & 0x8000)) && ((a & 0x8000) != (result & 0x8000));

        // Zero flag: Se o resultado for zero
        flags[ZERO] = (result == 0);

        // Sign flag: Se o bit mais significativo for 1 (negativo em complemento de dois)
        flags[SIGN] = (result & 0x8000) != 0;
    }

    void setSignFlag(uint16_t val){
        flags[SIGN] = (val >> 15) & 1;
    }
    
    void setZeroFlag(bool val) {
        flags[ZERO] = (val) ? 1 : 0;
    }
    
    void setCarryFlag(bool val) {
        flags[CARRY] = (val != 0) ? 1 : 0;
    }
    
    
    bool getCarry() const
    {
        return flags[CARRY];
    }

    bool getOverflow() const
    {
        return flags[OVERFLOW];
    }

    bool getZero() const
    {
        return flags[ZERO];
    }

    bool getSign() const
    {
        return flags[SIGN];
    }

    void printFlags() const
    {
        std::cout << "FLAGS:" << std::endl;
        std::cout << "C: " << flags[CARRY] << " Ov: " << flags[OVERFLOW] << " Z: " << flags[ZERO] << " S: " << flags[SIGN] << std::endl;
    }
};

/**
    void setFlags(const uint16_t &result)
    {
        flags[CARRY] = (result >> 0) & 1;
        flags[OVERFLOW] = (result >> 1) & 1;
        flags[ZERO] = (result == 0);
        flags[SIGN] = (result & 0x8000) != 0;
    }
};
 */