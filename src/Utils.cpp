#include "Utils.hpp"

#include <stdexcept>

std::vector<std::string> Utils::TokeniseInstruction(std::string Instruction)
{
    std::vector<std::string> Output;

    std::string CurrentToken = "";

    for (auto Character : Instruction)
    {
        switch (Character)
        {
        case ' ':
            Output.push_back(CurrentToken);
            CurrentToken.clear();
            break;
        case ':':   // Indicates the end of a line, meaning we can ignore comments after it.
            Output.push_back(CurrentToken);
            Output.push_back(":"); // Labels will always end in ":". This is easy to check.
            return Output;
        case ',':
            Output.push_back(CurrentToken);
            Output.push_back(",");
            CurrentToken.clear();
            break;
        case ';':   // Comment. We can ignore this and everything else after it.
            return Output;
        default:
            CurrentToken.push_back(Character);
            break;
        }
    }

    Output.push_back(CurrentToken);

    return Utils::StripEmptyFromArray(Output); // finally, strip all the empty ones to return a clean array.
}

std::vector<std::string> Utils::StripEmptyFromArray(std::vector<std::string> InArray)
{
    std::vector<std::string> OutArray;

    for (auto Item : InArray)
    {
        if (Utils::IsStringEmpty(Item)) continue;

        OutArray.push_back(Item);
    }

    return OutArray;
}

bool Utils::IsStringEmpty(std::string InputString)
{
    if (InputString.size() == 0)
    {
        return true;
    }

    for (auto Character : InputString)
    {
        switch (Character)
        {
        case ' ':
        case '\n':
        case '\b':
        case '\r':
        case '\t':
            break;
        default:
            return false;
        }
    }

    return true;
}

unsigned long InstructionMnemonicToID(std::string Mnemonic)
{
    
}