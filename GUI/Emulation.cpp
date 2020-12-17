#include "Emulation.hpp"
#include "Utils.hpp"
#include "Mappings.hpp"
#include "Instructions.hpp"

#include <iostream>
#include <string>
#include <algorithm>
#include <thread>

#pragma warning(disable: 26495) // Uninitialised class members.

Emulation::Emulation()
{
    this->m_pMemory = new EmulatedMemory<MEMORY_SIZE>();
    this->m_pLabelManager = new LabelManager<MEMORY_ADDRESS_SIZE>();
}

Emulation::~Emulation()
{

}

EmulatedRegisters::EmulatedRegisters()
{

}

EmulatedRegisters::~EmulatedRegisters()
{

}

REGISTER_VAL_SIZE* EmulatedRegisters::GetRegisterByNumber(unsigned long long n)
{

    std::unordered_map<unsigned long long, REGISTER_VAL_SIZE*> RegisterMapping = {
        {0,     &this->_Reg0},
        {1,     &this->_Reg1},
        {2,     &this->_Reg2},
        {3,     &this->_Reg3},
        {4,     &this->_Reg4},
        {5,     &this->_Reg5},
        {6,     &this->_Reg6},
        {7,     &this->_Reg7},
        {8,     &this->_Reg8},
        {9,     &this->_Reg9},
        {10,    &this->_Reg10},
        {11,    &this->_Reg11},
    };

    // Protects from segfaulting the program.
    if (n > RegisterMapping.size() - 1)
    {
        throw std::runtime_error{ "Attempted to get invalid register!" };
    }

    return RegisterMapping.find(n)->second;

}

void Emulation::Reset()
{
    // Unsafe(??) memory manual memory allocation.
    if (this->m_pRegisters)
    {
        free((void*)m_pRegisters);
        m_pRegisters = nullptr;
    }

    this->m_pRegisters = (EmulatedRegisters*)malloc(sizeof(EmulatedRegisters));
    this->m_pMemory->ResetMemory();
    this->m_pLabelManager->Reset();
}

bool Emulation::ExecuteSingleInstruction(std::string InstructionString)
{
    std::vector<std::string> ParsedInstruction = Utils::StripEmptyFromArray(Utils::TokeniseInstruction(InstructionString));
    
    // Is it a label?
    if (ParsedInstruction[ParsedInstruction.size() - 1] == ":")
    {
        throw std::runtime_error{ "Attempted to define a label within quick execution mode!" };

        return false;
    }

    unsigned long long uMappedInstruction = Utils::InstructionMnemonicToID(ParsedInstruction[0]);

    BoundInstruction_t pCallableBoundFunction = InstructionBindings[uMappedInstruction];

    if (!pCallableBoundFunction)
    {
        throw std::runtime_error{ "Severe error! Instructions internal binding not found!!" };
    }

    pCallableBoundFunction(this, ParsedInstruction);

    return false;
}

void Emulation::Dump()
{
    for (unsigned long long i = 0; i < 12; i++)
    {
        std::cout << "R" << i << " = " << *(this->m_pRegisters->GetRegisterByNumber(i)) << std::endl;
    }

    std::cout << "IP   = " << this->m_pRegisters->_IP << std::endl;
    std::cout << "F_EQ = " << this->m_pRegisters->_F_EQ << std::endl;
    std::cout << "F_NE = " << this->m_pRegisters->_F_NE << std::endl;
    std::cout << "F_GT = " << this->m_pRegisters->_F_GT << std::endl;
    std::cout << "F_LT = " << this->m_pRegisters->_F_LT << std::endl;
}

void Emulation::QuickExecutionMode() 
{
    std::cout << "Quick Execution Mode" << std::endl;
    std::cout << "Everything you type will be interpreted as assembly." << std::endl;

    std::string OneInstruction;

    while (true)
    {
        std::cout << ">> ";
        std::getline(std::cin, OneInstruction);

        try 
        {
            this->ExecuteSingleInstruction(OneInstruction);
        }
        catch (const std::runtime_error& Error)
        {
            std::cout << "[QuickExc] Caught error: " << Error.what() << std::endl;
        }
    }
}

bool Emulation::RunFile(std::string FileData, bool* bContinueCondition, int* piStepDelayMS)
{
    if (FileData.size() == 0)
    {
        throw std::runtime_error{ "Empty file data? -> Was the file name correct?" };
    }

    std::vector<std::string> Lines = Utils::BasicStringExplode(FileData, '\n');

    this->m_pRegisters->_IP = 0;

    while (true)
    {
        if (bContinueCondition && !(*bContinueCondition))
            break;

        if (piStepDelayMS && *piStepDelayMS != 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(*piStepDelayMS));
        }


        if (this->m_pRegisters->_IP == Lines.size())
        {
            break;
        }

        std::string RawLine = Lines[this->m_pRegisters->_IP];

        if (RawLine.size() == 0)
        {
            this->m_pRegisters->_IP++;
            continue;
        }

        std::string FullInstruction = Utils::RemovePrecedingWhitespace(RawLine);
        
        if (FullInstruction.size() == 0)
        {
            this->m_pRegisters->_IP++;
            continue;
        }

        std::vector<std::string> Tokenised = Utils::TokeniseInstruction(FullInstruction);
        std::vector<std::string> ParsedInstruction = Utils::StripEmptyFromArray(Tokenised);

        if (ParsedInstruction.size() == 0)
        {
            this->m_pRegisters->_IP++;
            continue;
        }

        // Is it a label?
        if (ParsedInstruction[ParsedInstruction.size() - 1] == ":")
        {
            std::string LabelIdentifier = ParsedInstruction[0];

            if (LabelIdentifier.size() == 0)
            {
                throw std::runtime_error{ "Label name length must be above zero." };
            }

            //throw std::runtime_error{ "new label lol: " + LabelIdentifier + std::to_string(this->m_pRegisters->_IP) };

            // A new label is the current instruction pointer. When jumping to a label, the instruction pointer is set.
            this->m_pLabelManager->InsertLabel(Utils::StringToUpper(LabelIdentifier), this->m_pRegisters->_IP);

            this->m_pRegisters->_IP++;
            
            continue;
        }

        // Get this current instructions mapped value, if possible.
        unsigned long long uMappedInstruction = Utils::InstructionMnemonicToID(Utils::StringToUpper(ParsedInstruction[0]));

        if (uMappedInstruction < 0 || uMappedInstruction > 20)
        {
            // We're unlikely to actually reach here, but it's safe to check either way.
            throw std::runtime_error{ "Invalid instruction detected!" };

            return false;
        }

        CurrentInstruction.clear();

        for (auto x : ParsedInstruction)
        {
            CurrentInstruction += x + " ";
        }
        CurrentInstruction = CurrentInstruction.substr(0, CurrentInstruction.size() - 1);

        BoundInstruction_t pCallableBoundFunction = InstructionBindings[uMappedInstruction];

        if (!pCallableBoundFunction)
        {
            throw std::runtime_error{ "Severe error! Instructions internal binding not found!!" };
           
            return false;
        }

        for (std::size_t i = 0; i < ParsedInstruction.size(); i++)
        {
            ParsedInstruction[i] = Utils::StringToUpper(ParsedInstruction[i]);
        }

        pCallableBoundFunction(this, ParsedInstruction);
    }

    return true;
}