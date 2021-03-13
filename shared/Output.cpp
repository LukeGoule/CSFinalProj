#include "Output.hpp"
#include "Instruction.hpp"
#include "Mappings.hpp"
#include "Utils2.hpp"
#include "RunLengthEncoding.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>

AssembledOutput::AssembledOutput(unsigned long nInstructions)
{
	m_vecInstructions = std::vector<AQAInstruction>(nInstructions);
}

AssembledOutput::~AssembledOutput()
{

}
 

void AssembledOutput::Output(std::string FileName)
{
	auto iBufferSize = sizeof(AQAInstruction) * m_vecInstructions.size();
	char* buf = (char*)malloc(iBufferSize);
	
	if (!buf)
	{
		// error
		return;
	}

	std::size_t i = 0;
	for (auto Instruction : m_vecInstructions) 
	{
		memcpy(buf + (i * sizeof(AQAInstruction)), &Instruction, sizeof(AQAInstruction));
		i++;
	}

	Utils::RLE::PackFile(FileName, buf, iBufferSize);
}

AssembledOutput* AssembledOutput::AddInstruction(AQAInstruction* pInstruction)
{
	if (!pInstruction)
		return this;

	m_vecInstructions[nUtilisedInstructions] = *pInstruction;
	nUtilisedInstructions++;
	return this;
}

AssembledOutput* AssembledOutput::AddInstruction(AQAInstruction Instruction)
{
	m_vecInstructions[nUtilisedInstructions] = Instruction;
	nUtilisedInstructions++;
	return this;
}

AssembledOutput* AssembledOutput::AddList(std::vector<std::string> Instructions)
{
	unsigned long long iIndex = 0;
	for (auto Inst : Instructions)
	{
		AddInstruction(AssembledOutput::AssembleLine(Inst, iIndex));
		iIndex++;
	}

	return this;
}

AssembledOutput* AssembledOutput::AddList(std::vector<AQAInstruction> Instructions)
{
	for (auto Inst : Instructions)
	{
		AddInstruction(Inst);
	}

	return this;
}

AQAInstruction* AssembledOutput::AssembleLine(std::string Line, unsigned long iLineNumber)
{
	auto Tokenised = Utils::Tokenise(Line);

	if (Tokenised.size() == 0)
	{
		return nullptr;
	}

	std::transform(Tokenised[0].begin(), Tokenised[0].end(), Tokenised[0].begin(), std::toupper);

	auto Found = new_FullInstructionMapping.find(Tokenised[0]);

	if (Found == new_FullInstructionMapping.end())
	{

		throw std::runtime_error{ "[Line " + std::to_string(iLineNumber) + "] Instruction " + Tokenised[0] + " not found" };
	}

	unsigned long long uInstruction = Found->second;
	
	std::string ScannableString;
	for (auto Item : Tokenised)
	{
		ScannableString += Item += ".";
	}

	AQAInstruction* pNew = new AQAInstruction(uInstruction);
	
	switch (uInstruction)
	{
	case INSTR_LDR:
		BASIC_REG_SCAN_2("LDR", Rd, MemoryRef);
		break;
	case INSTR_STR:
		BASIC_REG_SCAN_2("STR", Rd, MemoryRef);
		break;
	case INSTR_ADD:
		OPERAND_SCAN_3("ADD", Rd, Rn);
		break;
	case INSTR_SUB:
		OPERAND_SCAN_3("SUB", Rd, Rn);
		break;
	case INSTR_MOV:
		OPERAND_SCAN_2("MOV", Rd);
		break;
	case INSTR_CMP:
		OPERAND_SCAN_2("CMP", Rn);
		break;
	case INSTR_B:
		BASIC_SCAN_1("B", Label);
		break;
	case INSTR_AND:
		OPERAND_SCAN_3("AND", Rn, Rd);
		break;
	case INSTR_ORR:
		OPERAND_SCAN_3("ORR", Rd, Rn);
		break;
	case INSTR_MVN:
		OPERAND_SCAN_2("MVN", Rd);
		break;
	case INSTR_LSL:
		OPERAND_SCAN_3("LSL", Rd, Rn);
		break;
	case INSTR_OUT:
		BASIC_REG_SCAN_1("OUT", Rd);
		break;
	case INSTR_BEQ:
		CONDITION_PTR_SCAN("BEQ");
		break;
	case INSTR_BNE:
		CONDITION_PTR_SCAN("BNE");
		break;
	case INSTR_BGT:
		CONDITION_PTR_SCAN("BGT");
		break;
	case INSTR_BLT:
		CONDITION_PTR_SCAN("BLT");
		break;
	case INSTR_HALT:
	case INSTR_DUMP:
		break;
	default:
		throw std::runtime_error{ "[Line " + std::to_string(iLineNumber) + "] Something has gone wrong." };
		break;
	}
	
	return pNew;
}

AssembledOutput* AssembledOutput::Load(std::string FileName)
{
	std::string BufferData = Utils::RLE::UnpackFile(FileName);

	if (BufferData.size() % sizeof(AQAInstruction) != 0)
	{
		std::cout << "BufferData.size() % sizeof(AQAInstruction) != 0" << std::endl;
		return nullptr;
	}

	AssembledOutput* pNewOutput = new AssembledOutput((unsigned long)BufferData.size() / (unsigned long)sizeof(AQAInstruction));

	std::size_t index = 0;
	for (int i = 0; i < BufferData.size(); i += sizeof(AQAInstruction), index++)
	{
		AQAInstruction New;

		memcpy(&New, BufferData.data() + i, sizeof(AQAInstruction));

		pNewOutput->m_vecInstructions[index] = New;
	}

	return pNewOutput;
}