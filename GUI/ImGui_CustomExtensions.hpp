#pragma once

#include "imgui/imgui.h"

namespace ImGui
{
    namespace Custom {
        bool ToggleButton(bool* bChange, const char* szTrueConditionString, const char* szFalseConditionString, ImVec2 Size = ImVec2(0.f,0.f));
    }
}