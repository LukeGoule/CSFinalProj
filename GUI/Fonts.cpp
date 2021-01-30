#include "Fonts.hpp"
#include "Localisation.hpp"
#include "Menu.hpp"
#include "WindowManager.hpp"

#include <unordered_map>
#include <Windows.h>

std::unordered_map<long, ImFont*> g_FontMap = {{FONTID_CONSOLAS_13, NULL}, {FONTID_VERDANA_13, NULL}, {FONTID_VERDANA_145, NULL}};

ImFont* Fonts::GetFont(long ID)
{
	return g_FontMap[ID];
}

void Fonts::Load()
{
	ImGuiIO& io = ImGui::GetIO();

    // Load some the default windows fonts this program uses.
    g_FontMap[FONTID_CONSOLAS_13] = io.Fonts->AddFontFromFileTTF(CONSOLAS_FILE, 13.f);
    g_FontMap[FONTID_VERDANA_13] = io.Fonts->AddFontFromFileTTF(VERDANA_FILE, 13.f);
    g_FontMap[FONTID_VERDANA_145] = io.Fonts->AddFontFromFileTTF(VERDANA_FILE, 14.5f);

    if (!g_FontMap[FONTID_CONSOLAS_13] || !g_FontMap[FONTID_VERDANA_13] || !g_FontMap[FONTID_VERDANA_145])
    {
        MessageBoxA(NULL, Localisation.Error.Fonts, Localisation.Error.Fatal, MB_ICONERROR);
        WIN->m_bForceQuit = true; // Safely tell the window to exit.
    }
}