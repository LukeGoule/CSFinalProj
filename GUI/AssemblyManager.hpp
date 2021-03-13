#pragma once

#ifndef ASM_MNGR_HPP
#define ASM_MNGR_HPP

#include "../shared/Output.hpp"

namespace AssemblyManager
{
	extern std::string Code, AssembleStatus;
	extern bool bExecuting, bPaused, bAssembling;

	void AssembleCode();

	void ExecThread();

	void Reset();
}

#endif