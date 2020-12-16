#ifndef INSTRUCTIONS_HPP
#define INSTRUCTIONS_HPP

#include <string>
#include <vector>

class Emulation;

#define INSTRUCTION_INPUTS Emulation* pThis, std::vector<std::string> ParsedInstruction
#define INSTRUCTION_EXECUTE_OK   (bool)0x1
#define INSTRUCTION_EXECUTE_BAD  (bool)0x0

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