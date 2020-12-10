#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include <unordered_map>
#include <string>

#define CreateMapping(Instruction) {std::string(#Instruction), (unsigned long long)Instruction},


/*
    Following Specification defined at
    https://filestore.aqa.org.uk/resources/computing/AQA-75162-75172-ALI.PDF
*/

// LDR Rd, <memory ref>
// Load the value stored in the memory location specified by <memory ref> into register d.
#define LDR     0x00000000

// STR Rd, <memory ref> 
// Store the value that is in register d into the memory location specified by <memory ref>.
#define STR     0x00000001

// ADD Rd, Rn, <operand2>
// Add the value specified in <operand2> to the value in register n and store the result in register d.
#define ADD     0x00000002

// SUB Rd, Rn, <operand2> 
// Subtract the value specified by <operand2> from the value in register n and store the result in register d.
#define SUB     0x00000003

// MOV Rd, <operand2> 
// Copy the value specified by <operand2> into register d.
#define MOV     0x00000004

// CMP Rn, <operand2> 
// Compare the value stored in register n with the value specified by <operand2>.
#define CMP     0x00000005

// B <label> 
// Always branch to the instruction at position <label> in the program.
#define B       0x00000006

/*
 Branch to the instruction at position <label> if the last comparison met the criterion specified by <condition>. 
    Possible values for <condition> and their meanings are:
    EQ: equal to 
    NE: not equal to
    GT: greater than 
    LT: less than
*/
#define BEQ     0x00000007
#define BNE     0x00000008
#define BGT     0x00000009
#define BLT     0x0000000A

// AND Rd, Rn, <operand2>
// Perform a bitwise logical AND operation between the value in register n and the value specified by <operand2> and store the result in register d.
#define AND     0x0000000B

// ORR Rd, Rn, <operand2> 
// Perform a bitwise logical OR operation between the value in register n and the value specified by <operand2> and store the result in register d.
#define ORR     0x0000000C

// EOR Rd, Rn, <operand2> 
// Perform a bitwise logical XOR (exclusive or) operation between the value in register n and the value specified by <operand2> and store the result in register d.
#define EOR     0x0000000D

// MVN Rd, <operand2>
// Perform a bitwise logical NOT operation on the value specified by <operand2> and store the result in register d.
#define MVN     0x0000000E

// LSL Rd, Rn, <operand2> 
// Logically shift left the value stored in register n by the number of bits specified by <operand2>  and store the result in register d.
#define LSL     0x0000000F

// LSR Rd, Rn, <operand2>
// Logically shift right the value stored in register n by the number of bits specified by <operand2> and store the result in register d.
#define LSR     0x00000010

// Stops the execution of the program.
#define HALT    0x00000011

std::unordered_map<std::string, unsigned long long> m = {
    CreateMapping(LDR)
    CreateMapping(STR)
    CreateMapping(ADD)
    CreateMapping(SUB)
    CreateMapping(MOV)
    CreateMapping(CMP)
    CreateMapping(B)
    CreateMapping(BEQ)
    CreateMapping(BNE)
    CreateMapping(BGT)
    CreateMapping(BLT)
    CreateMapping(AND)
    CreateMapping(ORR)
    CreateMapping(EOR)
    CreateMapping(MVN)
    CreateMapping(LSL)
    CreateMapping(LSR)
    CreateMapping(HALT)
};

#endif