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
#define STR     LDR + 1

// ADD Rd, Rn, <operand2>
// Add the value specified in <operand2> to the value in register n and store the result in register d.
#define ADD     LDR + 2

// SUB Rd, Rn, <operand2> 
// Subtract the value specified by <operand2> from the value in register n and store the result in register d.
#define SUB     LDR + 3

// MOV Rd, <operand2> 
// Copy the value specified by <operand2> into register d.
#define MOV     LDR + 4

// CMP Rn, <operand2> 
// Compare the value stored in register n with the value specified by <operand2>.
#define CMP     LDR + 5

// B <label> 
// Always branch to the instruction at position <label> in the program.
#define B       LDR + 6

/*
 Branch to the instruction at position <label> if the last comparison met the criterion specified by <condition>. 
    Possible values for <condition> and their meanings are:
    EQ: equal to 
    NE: not equal to
    GT: greater than 
    LT: less than
*/
#define BEQ     LDR + 7
#define BNE     LDR + 8
#define BGT     LDR + 9
#define BLT     LDR + 10

// AND Rd, Rn, <operand2>
// Perform a bitwise logical AND operation between the value in register n and the value specified by <operand2> and store the result in register d.
#define AND     LDR + 11

// ORR Rd, Rn, <operand2> 
// Perform a bitwise logical OR operation between the value in register n and the value specified by <operand2> and store the result in register d.
#define ORR     LDR + 12

// XOR Rd, Rn, <operand2> 
// Perform a bitwise logical XOR (exclusive or) operation between the value in register n and the value specified by <operand2> and store the result in register d.
#define EOR     LDR + 13

// MVN Rd, <operand2>
// Perform a bitwise logical NOT operation on the value specified by <operand2> and store the result in register d.
#define MVN     LDR + 14

// LSL Rd, Rn, <operand2> 
// Logically shift left the value stored in register n by the number of bits specified by <operand2>  and store the result in register d.
#define LSL     LDR + 15

// LSR Rd, Rn, <operand2>
// Logically shift right the value stored in register n by the number of bits specified by <operand2> and store the result in register d.
#define LSR     LDR + 16

// Stops the execution of the program.
#define HALT    LDR + 17

// Outputs a single register's value
#define OUT     LDR + 18

// Outputs all register's values
#define DUMP    LDR + 19

extern std::unordered_map<std::string, unsigned long long> FullInstructionMapping;

#endif