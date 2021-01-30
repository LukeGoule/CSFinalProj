#pragma once

#ifndef FONTS_MAN_HPP
#define FONTS_MAN_HPP

#include "imgui/imgui.h"

#define FONTID_VERDANA_13 0
#define FONTID_CONSOLAS_13 1
#define FONTID_VERDANA_145 2

#define CONSOLAS_FILE	"C:\\Windows\\Fonts\\consola.ttf"
#define VERDANA_FILE	"C:\\Windows\\Fonts\\verdana.ttf"

namespace Fonts
{
	ImFont* GetFont(long ID);
	
	void Load();
}

#endif