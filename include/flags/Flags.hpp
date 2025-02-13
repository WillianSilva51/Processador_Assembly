#pragma once

#include <iostream>
#include <cstdint>
class Flags
{
private:
    bool C;
    bool Ov;
    bool Z;
    bool S;

public:
    Flags() : C(false), Ov(false), Z(false), S(false) {}

    ~Flags() {}

    void reset()
    {
        C = false;
        Ov = false;
        Z = false;
        S = false;
    }

    void setFlags(const uint16_t &result)
    {
        C = (result >> 0) & 1;
        Ov = (result >> 1) & 1;
        Z = (result == 0);
        S = (result & 0x8000) != 0;
    }

    bool getCarry()
    {
        return C;
    }

    bool getOverflow()
    {
        return Ov;
    }

    bool getZero()
    {
        return Z;
    }

    bool getSign()
    {
        return S;
    }

    void printFlags() const
    {
        std::cout << std::hex << "C: 0x" << C << " Ov: 0x" << Ov << " Z: 0x" << Z << " S: 0x" << S << std::endl;
    }
};