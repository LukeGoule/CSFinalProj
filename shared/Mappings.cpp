#include "Mappings.hpp"

std::unordered_map<std::string, unsigned long long> new_FullInstructionMapping = {
    {"LDR",     INSTR_LDR},
    {"STR",     INSTR_STR},
    {"ADD",     INSTR_ADD},
    {"SUB",     INSTR_SUB},
    {"MOV",     INSTR_MOV},
    {"CMP",     INSTR_CMP},
    {"B",       INSTR_B},
    {"BEQ",     INSTR_BEQ},
    {"BNE",     INSTR_BNE},
    {"BGT",     INSTR_BGT},
    {"BLT",     INSTR_BLT},
    {"AND",     INSTR_AND},
    {"ORR",     INSTR_ORR},
    {"EOR",     INSTR_EOR},
    {"MVN",     INSTR_MVN},
    {"LSL",     INSTR_LSL},
    {"LSR",     INSTR_LSR},
    {"HALT",    INSTR_HALT},
    {"OUT",     INSTR_OUT},
    {"DUMP",    INSTR_DUMP}
};

std::unordered_map<unsigned long long, std::string> new_FullInstructionMapping_INVERSE = {
    {INSTR_LDR,     "LDR"},
    {INSTR_STR,     "STR"},
    {INSTR_ADD,     "ADD"},
    {INSTR_SUB,     "SUB"},
    {INSTR_MOV,     "MOV"},
    {INSTR_CMP,     "CMP"},
    {INSTR_B,       "B"},
    {INSTR_BEQ,     "BEQ"},
    {INSTR_BNE,     "BNE"},
    {INSTR_BGT,     "BGT"},
    {INSTR_BLT,     "BLT"},
    {INSTR_AND,     "AND"},
    {INSTR_ORR,     "ORR"},
    {INSTR_EOR,     "EOR"},
    {INSTR_MVN,     "MVN"},
    {INSTR_LSL,     "LSL"},
    {INSTR_LSR,     "LSR"},
    {INSTR_HALT,    "HALT"},
    {INSTR_OUT,     "OUT"},
    {INSTR_DUMP,    "DUMP"}
};