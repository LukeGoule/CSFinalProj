#include "Instruction.hpp"
#include "Mappings.hpp"
#include <iostream>

AQAInstruction::AQAInstruction(IDW uOpcode, IDW uRd, IDW uRn, IDW uMemoryRef, IDW uOperand2, IDW uLabel, bool bOperand2Type)
	:	m_uOpcode(uOpcode), 
		m_uRd(uRd), 
		m_uRn(uRn), 
		m_uMemoryRef(uMemoryRef), 
		m_uOperand2(uOperand2), 
		m_uLabel(uLabel),
		m_bOperand2Type(bOperand2Type)
{}

AQAInstruction::~AQAInstruction()
{}

void AQAInstruction::ResetData()
{
	m_uOpcode			= 0ULL;
	m_uRd				= 0ULL;
	m_uRn				= 0ULL;
	m_uMemoryRef		= 0ULL;
	m_uOperand2		= 0ULL;
	m_uLabel			= 0ULL;
	m_bOperand2Type	= false;
}

void AQAInstruction::Dump()
{
	std::cout << "\tOpcode:       " << m_uOpcode << std::endl;
	std::cout << "\tRd:           " << m_uRd << std::endl;
	std::cout << "\tRn:           " << m_uRn << std::endl;
	std::cout << "\tMemory Ref:   " << m_uMemoryRef << std::endl;
	std::cout << "\tOperand2:     " << m_uOperand2 << std::endl;
	std::cout << "\tLabel:        " << m_uLabel << std::endl;
	std::cout << "\tOperand2Type: " << m_bOperand2Type << std::endl;
}

//int to string
#define ITS(i) std::to_string(i)
#define operand2transl (m_bOperand2Type ? "#" : "") + ITS(m_uOperand2)

std::string AQAInstruction::Disassemble(unsigned long long iIndex)
{
	auto Find = new_FullInstructionMapping_INVERSE.find(m_uOpcode);
	
	if (Find == new_FullInstructionMapping_INVERSE.end())
	{
		throw std::runtime_error{ "Opcode [" + std::to_string(m_uOpcode) + "] could not be found" };
		return {};
	}

	std::string OutString = "[" + std::to_string(iIndex) + "] " + Find->second + " ";

	switch (m_uOpcode)
	{
	// <instr> Rd, <memory ref>
	case INSTR_LDR:
	case INSTR_STR:
		OutString += "R" + ITS(m_uRd) + ", " + ITS(m_uMemoryRef);
		break;

	// <instr> Rd, Rn, <operand>
	case INSTR_ADD:
	case INSTR_SUB:
	case INSTR_AND:
	case INSTR_ORR:
	case INSTR_EOR:
	case INSTR_LSL:
	case INSTR_LSR:
		OutString += "R" + ITS(m_uRd) + ", R" + ITS(m_uRn) + ", " + operand2transl;
		break;

	// <instr> Rd, <operand2>
	case INSTR_MOV:
	case INSTR_MVN:
		OutString += "R" + ITS(m_uRd) + ", " + operand2transl;
		break;

	// <instr> Rn, <operand2>
	case INSTR_CMP:
		OutString += "R" + ITS(m_uRn) + ", " + operand2transl;
		break;
	
	// <instr> label
	case INSTR_B:
	case INSTR_BEQ:
	case INSTR_BNE:
	case INSTR_BGT:
	case INSTR_BLT:
		OutString += ITS(m_uLabel); // TODO: Translate label positions properly.
		break;
	
	// <instr> Rd
	case INSTR_OUT:
		OutString += "R" + ITS(m_uRd);
		break;

	// <instr>
	case INSTR_HALT:
	case INSTR_DUMP:
		break;
	default:
		throw std::runtime_error{ "Failed to translate instruction??" };
		break;
	}

	return OutString;
}