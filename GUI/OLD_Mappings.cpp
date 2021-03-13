#include "Mappings.hpp"

std::unordered_map<std::string, unsigned long long> FullInstructionMapping = {
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
    CreateMapping(OUT)
    CreateMapping(DUMP)
};