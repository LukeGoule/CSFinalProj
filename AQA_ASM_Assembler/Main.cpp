#include "../shared/Utils.hpp"
#include "../shared/Output.hpp"
#include "../shared/Instruction.hpp"
#include "../shared/Mappings.hpp"
#include "../shared/RunLengthEncoding.hpp"

int main(int argc, char** argv)
{
	// Output creates the final data, compresses it, an saves it to program.bin.
	/*(new AssembledOutput(5))->AddList(std::vector<std::string> {
		"MOV R0, #0",
		"ADD R0, R0, #1",
		"CMP R0, #200",
		"OUT R0",
		"BNE 1"
	})->Output("program.bin");*/

	std::string FileName = "";

	if (argc == 1)
	{
		std::getline(std::cin, FileName);
	}
	else
	{
		if (strlen(argv[1]) == 0)
		{
			std::getline(std::cin, FileName);
		}
		else
		{
			FileName = argv[1];
		}
	}

	std::cout << "Attempting to assemble " << FileName << std::endl;

	std::ifstream File(FileName);

	if (!File.is_open())
	{
		std::cout << "Failed to find file " << FileName << std::endl;
		return 0;
	}

	std::string line;
	std::vector<std::string> Lines;
	while (std::getline(File, line))
	{
		Lines.push_back(line);
	}

	AssembledOutput* pNewAssembly = new AssembledOutput(Lines.size());
	pNewAssembly->AddList(Lines)->Output(FileName + ".bin");

	std::cout << "Saved to " << FileName << ".bin." << std::endl;

	// Load decompresses the data and parses it into it's instructions, ensuring the program successfully assembled.
	AssembledOutput* pIn = AssembledOutput::Load(FileName + ".bin");
	std::cout << "Reloaded and disassembled to check compilation: " << std::endl;
	
	for (int i = 0; i < pIn->m_vecInstructions.size(); i++)
	{
		std::cout << pIn->m_vecInstructions[i].Disassemble(i) << std::endl;
	}

	std::cin.ignore();

	return 0;
}