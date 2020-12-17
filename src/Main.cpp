#include <iostream>
#include <fstream>
#include <streambuf>

#include "Emulation.hpp"
#include "Utils.hpp"

std::string PromptOption(std::string PromptString = "")
{
    if (PromptString.size() != 0)
        std::cout << PromptString << std::endl;

    std::string Option;

    while (true) {
        std::cout << ">> ";
        std::string InputString;
        std::getline(std::cin, InputString);

        if (InputString.size() == 0)
        {
            std::cout << "Invalid option";
            continue;
        }

        Option = Utils::RemovePrecedingWhitespace(InputString)[0];

        if (Option != "1" && Option != "2" && Option != "3")
        {
            std::cout << "Invalid option";
            continue;
        }

        break;
    }

    return Option;
}

std::string PromptFile()
{
    std::cout << "Please enter a valid file name." << std::endl;

    while (true)
    {
        std::string EnteredName;

        while (true) {
            std::cout << ">> ";
            std::string InputString;
            std::getline(std::cin, InputString);

            if (InputString.size() == 0)
            {
                std::cout << "Invalid file! [bad input]" << std::endl;
                continue;
            }

            EnteredName = Utils::RemovePrecedingWhitespace(InputString)[0];

            break;
        }

        return EnteredName;
    }
}

int main(int argc, char** argv)
{
    std::vector<std::string> ProgramArgs = Utils::StripEmptyFromArray(Utils::ArgArrayFromMainArgs(argc, argv));
    
    std::cout << "Copyright © 2020 Luke Goule <luke.goule@gmail.com>" << std::endl;
    std::cout << "Full license available at https://luke-goule.mit-license.org/" << std::endl;

    if (ProgramArgs.size() > 1) 
    {
        bool bQuickMode = Utils::ExistsWithinArray(ProgramArgs, "-q") || Utils::ExistsWithinArray(ProgramArgs, "--quick");

        Emulation* pEmulationInstance = new Emulation();
        pEmulationInstance->Reset();

        if (bQuickMode) // Opens the "quick execution mode"
        {
            pEmulationInstance->QuickExecutionMode();
        }
        else
        {
            std::cout << "Executing " << ProgramArgs[1] << std::endl;

            // https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
            std::ifstream FileStream(ProgramArgs[1]);
            std::string ProgramFile((std::istreambuf_iterator<char>(FileStream)), std::istreambuf_iterator<char>());
            FileStream.close();

            pEmulationInstance->RunFile(ProgramFile);
        }
    }
    else
    {
        std::string Option = PromptOption("1) Quick execution mode\n2) Open a file by name\n3) Exit the program");

        Emulation* pEmulationInstance = new Emulation();
        pEmulationInstance->Reset();

        if (Option == "1") // "Quick execution mode"
        {
            pEmulationInstance->QuickExecutionMode();
        }
        else if (Option == "2") // Open a file by name, then start quick execution
        {
            std::string FileName = PromptFile();

            // https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
            std::ifstream FileStream(FileName);
            std::string ProgramFile((std::istreambuf_iterator<char>(FileStream)), std::istreambuf_iterator<char>());
            FileStream.close();

            pEmulationInstance->RunFile(ProgramFile);

            pEmulationInstance->QuickExecutionMode();
        }
        else
        {
            return 0;
        }
    }

    return 0;
}