#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>

namespace Utils
{
    std::vector<std::string> TokeniseInstruction(std::string Instruction);
    std::vector<std::string> StripEmptyFromArray(std::vector<std::string> InArray);
    std::vector<std::string> BasicStringExplode(std::string InputString, char Delimiter);
    std::vector<std::string> ArgArrayFromMainArgs(int argc, char** argv);

    std::string RemovePrecedingWhitespace(std::string InputString);
    std::string StripWhiteSpace(std::string InputString);

    bool IsStringEmpty(std::string InputString);
    
    // Linearly search for an item within a string array.
    bool ExistsWithinArray(std::vector<std::string> InputArray, std::string SearchItem);

    unsigned long InstructionMnemonicToID(std::string Mnemonic);
    unsigned long long String2Int_SafeFail(std::string IntegerString);

    /* Expects input in form R<number> */
    unsigned long long ExtractRegisterFromRegisterCode(std::string RegisterCode);

    unsigned long long ExtractDecimalFromOperand2(std::string Operand2);
}

#endif