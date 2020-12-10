#include <iostream>

#include "Emulation.hpp"
#include "Utils.hpp"

int main(int argc, char** argv)
{

    Emulation* pEmulationInstance = new Emulation();

    pEmulationInstance->ExecuteSingleInstruction("MOV R0, 100");

    std::string OneInstruction;

    while (true)
    {
        std::getline(std::cin, OneInstruction);
        
        for (auto x : Utils::TokeniseInstruction(OneInstruction))
        {
            std::cout << x << std::endl;
        }
    }

    return 0x0;
}