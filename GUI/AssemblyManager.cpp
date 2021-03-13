#include "AssemblyManager.hpp"

#include <Windows.h>

std::string AssemblyManager::Code, AssemblyManager::AssembleStatus;
bool AssemblyManager::bExecuting, AssemblyManager::bPaused, AssemblyManager::bAssembling;

void AssemblyManager::AssembleCode()
{
	OutputDebugStringA(__FUNCDNAME__);
}

void AssemblyManager::ExecThread()
{
	OutputDebugStringA(__FUNCDNAME__);
}

void AssemblyManager::Reset()
{
	OutputDebugStringA(__FUNCDNAME__);
}