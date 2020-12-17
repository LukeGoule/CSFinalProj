
#include "WindowManager.hpp"

int WINAPI WinMain(
	HINSTANCE hInstance,		// Instance of this application in memory
	HINSTANCE hPrevInstance,	// Always 0.
	LPSTR pCmdLine,				// String of command line.
	int nCmdShow				// Minimized, maximized or shown normally.
)
{
	WindowManager::get().Setup(L"AQA Assembly Emulator GUI", 800, 600, hInstance);

	WindowManager::get().SetupImGui();

	WindowManager::get().Main();

	WindowManager::get().Cleanup();

	return FALSE;
}