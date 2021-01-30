#pragma once

#ifndef OUTPUT_HPP
#define OUTPUT_HPP

#include <vector>
#include <string>

#define BASIC_SCAN_1(Name, Out) sscanf_s(ScannableString.c_str(), Name ".%llu.", &pNew->m_u##Out)
#define BASIC_REG_SCAN_1(Name, Out) sscanf_s(ScannableString.c_str(), Name ".R%llu.", &pNew->m_u##Out)
#define BASIC_REG_SCAN_2(Name, Out1, Out2) sscanf_s(ScannableString.c_str(), Name ".R%llu.R%llu.", &pNew->m_u##Out1, &pNew->m_u##Out2)
#define CONDITION_PTR_SCAN(Name) sscanf_s(ScannableString.c_str(), Name ".%llu.", &pNew->m_uLabel)

#define OPERAND_SCAN_2(Name, Out1) if (sscanf_s(ScannableString.c_str(), Name ".R%llu.R%llu.", &pNew->m_u##Out1, &pNew->m_uOperand2) != 2){	\
	sscanf_s(ScannableString.c_str(), Name ".R%llu.#%llu.", &pNew->m_u##Out1, &pNew->m_uOperand2);pNew->m_bOperand2Type = true;\
	}
#define OPERAND_SCAN_3(Name, Out1, Out2) if (sscanf_s(ScannableString.c_str(), Name ".R%llu.R%llu.R%llu", &pNew->m_u##Out1, &pNew->m_u##Out2, &pNew->m_uOperand2) != 3) {	\
	sscanf_s(ScannableString.c_str(), Name ".R%llu.R%llu.#%llu.", &pNew->m_u##Out1, &pNew->m_u##Out2, &pNew->m_uOperand2);pNew->m_bOperand2Type = true;\
	}

class AQAInstruction;

class AssembledOutput
{

public:

	AssembledOutput(unsigned long nInstructions);
	~AssembledOutput();

	void Output(std::string FileName);
	AssembledOutput* AddInstruction(AQAInstruction* pInstruction);
	AssembledOutput* AddInstruction(AQAInstruction Instruction);
	AssembledOutput* AddList(std::vector<std::string> Instructions);
	AssembledOutput* AddList(std::vector<AQAInstruction> Instructions);

	static AQAInstruction* AssembleLine(std::string Line, unsigned long iLineNumber = 0UL);
	static AssembledOutput* Load(std::string FileName);

public:

	std::size_t nUtilisedInstructions;
	std::vector<AQAInstruction> m_vecInstructions;
};

#endif