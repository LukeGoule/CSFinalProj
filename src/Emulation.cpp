#include "Emulation.hpp"
#include "Utils.hpp"
#include "Mappings.hpp"
#include "Instructions.hpp"

#include <iostream>
#include <string>

Emulation::Emulation()
{
    this->m_pMemory = new EmulatedMemory<MEMORY_SIZE>();
}

Emulation::~Emulation()
{

}

EmulatedRegisters::EmulatedRegisters()
{

}

EmulatedRegisters::~EmulatedRegisters()
{

}

REGISTER_VAL_SIZE* EmulatedRegisters::GetRegisterByNumber(unsigned long long n)
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

void Emulation::Reset()
{
    // Unsafe(??) memory manual memory allocation.
    if (this->m_pRegisters)
    {
        free((void*)m_pRegisters);
        m_pRegisters = nullptr;
    }

    this->m_pRegisters = (EmulatedRegisters*)malloc(sizeof(EmulatedRegisters));
    this->m_pMemory->ResetMemory();
}

InstructionReturn* Emulation::ExecuteSingleInstruction(std::string InstructionString)
{
    std::vector<std::string> ParsedInstruction = Utils::TokeniseInstruction(InstructionString);
    
    // Is it a label?
    if (ParsedInstruction[ParsedInstruction.size() - 1] == ":")
    {
        return nullptr;
    }

    unsigned long long uMappedInstruction = Utils::InstructionMnemonicToID(ParsedInstruction[0]);

    BoundInstruction_t pCallableBoundFunction = InstructionBindings[uMappedInstruction];

    if (!pCallableBoundFunction)
    {
        throw std::runtime_error{ "Severe error! Instructions internal binding not found!!" };
    }

    pCallableBoundFunction(this, ParsedInstruction);

    return nullptr;
}

void Emulation::Dump()
{
    for (unsigned long long i = 0; i < 12; i++)
    {
        std::cout << "R" << i << " -> " << *(this->m_pRegisters->GetRegisterByNumber(i)) << std::endl;
    }

    std::cout << "IP -> " << this->m_pRegisters->_IP << std::endl;
    std::cout << "CF -> " << this->m_pRegisters->_CF << std::endl;
}