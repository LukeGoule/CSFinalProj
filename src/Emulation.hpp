#ifndef EMULATION_HPP
#define EMULATION_HPP

#include <string>

/*
Based heavily off this: https://filestore.aqa.org.uk/resources/computing/AQA-75162-75172-ALI.PDF
*/

#define RETURN_VAL_SIZE     unsigned long long
#define REGISTER_VAL_SIZE   unsigned long long

class InstructionReturn
{
    RETURN_VAL_SIZE m_uReturnValue = 0L;
public:

    InstructionReturn(RETURN_VAL_SIZE uReturnValue);
    ~InstructionReturn();

    RETURN_VAL_SIZE GetReturnValue();
};

#define RegFunc(n) REGISTER_VAL_SIZE* R##n () { return &this->_Reg##n; }

class EmulatedRegisters
{
    // "The available general purpose registers that the programmer can use are numbered 0 to 12."
    REGISTER_VAL_SIZE _Reg0;
    REGISTER_VAL_SIZE _Reg1;
    REGISTER_VAL_SIZE _Reg2;
    REGISTER_VAL_SIZE _Reg3;
    REGISTER_VAL_SIZE _Reg4;
    REGISTER_VAL_SIZE _Reg5;
    REGISTER_VAL_SIZE _Reg6;
    REGISTER_VAL_SIZE _Reg7;
    REGISTER_VAL_SIZE _Reg8;
    REGISTER_VAL_SIZE _Reg9;
    REGISTER_VAL_SIZE _Reg10;
    REGISTER_VAL_SIZE _Reg11;
    
public:

    EmulatedRegisters();
    ~EmulatedRegisters();

    RegFunc(0)
    RegFunc(1)
    RegFunc(2)
    RegFunc(3)
    RegFunc(4)
    RegFunc(5)
    RegFunc(6)
    RegFunc(7)
    RegFunc(8)
    RegFunc(9)
    RegFunc(10)
    RegFunc(11)

};

class Emulation
{
public:

    EmulatedRegisters* m_pRegisters;

    Emulation();
    ~Emulation();

    void Reset();
    
    InstructionReturn* ExecuteSingleInstruction(std::string InstructionString);
};

#endif