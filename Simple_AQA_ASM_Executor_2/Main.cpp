#include "../shared/Utils.hpp"
#include "../shared/Output.hpp"
#include "../shared/Instruction.hpp"
#include "../shared/Mappings.hpp"
#include "../shared/RunLengthEncoding.hpp"

#include "../shared/Emulator.hpp"

int main(int argc, char** argv)
{
	std::string ToRun = "";

	if (argc == 1)
	{
		std::getline(std::cin, ToRun);
	}
	else
	{
		if (strlen(argv[1]) == 0)
		{
			std::getline(std::cin, ToRun);
		}
		else
		{
			ToRun = argv[1];
		}
	}

	std::cout << "Attempting to execute " << ToRun << std::endl;

	std::ifstream File(ToRun);

	if (!File.is_open())
	{
		std::cout << "Failed to find file " << ToRun << std::endl;
		return 0;
	}

	AssembledOutput* pIn = AssembledOutput::Load(ToRun);
	
	std::cout << "Disassembly: " << std::endl;

	for (int i = 0; i < pIn->m_vecInstructions.size(); i++)
	{
		std::cout << pIn->m_vecInstructions[i].Disassemble(i) << std::endl;
	}

	std::cout << "Running.." << std::endl;

	AQAEmulator* pEmulator = new AQAEmulator();
	pEmulator->RunProgram(pIn);
}