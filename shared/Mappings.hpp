#pragma once

#ifndef MAPPINGS_HPP
#define MAPPINGS_HPP

#include <unordered_map>
#include <string>

#define CreateMapping(Instruction) {std::string(#Instruction), (unsigned long long)INSTR_##Instruction},

/*
    Following Specification defined at
    https://filestore.aqa.org.uk/resources/computing/AQA-75162-75172-ALI.PDF
*/

// LDR Rd, <memory ref>
// Load the value stored in the memory location specified by <memory ref> into register d.
#define INSTR_LDR     0x00000000

// STR Rd, <memory ref> 
// Store the value that is in register d into the memory location specified by <memory ref>.
#define INSTR_STR     INSTR_LDR + 1

// ADD Rd, Rn, <operand2>
// Add the value specified in <operand2> to the value in register n and store the result in register d.
#define INSTR_ADD     INSTR_LDR + 2

// SUB Rd, Rn, <operand2> 
// Subtract the value specified by <operand2> from the value in register n and store the result in register d.
#define INSTR_SUB     INSTR_LDR + 3

// MOV Rd, <operand2> 
// Copy the value specified by <operand2> into register d.
#define INSTR_MOV     INSTR_LDR + 4

// CMP Rn, <operand2> 
// Compare the value stored in register n with the value specified by <operand2>.
#define INSTR_CMP     INSTR_LDR + 5

// B <label> 
// Always branch to the instruction at position <label> in the program.
#define INSTR_B       INSTR_LDR + 6

/*
 Branch to the instruction at position <label> if the last comparison met the criterion specified by <condition>.
    Possible values for <condition> and their meanings are:
    EQ: equal to
    NE: not equal to
    GT: greater than
    LT: less than
*/
#define INSTR_BEQ     INSTR_LDR + 7
#define INSTR_BNE     INSTR_LDR + 8
#define INSTR_BGT     INSTR_LDR + 9
#define INSTR_BLT     INSTR_LDR + 10

// AND Rd, Rn, <operand2>
// Perform a bitwise logical AND operation between the value in register n and the value specified by <operand2> and store the result in register d.
#define INSTR_AND     INSTR_LDR + 11

// ORR Rd, Rn, <operand2> 
// Perform a bitwise logical OR operation between the value in register n and the value specified by <operand2> and store the result in register d.
#define INSTR_ORR     INSTR_LDR + 12

// EOR Rd, Rn, <operand2> 
// Perform a bitwise logical XOR (exclusive or) operation between the value in register n and the value specified by <operand2> and store the result in register d.
#define INSTR_EOR     INSTR_LDR + 13

// MVN Rd, <operand2>
// Perform a bitwise logical NOT operation on the value specified by <operand2> and store the result in register d.
#define INSTR_MVN     INSTR_LDR + 14

// LSL Rd, Rn, <operand2> 
// Logically shift left the value stored in register n by the number of bits specified by <operand2>  and store the result in register d.
#define INSTR_LSL     INSTR_LDR + 15

// LSR Rd, Rn, <operand2>
// Logically shift right the value stored in register n by the number of bits specified by <operand2> and store the result in register d.
#define INSTR_LSR     INSTR_LDR + 16

// Stops the execution of the program.
#define INSTR_HALT    INSTR_LDR + 17

// Outputs a single register's value
#define INSTR_OUT     INSTR_LDR + 18

// Outputs all register's values
#define INSTR_DUMP    INSTR_LDR + 19

extern std::unordered_map<std::string, unsigned long long> new_FullInstructionMapping;
extern std::unordered_map<unsigned long long, std::string> new_FullInstructionMapping_INVERSE;

#endif