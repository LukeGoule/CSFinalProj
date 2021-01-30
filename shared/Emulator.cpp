#include "Emulator.hpp"
#include "../shared/Instruction.hpp"
#include "../shared/Output.hpp"
#include "../shared/Mappings.hpp"

#include <Windows.h>

AQAEmulator::AQAEmulator(std::size_t nMemoryBytes)
	: m_nMemoryLimit(nMemoryBytes)
{
	m_pMemory = (unsigned long long*)malloc(nMemoryBytes);

	if (!m_pMemory)
	{
		throw std::runtime_error{ "Failed to allocate memory bytes" };
	}

	m_pRegisters = new EmulatedRegisters();
}

AQAEmulator::~AQAEmulator()
{}

void AQAEmulator::RunProgram(AssembledOutput* pAssembled)
{
	auto nInstructions = pAssembled->m_vecInstructions.size();
	
	m_pRegisters->CompleteReset();

	while (true && 
		!(m_pRegisters->_F_HALT == 1ULL) &&
		(m_pRegisters->_IP != nInstructions)) // fuck this im tired. gn
	{
		AQAInstruction Instruction = pAssembled->m_vecInstructions[m_pRegisters->_IP];

		//std::cout << "Executing [" << Instruction.Disassemble() << "].." << std::endl;

		if (RunSingleInstruction(&Instruction))
		{
			m_pRegisters->_IP++;
		}

		if (m_uDelayMS)
			Sleep(m_uDelayMS);

		m_uCycles++;
	}

	std::cout << "Emulation HALTed" << std::endl;
}

void AQAEmulator::ClearMemory()
{
	for (std::size_t i = 0; i < m_nMemoryLimit; i++)
	{
		m_pMemory[i] = 0ULL;
	}
}

void AQAEmulator::ClearRegisters()
{
	m_pRegisters->CompleteReset();
}

#define QUICK_OPERAND2 uOperand2Value = pInstruction->m_uOperand2; if (!pInstruction->m_bOperand2Type) { uOperand2Value = GetRegister(pInstruction->m_uOperand2); };

#define SET_REG(r, val) SetRegister(pInstruction->m_u##r, val)
#define GET_REG(r) GetRegister(pInstruction->m_u##r)

#define SET_MEM(val) SetMemory(pInstruction->m_uMemoryRef, val);
#define GET_MEM GetMemory(pInstruction->m_uMemoryRef)

bool AQAEmulator::RunSingleInstruction(AQAInstruction* pInstruction)
{
	unsigned long long uOperand2Value, uReg, uVal;
	std::size_t i;

	switch (pInstruction->m_uOpcode)
	{
	// LDR Rd, <memory ref>
	case INSTR_LDR:
		SET_REG(Rd, GET_MEM);
	
		return true;
		break;

	// STR Rd, <memory ref> 
	case INSTR_STR:
		SET_MEM(GET_REG(Rd))

		return true;
		break;

	// ADD Rd, Rn, <operand2>
	case INSTR_ADD:
		QUICK_OPERAND2;
		SET_REG(Rn, GET_REG(Rn) + uOperand2Value);

		return true;
		break;

	// SUB Rd, Rn, <operand2> 
	case INSTR_SUB:
		QUICK_OPERAND2;
		SET_REG(Rd, GET_REG(Rn) - uOperand2Value);

		return true;
		break;

	// MOV Rd, <operand2> 
	case INSTR_MOV:
		QUICK_OPERAND2;
		SET_REG(Rd, uOperand2Value);

		return true;
		break;

	// CMP Rn, <operand2> 
	case INSTR_CMP:
		QUICK_OPERAND2;

		uReg = GET_REG(Rn);
		uVal = uOperand2Value;

		if (uReg == uVal)
		{
			m_pRegisters->_F_EQ = 1ULL;
		}

		if (uReg != uVal)
		{
			m_pRegisters->_F_NE = 1ULL;
		}

		if (uReg >= uVal)
		{
			m_pRegisters->_F_GT = 1ULL;
		}

		if (uReg <= uVal)
		{
			m_pRegisters->_F_LT = 1ULL;
		}

		return true;
		break;

	// B <label> 
	case INSTR_B:
		m_pRegisters->_IP = pInstruction->m_uLabel /* Don't convert to a register! We want the pure data. */;
		return false;
		break;
	
	/*
		B<condition> <label/pos>
	*/
	case INSTR_BEQ:
		if (m_pRegisters->_F_EQ == 1ULL)
		{
			m_pRegisters->_F_EQ = 0ULL;

			m_pRegisters->_IP = pInstruction->m_uLabel;
		}

		return false;
		break;

	case INSTR_BNE:
		if (m_pRegisters->_F_NE == 1ULL)
		{
			m_pRegisters->_F_NE = 0ULL;

			m_pRegisters->_IP = pInstruction->m_uLabel;
		}

		return false;
		break;

	case INSTR_BGT:
		if (m_pRegisters->_F_GT == 1ULL)
		{
			m_pRegisters->_F_GT = 0ULL;

			m_pRegisters->_IP = pInstruction->m_uLabel;
		}

		return false;
		break;

	case INSTR_BLT:
		if (m_pRegisters->_F_LT == 1ULL)
		{
			m_pRegisters->_F_LT = 0ULL;

			m_pRegisters->_IP = pInstruction->m_uLabel;
		}

		return false;
		break;

	// AND Rd, Rn, <operand2> // Rd = Rn & operand2
	case INSTR_AND:
		QUICK_OPERAND2;
		SET_REG(Rd, GET_REG(Rn) & uOperand2Value);

		return true;
		break;

	// ORR Rd, Rn, <operand2> 
	case INSTR_ORR:
		QUICK_OPERAND2;
		SET_REG(Rd, GET_REG(Rn) | uOperand2Value);
		
		return true;
		break;

	// EOR Rd, Rn, <operand2> 
	case INSTR_EOR:
		QUICK_OPERAND2;
		SET_REG(Rd, GET_REG(Rn) ^ uOperand2Value);

		return true;
		break;

	// MVN Rd, <operand2>
	case INSTR_MVN:
		QUICK_OPERAND2;
		SET_REG(Rd, ~uOperand2Value);

		return true;
		break;

	// LSL Rd, Rn, <operand2> 
	case INSTR_LSL:
		QUICK_OPERAND2;
		SET_REG(Rd, GET_REG(Rn) >> uOperand2Value);

		return true;
		break;

	// LSR Rd, Rn, <operand2>
	case INSTR_LSR:
		QUICK_OPERAND2;
		SET_REG(Rd, GET_REG(Rn) << uOperand2Value);

		return true;
		break;

	// Stops the execution of the program.
	case INSTR_HALT:
		m_pRegisters->_F_HALT = 1ULL;

		return true;
		break;
	
	// OUT Rd
	case INSTR_OUT:
		std::cout << "R" << pInstruction->m_uRd << " = " << GET_REG(Rd) << std::endl;

		return true;
		break;

	// Outputs all register's values
	case INSTR_DUMP:
		for (int i = 0; i < 12; i += 1)
		{
			std::cout << i << ": " << GetRegister(i) << std::endl;
		}

		return true;
		break;
	default:
		throw std::runtime_error{ "Unrecognised instruction!" };
		return false;
	}
}

#define REG_GUARD(n) if (n > 11) { throw std::runtime_error{ "Register given is out of range"}; }
#define MEMORY_GUARD(n) if (n > m_nMemoryLimit) { throw std::runtime_error{ "Memory ref given is out of range"}; }

unsigned long long AQAEmulator::GetRegister(std::size_t index)
{
	REG_GUARD(index);

	return *m_pRegisters->GetRegisterByNumber(index);
}

void AQAEmulator::SetRegister(std::size_t index, unsigned long long uValue)
{
	REG_GUARD(index);

	*m_pRegisters->GetRegisterByNumber(index) = uValue;
}

unsigned long long AQAEmulator::GetMemory(std::size_t index)
{
	MEMORY_GUARD(index);

	return m_pMemory[index];
}

void AQAEmulator::SetMemory(std::size_t index, unsigned long long uValue)
{
	MEMORY_GUARD(index);

	m_pMemory[index] = uValue;
}