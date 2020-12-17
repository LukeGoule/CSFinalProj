#include "Instructions.hpp"
#include "Emulation.hpp"
#include "Utils.hpp"

#include <iostream>
#include <Windows.h>

#ifdef _WIN32
#pragma warning(disable: 4244)
#endif                                                                                                        \

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

// Returns either a newly allocated uint64_t if the user has entered a new int (i.e. #100) or it returns
// the address of a register if the user enters something such as R0, R1, .. etc etc.
REGISTER_VAL_SIZE* Operand2Value(INSTRUCTION_INPUTS)
{
    auto sOperand2 = ParsedInstruction[3];

    if (sOperand2[0] == '#') // decimal
    {
        auto uValue = Utils::ExtractDecimalFromOperand2(sOperand2);

        // Pull the int off the stack and place it into a new memory address, return the memory address.
        // Saves us from doing "return &uValue". Which would be pretty dumb.
        REGISTER_VAL_SIZE* pReturnInt = new (REGISTER_VAL_SIZE);
        *pReturnInt = uValue;

        return pReturnInt;
    }
    else if (sOperand2[0] == 'R' || sOperand2[0] == 'r') // register
    {
        auto uRegister2 = Utils::ExtractRegisterFromRegisterCode(sOperand2);
        auto pRegister2 = pThis->m_pRegisters->GetRegisterByNumber(uRegister2);

        return pRegister2;
    }
    else
    {
        throw std::runtime_error{ "Unrecognised <operand2>." };
    }

    return nullptr; // Unreachable code!
}


// AQA Instructions.

bool Instructions::_LDR(INSTRUCTION_INPUTS) // LDR Rd, <memory ref>
{
    std::string sRegister = ParsedInstruction[1];
    std::string sMemoryRef = ParsedInstruction[3];

    auto uRegister = Utils::ExtractRegisterFromRegisterCode(sRegister);
    auto uMemoryRef = Utils::String2Int_SafeFail(sMemoryRef);

    auto pRegister = pThis->m_pRegisters->GetRegisterByNumber(uRegister);

    pThis->m_pMemory->Write(uMemoryRef, *pRegister);

    END_INSTRUCTION;

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_STR(INSTRUCTION_INPUTS) // STR Rd, <memory ref> 
{
    std::string sRegister = ParsedInstruction[1];
    std::string sMemoryRef = ParsedInstruction[3];

    auto uRegister = Utils::ExtractRegisterFromRegisterCode(sRegister);
    auto uMemoryRef = Utils::String2Int_SafeFail(sMemoryRef);

    auto pRegister = pThis->m_pRegisters->GetRegisterByNumber(uRegister);

    pThis->m_pMemory->Write(uMemoryRef, *pRegister);

    END_INSTRUCTION;

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_ADD(INSTRUCTION_INPUTS) // ADD Rd, Rn, <operand2>
{
    // Rd = <operand2> + Rn

    OPERATOR_INSTRUCTION_TEMPLATE(+);

    END_INSTRUCTION;

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_SUB(INSTRUCTION_INPUTS) // SUB Rd, Rn, <operand2>
{
    // Rd = Rn - <operand2>

    OPERATOR_INSTRUCTION_TEMPLATE(-);

    END_INSTRUCTION;

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_MOV(INSTRUCTION_INPUTS) // MOV Rd, <operand2> 
{
    auto sRegister = ParsedInstruction[1];
    
    auto pOperand = Operand2Value(INSTRUCTION_INPUTS_PASSTHROUGH);

    auto uRegister = Utils::ExtractRegisterFromRegisterCode(sRegister);
    auto pRegister = pThis->m_pRegisters->GetRegisterByNumber(uRegister);

    *pRegister = *pOperand;

    END_INSTRUCTION;

    return INSTRUCTION_EXECUTE_OK;
}

// Completes four comparisons in real ASM when doing one comparison emulation. Inefficient.
void CreateComparisonOutput(Emulation* pEmulation, unsigned long long Value0, unsigned long long Value1)
{
    pEmulation->m_pRegisters->_F_EQ = (Value0 == Value1);
    pEmulation->m_pRegisters->_F_NE = (Value0 != Value1);
    pEmulation->m_pRegisters->_F_LT = (Value0 < Value1);
    pEmulation->m_pRegisters->_F_GT = (Value0 > Value1);
}

// Sets all comparison "registers" to impossibly large values.
// Even using 4 as a value would work, but for certainty I have chosen the max value of a 64 bit integer.
void ResetComparisonOutput(Emulation* pEmulation)
{
    pEmulation->m_pRegisters->_F_EQ = INSTRUCTION_COMPARISON_BADVAL;
    pEmulation->m_pRegisters->_F_NE = INSTRUCTION_COMPARISON_BADVAL;
    pEmulation->m_pRegisters->_F_LT = INSTRUCTION_COMPARISON_BADVAL;
    pEmulation->m_pRegisters->_F_GT = INSTRUCTION_COMPARISON_BADVAL;
}

bool Instructions::_CMP(INSTRUCTION_INPUTS) // CMP Rn, <operand2>
{
    auto sRegister = ParsedInstruction[1];

    auto pOperand = Operand2Value(INSTRUCTION_INPUTS_PASSTHROUGH);

    auto uRegister = Utils::ExtractRegisterFromRegisterCode(sRegister);
    auto pRegister = pThis->m_pRegisters->GetRegisterByNumber(uRegister);

    CreateComparisonOutput(pThis, *pRegister, *pOperand);

    END_INSTRUCTION;

    return INSTRUCTION_EXECUTE_OK;
}

bool LabelJump(INSTRUCTION_INPUTS)
{
    
    auto sLabel = ParsedInstruction[1];

    if (sLabel.size() == 0)
    {
        throw std::runtime_error{ "Attempted to jump to a label that doesn't exist!" };

        ResetComparisonOutput(pThis);
        return INSTRUCTION_EXECUTE_BAD;
    }

    auto PossibleLabelMapping = pThis->m_pLabelManager->m_LabelMap.find(sLabel);

    if (PossibleLabelMapping == pThis->m_pLabelManager->m_LabelMap.end())
    {
        throw std::runtime_error{ "Failed to find label " + sLabel };

        ResetComparisonOutput(pThis);
    }
    
    auto LabelAddress = PossibleLabelMapping->second;

    pThis->m_pRegisters->_IP = LabelAddress;
    ResetComparisonOutput(pThis);

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_B(INSTRUCTION_INPUTS) // B <label> 
{
    return LabelJump(INSTRUCTION_INPUTS_PASSTHROUGH);
}

#pragma region CONDITIONAL_JUMPING

/*
B<condition> <label>
Branch to the instruction at position <label> if the last comparison met the criterion specified by <condition>.
    Possible values for <condition> and their meanings are:
    EQ: equal to
    NE: not equal to
    GT: greater than
    LT: less than
*/

bool Instructions::_BEQ(INSTRUCTION_INPUTS) // EQ: equal to
{
    if (pThis->m_pRegisters->_F_EQ)
    {
        return LabelJump(INSTRUCTION_INPUTS_PASSTHROUGH);
    }

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_BNE(INSTRUCTION_INPUTS) // NE: not equal to
{

    if (pThis->m_pRegisters->_F_NE)
    {
        return LabelJump(INSTRUCTION_INPUTS_PASSTHROUGH);
    }

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_BGT(INSTRUCTION_INPUTS) // GT: greater than
{
    if (pThis->m_pRegisters->_F_GT)
    {
        return LabelJump(INSTRUCTION_INPUTS_PASSTHROUGH);
    }

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_BLT(INSTRUCTION_INPUTS) // LT: less than
{
    if (pThis->m_pRegisters->_F_LT)
    {
        return LabelJump(INSTRUCTION_INPUTS_PASSTHROUGH);
    }

    return INSTRUCTION_EXECUTE_OK;
}

#pragma endregion

bool Instructions::_AND(INSTRUCTION_INPUTS) // AND Rd, Rn, <operand2>
{
    // Rd = Rn & <operand2>

    OPERATOR_INSTRUCTION_TEMPLATE(&);

    END_INSTRUCTION;

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_ORR(INSTRUCTION_INPUTS) // ORR Rd, Rn, <operand2> 
{
    // Rd = Rn | <operand2>

    OPERATOR_INSTRUCTION_TEMPLATE(|);

    END_INSTRUCTION;

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_EOR(INSTRUCTION_INPUTS) // EOR Rd, Rn, <operand2> 
{
    // Rd = Rn ^ <operand2>
    OPERATOR_INSTRUCTION_TEMPLATE(^);

    END_INSTRUCTION;

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_MVN(INSTRUCTION_INPUTS) // MVN Rd, <operand2>
{
    // Rd = ~<operand2>

    auto sRegister = ParsedInstruction[1];
    auto sOperand2 = ParsedInstruction[3];

    if (sOperand2[0] == '#') // decimal
    {
        auto uRegister = Utils::ExtractRegisterFromRegisterCode(sRegister);
        auto uValue = Utils::ExtractDecimalFromOperand2(sOperand2);

        auto pRegister = pThis->m_pRegisters->GetRegisterByNumber(uRegister);

        *pRegister = ~(uValue);
    }
    else if (sOperand2[0] == 'R' || sOperand2[0] == 'r') // register
    {
        auto uRegister = Utils::ExtractRegisterFromRegisterCode(sRegister);
        auto uRegister2 = Utils::ExtractRegisterFromRegisterCode(sOperand2);

        auto pRegister = pThis->m_pRegisters->GetRegisterByNumber(uRegister);
        auto pRegister2 = pThis->m_pRegisters->GetRegisterByNumber(uRegister2);

        *pRegister = ~(*pRegister2);
    }
    else
    {
        throw std::runtime_error{ "Unrecognised <operand2>." };

        return INSTRUCTION_EXECUTE_BAD;
    }
    
    END_INSTRUCTION;

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_LSL(INSTRUCTION_INPUTS) // LSL Rd, Rn, <operand2>
{
    // Rd = Rn << <operand2>
    OPERATOR_INSTRUCTION_TEMPLATE(<<);

    END_INSTRUCTION;

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_LSR(INSTRUCTION_INPUTS) // LSR Rd, Rn, <operand2>
{
    // Rd = Rn >> <operand2>
    OPERATOR_INSTRUCTION_TEMPLATE(>>);

    END_INSTRUCTION;

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_HALT(INSTRUCTION_INPUTS) // Stops the execution of the program.
{
    // Program's exit code is whatever is stored in R0.
    exit(*pThis->m_pRegisters->R0());

    END_INSTRUCTION;

    return INSTRUCTION_EXECUTE_OK;
}

// Non-AQA Instructions.

bool Instructions::_OUT(INSTRUCTION_INPUTS) // Outputs a single register's value
{
    auto sRegister = ParsedInstruction[1];
    auto uRegister = Utils::ExtractRegisterFromRegisterCode(sRegister);
    auto pRegister = pThis->m_pRegisters->GetRegisterByNumber(uRegister);

    std::cout << "R" << uRegister << " = " << *pRegister << std::endl;

    END_INSTRUCTION;

    return INSTRUCTION_EXECUTE_OK;
}

bool Instructions::_DUMP(INSTRUCTION_INPUTS) // Outputs all register's values
{
    pThis->Dump();

    END_INSTRUCTION;

    return INSTRUCTION_EXECUTE_OK;
}