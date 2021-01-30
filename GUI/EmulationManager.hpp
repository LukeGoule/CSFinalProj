#pragma once

#ifndef EMULATION_MANAGER_HPP
#define EMUALTION_MANAGER_HPP

#include "Emulation.hpp"

#include <string>
#include <thread>

#define EMULATOR_DAEMON_EXIT_TIMER 250 // ms

/*
EmulationManager namespace handles all of the threading control and storage for the Emulation class instance the window uses.
Not a class since it needs to be fully static to conform with Windows API.
*/
namespace EmulationManager
{
	extern bool g_bEmulatorRunning;
	extern bool g_bEmulatorPaused;
	extern bool g_bProgramFullExit;

	extern Emulation* g_pEmulator;

	extern std::string g_Code;
	extern std::string g_LastError;

	extern int g_iDelayMS;

	extern std::thread Daemon;

	void NewState();

	void EmuThread();

	void Launch();
}

// Definitions to allow the code to look much cleaner

#define EMULATOR					EmulationManager::g_pEmulator
#define REGISTERS					EMULATOR->m_pRegisters
#define EMULATOR_DAEMON				EmulationManager::Daemon

#define SET_EMULATOR_RUNNING(b)		EmulationManager::g_bEmulatorRunning = b
#define SET_EMULATOR_PAUSED(b)		EmulationManager::g_bEmulatorPaused = b
#define SET_EMULATOR_FULLEXIT(b)	EmulationManager::g_bProgramFullExit = b
#define SET_EMULATOR_ERROR(e)		EmulationManager::g_LastError = e
#define SET_EMULATOR_DELAY(d)		EmulationManager::g_iDelayMS = d
#define SET_EMULATOR_CODE(c)		EmulationManager::g_Code = c

#define GET_EMULATOR_RUNNING		EmulationManager::g_bEmulatorRunning
#define GET_EMULATOR_PAUSED			EmulationManager::g_bEmulatorPaused
#define GET_EMULATOR_FULLEXIT		EmulationManager::g_bProgramFullExit
#define GET_EMULATOR_ERROR			EmulationManager::g_LastError
#define GET_EMULATOR_DELAY			EmulationManager::g_iDelayMS
#define GET_EMULATOR_CODE			EmulationManager::g_Code

#endif