#pragma once

#ifndef EMULATOR_HPP
#define EMULATOR_HPP

#include <iostream>
#include <string>
#include <vector>

#include "EmulatedRegisters.hpp"

class AssembledOutput;
class AQAInstruction;

class AQAEmulator
{

	// Memory, allocated on constructor call
	unsigned long long* m_pMemory;
	std::size_t m_nMemoryLimit;

	unsigned long long m_uCycles;
	unsigned long long m_uDelayMS = 10ULL;

	EmulatedRegisters* m_pRegisters;

public:

	AQAEmulator(std::size_t nMemoryBytes = 1024ULL);
	~AQAEmulator();

	void RunProgram(AssembledOutput* pAssembled);
	void ClearMemory();
	void ClearRegisters();
	bool RunSingleInstruction(AQAInstruction* pInstruction);

	unsigned long long GetRegister(std::size_t index);
	void SetRegister(std::size_t index, unsigned long long uValue);

	unsigned long long GetMemory(std::size_t index);
	void SetMemory(std::size_t index, unsigned long long uValue);
};

#endif