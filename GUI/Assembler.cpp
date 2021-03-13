#include "Tabs.hpp"
#include "Menu.hpp"
#include "WindowManager.hpp"
#include "ImGui_CustomExtensions.hpp"
#include "EmulationManager.hpp"
#include "Localisation.hpp"
#include "Colours.hpp"

#pragma warning(disable: 4244)
#pragma warning(disable: 4309)
#pragma warning(disable: 4305)


TabAssembler::TabAssembler()
    : MenuTab("Assembler", TABID_ASSEMBLER)
{}

TabAssembler::~TabAssembler()
{}

void TabAssembler::Draw()
{
    if (!m_bActive) return;
    if (!WIN) return;

    ImGui::PushFont(WIN->m_pFontConsolas);
    {
        WIN->m_Editor.Render(Localisation.CodeEntry, ImVec2(WIN->m_iWindowSizeX - ImGui::GetStyle().FramePadding.x * 5, (WIN->m_iWindowSizeY - (273 + 35))));
    }
    ImGui::PopFont();

    ImGui::Spacing();

    constexpr float fButtonWidth = 50.f;

    ImGui::PushFont(WIN->m_pFontVerdana);
    {
        //ImGui::Custom::ToggleButton(&GET_EMULATOR_RUNNING, Localisation.Stop, Localisation.Run, ImVec2(fButtonWidth, 0.f));

        ImGui::SameLine();
        //ImGui::Custom::ToggleButton(&GET_EMULATOR_PAUSED, Localisation.Play, Localisation.Pause, ImVec2(fButtonWidth, 0.f));

        ImGui::SameLine();

        /*if (ImGui::Button(Localisation.Reset, ImVec2(fButtonWidth, 0.f)))
        {
            EmulationManager::NewState();
        }*/
    }
    ImGui::PopFont();

    ImGui::SameLine();
    ImGui::PushItemWidth(380);
    ImGui::LabelText("", Localisation.LastError, GET_EMULATOR_ERROR.c_str());
    ImGui::SameLine();
    ImGui::PushItemWidth(225);
    ImGui::SliderInt("", &GET_EMULATOR_DELAY, 5, 1000, "Delay - %dms");
    ImGui::PopItemWidth();

    constexpr float fBottomFrameHeight = 177.5f;

    // Left frame
    ImGui::BeginChildFrame(82394809234, ImVec2((WIN->m_iWindowSizeX - 30) / 2, fBottomFrameHeight));
    {
        ImGui::TextColored(Colours.Text.Normal, "Reg0                            %020llu", *REGISTERS->R0());
        ImGui::TextColored(Colours.Text.TableAlt, "Reg1                            %020llu", *REGISTERS->R1());
        ImGui::TextColored(Colours.Text.Normal, "Reg2                            %020llu", *REGISTERS->R2());
        ImGui::TextColored(Colours.Text.TableAlt, "Reg3                            %020llu", *REGISTERS->R3());
        ImGui::TextColored(Colours.Text.Normal, "Reg4                            %020llu", *REGISTERS->R4());
        ImGui::TextColored(Colours.Text.TableAlt, "Reg5                            %020llu", *REGISTERS->R5());

        ImGui::Spacing();

        ImGui::TextColored(Colours.Text.Normal, "Equals Flag                     %020llu", REGISTERS->_F_EQ);
        ImGui::TextColored(Colours.Text.TableAlt, "Not Equals Flag                 %020llu", REGISTERS->_F_NE);

        ImGui::Spacing();

        ImGui::TextColored(Colours.Text.Normal, "Instruction Pointer             %020llu", REGISTERS->_IP);
    }
    ImGui::EndChildFrame();

    ImGui::SameLine();

    // Right frame
    ImGui::BeginChildFrame(82394809235, ImVec2((WIN->m_iWindowSizeX - 30) / 2, fBottomFrameHeight));
    {
        ImGui::TextColored(Colours.Text.Normal, "Reg6                            %020llu", *REGISTERS->R6());
        ImGui::TextColored(Colours.Text.TableAlt, "Reg7                            %020llu", *REGISTERS->R7());
        ImGui::TextColored(Colours.Text.Normal, "Reg8                            %020llu", *REGISTERS->R8());
        ImGui::TextColored(Colours.Text.TableAlt, "Reg9                            %020llu", *REGISTERS->R9());
        ImGui::TextColored(Colours.Text.Normal, "Reg10                           %020llu", *REGISTERS->R10());
        ImGui::TextColored(Colours.Text.TableAlt, "Reg11                           %020llu", *REGISTERS->R11());

        ImGui::Spacing();

        ImGui::TextColored(Colours.Text.Normal, "Greater Than Flag               %020llu", REGISTERS->_F_GT);
        ImGui::TextColored(Colours.Text.TableAlt, "Less Than Flag                  %020llu", REGISTERS->_F_LT);

        ImGui::Spacing();

        ImGui::TextColored(Colours.Text.Normal, Localisation.Eval, EMULATOR->CurrentInstruction.c_str());
    }
    ImGui::EndChildFrame();

    ImGui::TextColored(Colours.Text.VeryLight, Localisation.Copyright);
}

void TabAssembler::LoadComponent()
{
    OutputDebugStringA("Loaded assembler component");
}