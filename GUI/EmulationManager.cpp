#include "EmulationManager.hpp"
#include "DefaultProgram.hpp"

#include <Windows.h>
#include <thread>

namespace EmulationManager
{
    Emulation* g_pEmulator;
    
    bool g_bEmulatorRunning = false;
    bool g_bEmulatorPaused = false;
    bool g_bProgramFullExit = false;

    std::string g_Code;
    std::string g_LastError = "";
    
    int g_iDelayMS = 500;

    std::thread Daemon;
}

void EmulationManager::NewState() 
{
    if (EMULATOR)
    {
        delete EMULATOR;
    }

    EMULATOR = new Emulation();

    EMULATOR->Reset();

    EMULATOR->m_pRegisters->CompleteReset();

    g_Code = CodeString;
}

void EmulationManager::EmuThread()
{
    while (!g_bProgramFullExit)
    {
        Sleep(10); // This delay must be added in to allow time for the CPU to rest, otherwise we could be running millions of cycles a second. This locks up the rest of the application.

        if (!GET_EMULATOR_RUNNING) continue;

        try
        {
            EMULATOR->RunFile(g_Code, &GET_EMULATOR_RUNNING, &GET_EMULATOR_DELAY, &GET_EMULATOR_PAUSED);
        }
        catch (const std::runtime_error& Error)
        {
            g_LastError = Error.what();
        }
        catch (const std::out_of_range& Error)
        {
            g_LastError = Error.what();
        }

        SET_EMULATOR_RUNNING(false);
    }
}

void EmulationManager::Launch() 
{
    Daemon = std::thread(EmulationManager::EmuThread);
}