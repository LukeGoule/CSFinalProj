#include <iostream>

#include "Emulation.hpp"
#include "Utils.hpp"

int main(int argc, char** argv)
{

    Emulation* pEmulationInstance = new Emulation();
    pEmulationInstance->Reset();
    
    std::string OneInstruction;

    while (true)
    {
        std::getline(std::cin, OneInstruction);
        
        try {
            pEmulationInstance->ExecuteSingleInstruction(OneInstruction);
        }
        catch (const std::runtime_error& Error)
        {
            std::cout << "Caught error: " << Error.what() << std::endl;
        }
    }

    return 0x0;
}