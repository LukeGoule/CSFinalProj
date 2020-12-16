#ifndef EMULATION_HPP
#define EMULATION_HPP

#include <string>
#include <stdexcept>

/*
Based heavily off this: https://filestore.aqa.org.uk/resources/computing/AQA-75162-75172-ALI.PDF
*/

#define RETURN_VAL_SIZE     unsigned long long
#define REGISTER_VAL_SIZE   unsigned long long
#define MEMORY_SIZE         1024 * 16               // 16 KB

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

    REGISTER_VAL_SIZE* GetRegisterByNumber(unsigned long long n);

    REGISTER_VAL_SIZE _IP;
    REGISTER_VAL_SIZE _CF; // Carry Flag, set to 1 when CMP is true.
};

template<unsigned long long MemSize> class EmulatedMemory
{
    
    uint8_t* m_pRealMemory;   // Memory!! Not a string!

public:

    inline EmulatedMemory()
        : m_pRealMemory(nullptr)
    {
        this->ResetMemory();
    }

    inline ~EmulatedMemory()
    {

    }

    inline void ResetMemory()
    {
        m_pRealMemory = (uint8_t*)malloc(MemSize);
    }

    inline bool CheckAddress(std::string OperationName, unsigned long long uAddress)
    {
        if (uAddress < 0 || uAddress > MemSize)
        {
            throw std::runtime_error{ "[" + OperationName + "] Attempted access outside of useable memory!" };
        }

        return true;
    }

    inline uint8_t Read(unsigned long long uAddress)
    {
        CheckAddress("Read", uAddress);

        return m_pRealMemory[uAddress];
    }

    inline void Write(unsigned long long uAddress, uint8_t btValue)
    {
        CheckAddress("Write", uAddress);

        m_pRealMemory[uAddress] = btValue;
    }
};

class Emulation
{
public:

    EmulatedRegisters*              m_pRegisters    = nullptr;
    EmulatedMemory<MEMORY_SIZE>*    m_pMemory       = nullptr;  // TODO: Make this flexible.

    Emulation();
    ~Emulation();

    void Reset();
    void Dump();
    
    InstructionReturn* ExecuteSingleInstruction(std::string InstructionString);
};

#endif