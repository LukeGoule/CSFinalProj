#include "WindowManager.hpp"

#ifdef _BITS32
#error "This application only supports 64 bit compilation."
#endif

int WINAPI WinMain(
	HINSTANCE hInstance,		// Instance of this application in memory
	HINSTANCE hPrevInstance,	// Always 0.
	LPSTR pCmdLine,				// String of command line.
	int nCmdShow				// Minimized, maximized or shown normally.
)
{
#ifdef _DEBUG
	const wchar_t* wszTitle = L"AQA Assembly Emulator GUI | !DEBUG! | build= " __DATE__ " " __TIME__;
#else
	const wchar_t* wszTitle = L"AQA Assembly Emulator GUI";
#endif

	WindowManager::get().Setup(wszTitle, 800, 700, hInstance);
	WindowManager::get().SetupImGui();
	WindowManager::get().Main();
	WindowManager::get().Cleanup();

	return FALSE;
}