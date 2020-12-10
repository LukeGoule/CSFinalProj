#include "Emulation.hpp"
#include "Utils.hpp"
#include "Mappings.hpp"

Emulation::Emulation()
{
    
}

Emulation::~Emulation()
{

}

void Emulation::Reset()
{
    /*
    if (this->m_pRegisters)
    {
        delete this->m_pRegisters;
    }

    this->m_pRegisters = new EmulatedRegisters();
    */

    // Unsafe(??) memory manual memory allocation.
    if (this->m_pRegisters)
    {
        free((void*)m_pRegisters);
        m_pRegisters = nullptr;
    }

    this->m_pRegisters = (EmulatedRegisters*)malloc(sizeof(EmulatedRegisters));
}

InstructionReturn* Emulation::ExecuteSingleInstruction(std::string InstructionString)
{
    std::vector<std::string> ParsedInstruction = Utils::TokeniseInstruction(InstructionString);
    

}