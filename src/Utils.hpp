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
    unsigned long long String2Int_SafeFail(std::string IntegerString);

    /* Expects input in form R<number> */
    unsigned long long ExtractRegisterFromRegisterCode(std::string RegisterCode);

    unsigned long long ExtractDecimalFromOperand2(std::string Operand2);
}

#endif