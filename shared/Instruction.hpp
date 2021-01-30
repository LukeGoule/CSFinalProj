#pragma once

#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <string>

#define INSTRUCTION_DATA_WIDTH unsigned long long
#define IDW INSTRUCTION_DATA_WIDTH

// It may seem highly inefficient since I'm using lots of wasted data, and yea. It is.
// But for this application I don't think it matters all that much.
class AQAInstruction
{
public:

	AQAInstruction(IDW uOpcode = 0ULL, IDW uRd = 0ULL, IDW uRn = 0ULL, IDW uMemoryRef = 0ULL, IDW uOperand2 = 0ULL, IDW uLabel = 0ULL, bool bOperand2Type = false);
	~AQAInstruction();

	void ResetData();
	void Dump();
	std::string Disassemble(unsigned long long iIndex = 0ULL);

public:

	INSTRUCTION_DATA_WIDTH m_uOpcode;
	INSTRUCTION_DATA_WIDTH m_uRd;			// out Rd
	INSTRUCTION_DATA_WIDTH m_uRn;
	INSTRUCTION_DATA_WIDTH m_uMemoryRef;
	INSTRUCTION_DATA_WIDTH m_uOperand2;
	INSTRUCTION_DATA_WIDTH m_uLabel;
	bool m_bOperand2Type = false;
};

#endif