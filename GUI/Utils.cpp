#include "Utils.hpp"
#include "Mappings.hpp"

#include <stdexcept>
#include <algorithm>

#ifdef _WIN32
#pragma warning(disable: 4101 4244)
#endif

std::vector<std::string> Utils::TokeniseInstruction(std::string Instruction)
{
    std::vector<std::string> Output;

    std::string CurrentToken = "";

    for (auto Character : Instruction)
    {
        switch (Character)
        {
        case ' ':
            Output.push_back(Utils::StripWhiteSpace(CurrentToken));
            CurrentToken.clear();
            break;
        case ':':   // Indicates the end of a line, meaning we can ignore comments after it.
            Output.push_back(Utils::StripWhiteSpace(CurrentToken));
            Output.push_back(":"); // Labels will always end in ":". This is easy to check.
            return Output;
        case ',':
            Output.push_back(Utils::StripWhiteSpace(CurrentToken));
            Output.push_back(",");
            CurrentToken.clear();
            break;
        case ';':   // Comment. We can ignore this and everything else after it.
            Output.push_back(Utils::StripWhiteSpace(CurrentToken));
            return Utils::StripEmptyFromArray(Output);
        default:
            CurrentToken.push_back(Character);
            break;
        }
    }

    Output.push_back(Utils::StripWhiteSpace(CurrentToken));

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

std::vector<std::string> Utils::BasicStringExplode(std::string InputString, char Delimiter)
{
    if (InputString.size() == 0)
        return {};

    std::vector<std::string> Output;
    std::string CurrentString;

    for (auto Character : InputString)
    {
        if (Character == Delimiter)
        {
            Output.push_back(CurrentString);
            CurrentString.clear();
        }
        else 
        {
            CurrentString.push_back(Character);
        }
    }

    if (CurrentString.size() != 0)
        Output.push_back(CurrentString);

    return Output;
}

std::vector<std::string> Utils::ArgArrayFromMainArgs(int argc, char** argv)
{
    std::vector<std::string> Output;

    for (int i = 0; i < argc; i++)
    {
        std::string CurrentArg = std::string(argv[i]);

        if (Utils::IsStringEmpty(CurrentArg)) continue;

        Output.push_back(CurrentArg);
    }

    return Output;
}

std::string Utils::RemovePrecedingWhitespace(std::string InputString)
{
    std::string Output;
    std::size_t i = 0;

    for (; i < InputString.size(); i++)
    {
        bool bExit = false;
        switch (InputString[i])
        {
        case '\n':
        case '\t':
        case '\b':
        case '\r':
            break;
        default:
            bExit = true;
        }

        if (bExit) break;
    }

    return InputString.substr(i, InputString.size());
}

std::string Utils::StripWhiteSpace(std::string InputString)
{
    std::string Output;

    for (auto Character : InputString)
    {
        switch (Character)
        {
        case '\n':
        case '\r':
        case '\b':
        case '\t':
            break;
        default:
            Output.push_back(Character);
            break;
        }
    }

    return Output;
}

std::string Utils::StringToUpper(std::string InputString)
{
    auto UpperString = std::string(InputString);
    std::transform(UpperString.begin(), UpperString.end(), UpperString.begin(),
        [](unsigned char c) { return std::toupper(c); });

    return UpperString;
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

bool Utils::ExistsWithinArray(std::vector<std::string> InputArray, std::string SearchItem)
{
    for (auto Item : InputArray)
    {
        if (Item == SearchItem) return true;
    }

    return false;
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