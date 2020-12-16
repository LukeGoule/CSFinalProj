#include "Utils.hpp"
#include "Mappings.hpp"

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

unsigned long Utils::InstructionMnemonicToID(std::string Mnemonic)
{
    auto FoundInstruction = FullInstructionMapping.find(Mnemonic);
    if (FoundInstruction == FullInstructionMapping.end())
    {
        throw std::runtime_error{ "Failed to locate instruction \"" + Mnemonic + "\"" };
    }
    else
    {
        return FoundInstruction->second;
    }
    
}

unsigned long long Utils::String2Int_SafeFail(std::string IntegerString)
{
    unsigned long long uReturnValue = (unsigned long long)(-1L);

    try
    {
        uReturnValue = std::stoull(IntegerString);
    }
    catch (const std::invalid_argument& Error) 
    {
        throw std::runtime_error{ "Failed to convert string \"" + IntegerString + "\" to long int." };
    }
    catch (const std::out_of_range& Error)
    {
        throw std::runtime_error{ "Given integer string, \"" + IntegerString + "\", was outside of the available integer range." };
    }

    return uReturnValue;
}

unsigned long long Utils::ExtractRegisterFromRegisterCode(std::string RegisterCode)
{  
    if (RegisterCode[0] != 'R' && 
        RegisterCode[0] != 'r')
    {
        throw std::runtime_error{ "Invalid register code given." };
    }

    if (RegisterCode.size() > 3)
    {
        throw std::runtime_error{ "Invalid register code given." };
    }

    std::string RegNumString = RegisterCode.substr(1, RegisterCode.size() - 1);

    return Utils::String2Int_SafeFail(RegNumString);
}

unsigned long long Utils::ExtractDecimalFromOperand2(std::string Operand2)
{
    if (Operand2[0] != '#')
    {
        throw std::runtime_error{ "Invalid <operand2> given." };
    }

    std::string DecimalWithoutHash = Operand2.substr(1, Operand2.size() - 1);

    return Utils::String2Int_SafeFail(DecimalWithoutHash);
}