#pragma once

#ifndef EMULATEDREGISTERS_HPP
#define EMULATEDREGISTERS_HPP

#define RETURN_VAL_SIZE         unsigned long long
#define REGISTER_VAL_SIZE       unsigned long long
#define REGISTER_DEFAULT_VALUE  (REGISTER_VAL_SIZE)0L
#define MEMORY_ADDRESS_SIZE     unsigned long long
#define MEMORY_SIZE             1024 * 16               // 16 KB

// RegFunc is the quick getter/setter definer.
// Returns a pointer which may be modified in any way.
#define RegFunc(n)          REGISTER_VAL_SIZE* R##n () { return &this->_Reg##n; }


class AQARegisters
{
    // "The available general purpose registers that the programmer can use are numbered 0 to 12."
    REGISTER_VAL_SIZE _Reg0 = REGISTER_DEFAULT_VALUE;
    REGISTER_VAL_SIZE _Reg1 = REGISTER_DEFAULT_VALUE;
    REGISTER_VAL_SIZE _Reg2 = REGISTER_DEFAULT_VALUE;
    REGISTER_VAL_SIZE _Reg3 = REGISTER_DEFAULT_VALUE;
    REGISTER_VAL_SIZE _Reg4 = REGISTER_DEFAULT_VALUE;
    REGISTER_VAL_SIZE _Reg5 = REGISTER_DEFAULT_VALUE;
    REGISTER_VAL_SIZE _Reg6 = REGISTER_DEFAULT_VALUE;
    REGISTER_VAL_SIZE _Reg7 = REGISTER_DEFAULT_VALUE;
    REGISTER_VAL_SIZE _Reg8 = REGISTER_DEFAULT_VALUE;
    REGISTER_VAL_SIZE _Reg9 = REGISTER_DEFAULT_VALUE;
    REGISTER_VAL_SIZE _Reg10 = REGISTER_DEFAULT_VALUE;
    REGISTER_VAL_SIZE _Reg11 = REGISTER_DEFAULT_VALUE;

public:

    // Constructor
    AQARegisters();

    // Destructor
    ~AQARegisters();

    // Getter / setter for R0
    RegFunc(0);

    // getter / setter for R1
    RegFunc(1);

    // Getter / setter for R2
    RegFunc(2);

    // Getter / setter for R3
    RegFunc(3);

    // Getter / setter for R4
    RegFunc(4);

    // Getter / setter for R5
    RegFunc(5);

    // Getter / setter for R6
    RegFunc(6);

    // Getter / setter for R7
    RegFunc(7);

    // Getter / setter for R8
    RegFunc(8);

    // Getter / setter for R9
    RegFunc(9);

    // Getter / setter for R10
    RegFunc(10);

    // Getter / setter for R11
    RegFunc(11);

    // Returns the pointer to a register by it's register "ID".
    REGISTER_VAL_SIZE* GetRegisterByNumber(unsigned long long n);

    // Instruction pointer.
    REGISTER_VAL_SIZE _IP = REGISTER_DEFAULT_VALUE;


    /* Possible comparison returns. */

    // Equal to
    REGISTER_VAL_SIZE _F_EQ = REGISTER_DEFAULT_VALUE;

    // Not equal to
    REGISTER_VAL_SIZE _F_NE = REGISTER_DEFAULT_VALUE;

    // Greater than
    REGISTER_VAL_SIZE _F_GT = REGISTER_DEFAULT_VALUE;

    // Less than
    REGISTER_VAL_SIZE _F_LT = REGISTER_DEFAULT_VALUE;

    // Halt condition
    REGISTER_VAL_SIZE _F_HALT = REGISTER_DEFAULT_VALUE;

    void CompleteReset();
};

#endif