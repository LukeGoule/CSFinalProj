#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

namespace Utils
{
    std::vector<std::string> TokeniseInstruction(std::string Instruction);
    std::vector<std::string> StripEmptyFromArray(std::vector<std::string> InArray);

    bool IsStringEmpty(std::string InputString);

    unsigned long InstructionMnemonicToID(std::string Mnemonic);
}

#endif