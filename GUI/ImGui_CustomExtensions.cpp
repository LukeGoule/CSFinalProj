#include "ImGui_CustomExtensions.hpp"

namespace ImGui
{
    namespace Custom {
        bool ToggleButton(bool* bChange, const char* szTrueConditionString, const char* szFalseConditionString, ImVec2 Size) {
            if (ImGui::Button(*bChange ? szTrueConditionString : szFalseConditionString, Size))
            {
                *bChange = !*bChange;
            }

            return *bChange;
        }
    }
}