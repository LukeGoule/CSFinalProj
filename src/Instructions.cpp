#include "Instructions.hpp"
#include "Emulation.hpp"
#include "Utils.hpp"

#include <iostream>

#ifdef _WIN32
#pragma warning(disable: 4244)
#endif

// This "binding" should map one to one with the instruction IDs defined in Mappings.hpp
BoundInstruction_t InstructionBindings[20] =
{
    Instructions::_LDR,
    Instructions::_STR,
    Instructions::_ADD,
    Instructions::_SUB,
    Instructions::_MOV,
    Instructions::_CMP,
    Instructions::_B,
    Instructions::_BEQ,
    Instructions::_BNE,
    Instructions::_BGT,
    Instructions::_BLT,
    Instructions::_AND,
    Instructions::_ORR,
    Instructions::_EOR,
    Instructions::_MVN,
    Instructions::_LSL,
    Instructions::_LSR,
    Instructions::_HALT,
    Instructions::_OUT,
    Instructions::_DUMP
};

// AQA Instructions.

bool Instructions::_LDR(INSTRUCTION_INPUTS)
{
    std::string sRegister = ParsedInstruction[1];
    std::string sMemoryRef = ParsedInstruction[3];

    auto uRegister = Utils::ExtractRegisterFromRegisterCode(sRegister);
    auto uMemoryRef = Utils::String2Int_SafeFail(sMemoryRef);

    auto pRegister = pThis->m_pRegisters->GetRegisterByNumber(uRegister);

    pThis->m_pMemory->Write(uMemoryRef, *pRegister);

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_STR(INSTRUCTION_INPUTS)
{
    std::string sRegister = ParsedInstruction[1];
    std::string sMemoryRef = ParsedInstruction[3];

    auto uRegister = Utils::ExtractRegisterFromRegisterCode(sRegister);
    auto uMemoryRef = Utils::String2Int_SafeFail(sMemoryRef);

    auto pRegister = pThis->m_pRegisters->GetRegisterByNumber(uRegister);

    pThis->m_pMemory->Write(uMemoryRef, *pRegister);

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_ADD(INSTRUCTION_INPUTS)
{
    // Rd = <operand2> + Rn

    auto sRegister = ParsedInstruction[1];
    auto sRegister2 = ParsedInstruction[3];
    auto sOperand2 = ParsedInstruction[5];

    auto uRegister = Utils::ExtractRegisterFromRegisterCode(sRegister);
    auto uRegister2 = Utils::ExtractRegisterFromRegisterCode(sRegister2);

    auto pRegister = pThis->m_pRegisters->GetRegisterByNumber(uRegister);
    auto pRegister2 = pThis->m_pRegisters->GetRegisterByNumber(uRegister2);

    if (sOperand2[0] == '#')
    {
        *pRegister = *pRegister + Utils::ExtractDecimalFromOperand2(sOperand2);
    }
    else if (sOperand2[0] == 'R' || sOperand2[0] == 'r')
    {
        auto pRegister3 = pThis->m_pRegisters->GetRegisterByNumber(Utils::ExtractRegisterFromRegisterCode(sOperand2));

        *pRegister = *pRegister2 + *pRegister3;
    }
    else
    {
        throw std::runtime_error{ "Unrecognised <operand2>." };   

        return INSTRUCTION_EXECUTE_BAD;
    }

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_SUB(INSTRUCTION_INPUTS)
{
    // Rd = Rn - <operand2>

    auto sRegister = ParsedInstruction[1];
    auto sRegister2 = ParsedInstruction[3];
    auto sOperand2 = ParsedInstruction[5];

    auto uRegister = Utils::ExtractRegisterFromRegisterCode(sRegister);
    auto uRegister2 = Utils::ExtractRegisterFromRegisterCode(sRegister2);

    auto pRegister = pThis->m_pRegisters->GetRegisterByNumber(uRegister);
    auto pRegister2 = pThis->m_pRegisters->GetRegisterByNumber(uRegister2);

    if (sOperand2[0] == '#')
    {
        *pRegister = *pRegister - Utils::ExtractDecimalFromOperand2(sOperand2);
    }
    else if (sOperand2[0] == 'R' || sOperand2[0] == 'r')
    {
        auto pRegister3 = pThis->m_pRegisters->GetRegisterByNumber(Utils::ExtractRegisterFromRegisterCode(sOperand2));

        *pRegister = *pRegister2 - *pRegister3;
    }
    else
    {
        throw std::runtime_error{ "Unrecognised <operand2>." };

        return INSTRUCTION_EXECUTE_BAD;
    }

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_MOV(INSTRUCTION_INPUTS)
{
    auto sRegister = ParsedInstruction[1];
    auto sOperand2 = ParsedInstruction[3];

    if (sOperand2[0] == '#') // It's a decimal number
    {
        auto uRegister = Utils::ExtractRegisterFromRegisterCode(sRegister);
        auto uValue = Utils::ExtractDecimalFromOperand2(sOperand2);

        auto pRegister = pThis->m_pRegisters->GetRegisterByNumber(uRegister);
        
        *pRegister = uValue;
    }
    else if (sOperand2[0] == 'R' || sOperand2[0] == 'r') // It's a register
    {
        auto uRegister = Utils::ExtractRegisterFromRegisterCode(sRegister);
        auto uRegister2 = Utils::ExtractRegisterFromRegisterCode(sOperand2);
        
        auto pRegister = pThis->m_pRegisters->GetRegisterByNumber(uRegister);
        auto pRegister2 = pThis->m_pRegisters->GetRegisterByNumber(uRegister2);

        *pRegister = *pRegister2;
    }
    else 
    {
        throw std::runtime_error{ "Unrecognised <operand2>." };

        return INSTRUCTION_EXECUTE_BAD;
    }

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_CMP(INSTRUCTION_INPUTS)
{
    auto sRegister = ParsedInstruction[1];
    auto sOperand2 = ParsedInstruction[3];

    auto uRegister = Utils::ExtractRegisterFromRegisterCode(sRegister);
    auto pRegister = pThis->m_pRegisters->GetRegisterByNumber(uRegister);
    
    if (sOperand2[0] == '#')
    {
        auto uValue = Utils::ExtractDecimalFromOperand2(sOperand2);

        pThis->m_pRegisters->_CF = (REGISTER_VAL_SIZE)(*pRegister == uValue);
    }
    else if (sOperand2[0] == 'R' || sOperand2[0] == 'r')
    {
        auto uRegister2 = Utils::ExtractRegisterFromRegisterCode(sOperand2);
        auto pRegister2 = pThis->m_pRegisters->GetRegisterByNumber(uRegister2);

        pThis->m_pRegisters->_CF = (REGISTER_VAL_SIZE)(*pRegister == *pRegister2);
    }
    else 
    {
        throw std::runtime_error{ "Unrecognised <operand2>." };

        return INSTRUCTION_EXECUTE_BAD;
    }

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_B(INSTRUCTION_INPUTS)
{
    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_BEQ(INSTRUCTION_INPUTS)
{
    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_BNE(INSTRUCTION_INPUTS)
{
    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_BGT(INSTRUCTION_INPUTS)
{
    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_BLT(INSTRUCTION_INPUTS)
{
    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_AND(INSTRUCTION_INPUTS)
{
    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_ORR(INSTRUCTION_INPUTS)
{
    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_EOR(INSTRUCTION_INPUTS)
{
    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_MVN(INSTRUCTION_INPUTS)
{
    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_LSL(INSTRUCTION_INPUTS)
{
    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_LSR(INSTRUCTION_INPUTS)
{
    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_HALT(INSTRUCTION_INPUTS)
{
    return INSTRUCTION_EXECUTE_OK;
}

// Non-AQA Instructions.

bool Instructions::_OUT(INSTRUCTION_INPUTS)
{
    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_DUMP(INSTRUCTION_INPUTS)
{
    pThis->Dump();

    return INSTRUCTION_EXECUTE_OK;
}