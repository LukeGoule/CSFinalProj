#include "AQARegisters.hpp"

#include <unordered_map>
#include <stdexcept>

AQARegisters::AQARegisters()
{

}

AQARegisters::~AQARegisters()
{

}

REGISTER_VAL_SIZE* AQARegisters::GetRegisterByNumber(unsigned long long n)
{

    std::unordered_map<unsigned long long, REGISTER_VAL_SIZE*> RegisterMapping = {
        {0,     &this->_Reg0},
        {1,     &this->_Reg1},
        {2,     &this->_Reg2},
        {3,     &this->_Reg3},
        {4,     &this->_Reg4},
        {5,     &this->_Reg5},
        {6,     &this->_Reg6},
        {7,     &this->_Reg7},
        {8,     &this->_Reg8},
        {9,     &this->_Reg9},
        {10,    &this->_Reg10},
        {11,    &this->_Reg11},
    };

    // Protects from segfaulting the program.
    if (n > RegisterMapping.size() - 1)
    {
        throw std::runtime_error{ "Attempted to get invalid register!" };
    }

    return RegisterMapping.find(n)->second;

}

void AQARegisters::CompleteReset()
{
    *R0() = 0ULL;
    *R1() = 0ULL;
    *R2() = 0ULL;
    *R3() = 0ULL;
    *R4() = 0ULL;
    *R5() = 0ULL;
    *R6() = 0ULL;
    *R7() = 0ULL;
    *R8() = 0ULL;
    *R9() = 0ULL;
    *R10() = 0ULL;
    *R11() = 0ULL;

    _IP = 0ULL;
    _F_EQ = 0ULL;
    _F_NE = 0ULL;
    _F_GT = 0ULL;
    _F_LT = 0ULL;
    _F_HALT = 0ULL;
}