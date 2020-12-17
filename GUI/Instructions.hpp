#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#include <string>
#include <vector>

class Emulation;

#define INSTRUCTION_INPUTS              Emulation* pThis, std::vector<std::string> ParsedInstruction
#define INSTRUCTION_INPUTS_PASSTHROUGH  pThis, ParsedInstruction
#define INSTRUCTION_EXECUTE_OK          (bool)0x1
#define INSTRUCTION_EXECUTE_BAD         (bool)0x0
#define INSTRUCTION_COMPARISON_BADVAL   (unsigned long long)(~0x0)

// A simple code-generation trick to reduce line count. Since I have so much code that's repeated but
// with the only difference being the actual operation being completed, I can create a "template" that
// using macros that will change the operator. The compiler will still output the same rediculous
// amount of code but it's much cleaner to read when looking at the bindings. This is actually a bit of a mess.
#define OPERATOR_INSTRUCTION_TEMPLATE(_OPERATOR_)                                                                   \
auto sRegister = ParsedInstruction[1];                                                                              \
auto sRegister2 = ParsedInstruction[3];                                                                             \
auto sOperand2 = ParsedInstruction[5];                                                                              \
auto uRegister = Utils::ExtractRegisterFromRegisterCode(sRegister);                                                 \
auto uRegister2 = Utils::ExtractRegisterFromRegisterCode(sRegister2);                                               \
auto pRegister = pThis->m_pRegisters->GetRegisterByNumber(uRegister);                                               \
auto pRegister2 = pThis->m_pRegisters->GetRegisterByNumber(uRegister2);                                             \
if (sOperand2[0] == '#') {/*decimal*/                                                                               \
    *pRegister = *pRegister _OPERATOR_ Utils::ExtractDecimalFromOperand2(sOperand2);                                \
} else if (sOperand2[0] == 'R' || sOperand2[0] == 'r') {/*register*/                                                \
    auto pRegister3 = pThis->m_pRegisters->GetRegisterByNumber(Utils::ExtractRegisterFromRegisterCode(sOperand2));  \
    *pRegister = *pRegister2 _OPERATOR_ *pRegister3;                                                                \
} else {                                                                                                            \
    throw std::runtime_error{ "Unrecognised <operand2>." };                                                         \
    return INSTRUCTION_EXECUTE_BAD;                                                                                 \
}

// Increments the instruction pointer.
#define END_INSTRUCTION     pThis->m_pRegisters->_IP++;

namespace Instructions
{
    /*
    Functions as described by AQA.
    */

    bool _LDR   (INSTRUCTION_INPUTS);
    bool _STR   (INSTRUCTION_INPUTS);
    bool _ADD   (INSTRUCTION_INPUTS);
    bool _SUB   (INSTRUCTION_INPUTS);
    bool _MOV   (INSTRUCTION_INPUTS);
    bool _CMP   (INSTRUCTION_INPUTS);
    bool _B     (INSTRUCTION_INPUTS);
    bool _BEQ   (INSTRUCTION_INPUTS);
    bool _BNE   (INSTRUCTION_INPUTS);
    bool _BGT   (INSTRUCTION_INPUTS);
    bool _BLT   (INSTRUCTION_INPUTS);
    bool _AND   (INSTRUCTION_INPUTS);
    bool _ORR   (INSTRUCTION_INPUTS);
    bool _EOR   (INSTRUCTION_INPUTS);
    bool _MVN   (INSTRUCTION_INPUTS);
    bool _LSL   (INSTRUCTION_INPUTS);
    bool _LSR   (INSTRUCTION_INPUTS);
    bool _HALT  (INSTRUCTION_INPUTS);

    /*
    Outside of AQA definitions, used for debugging.
    */

    bool _OUT(INSTRUCTION_INPUTS);
    bool _DUMP(INSTRUCTION_INPUTS);
};

typedef bool(*BoundInstruction_t)(INSTRUCTION_INPUTS);

/*
Function bindings for each of the executable functions.
Fully static so it will always retain the same memory address.
*/
extern BoundInstruction_t InstructionBindings[20];

#endif