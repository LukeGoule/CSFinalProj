#include "Menu.hpp"
#include "Tabs.hpp"
#include "WindowManager.hpp"
#include "Colours.hpp"

#include <Windows.h>

#pragma region MAIN MENU CODE

Menu::Menu()
	// Call the superclass, passing the children as an array of newly dynamically allocated items.
	: MenuComponent(0L, nullptr, {
		new TabEmulator(),
		new TabAssembler(),
		new TabGuide(), 
		new TabAbout()
	})
{
	// This is done so we aren't wasting valuable resources constantly calculating values which probably won't change.
	m_iCurrentTab = m_vecChildren[0]->GetID(); // Will be the "Emulator" tab in this implementation.
	m_iEndTabID = m_vecChildren[m_vecChildren.size() - 1]->GetID();
}

Menu::~Menu()
{

}

// The main draw function will call this, then all the children of this menu item.
// This means we don't need to do anything silly like for x in children: x->draw().
void Menu::Draw()
{
	DrawTabs();

	// Add some spacing so the menu buttons don't merge with the content.
	ImGui::Spacing();
}

void Menu::DrawTabs()
{
	ImGui::PushFont(WIN->m_pFontVerdana15px);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,		ImVec2(10, 10));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,		ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,	ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button,				Colours.FullDark); // Makes the button not pop like the rest, instead it shows purely as a text item until it is hovered over.
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, Colours.Lighter);
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, Colours.EvenLighter);
	for (auto pChild : m_vecChildren)
	{
		// Cast the item in the vector properly to it's superclass.
		// If an item in this vector is NOT a child of MenuTab then
		// we have a serious OOP issue.
		MenuTab* pTab = reinterpret_cast<MenuTab*>(pChild);

		constexpr float BUTTON_WIDTH = 100.f; // TODO: Make this configurable.

		// Check if it is a null pointer.
		if (!pTab) continue;

		// This renders the actual button with a width of 100 pixels, the height it set to zero so it defaults to
		// the style previously defined.
		if (ImGui::Button(pTab->m_Name.c_str(), ImVec2(BUTTON_WIDTH, 0.f)))
		{
			this->m_iCurrentTab = pTab->m_iID;
			break;	// Added this to fix an error where multiple tabs were drawing simultaneously.
					// This has unfortunately had the unintended side effect of making the buttons fail to render for one frame.
					// TODO: Fix this.
		}

		// Prevent the last item from causing styling issues.
		if (pTab->m_iID != m_iEndTabID)
		{
			ImGui::SameLine();
		}

		if (pTab->m_iID == m_iCurrentTab)
		{
			// Inefficient solution. This needs to only actually change once.
			pTab->m_bActive = true;

			auto pDrawList = ImGui::GetWindowDrawList(); // Utilising this allows us to directly draw items on screen.
			auto WindowPos = ImGui::GetWindowPos();

			WindowPos = ImVec2(WindowPos.x + 1.f, WindowPos.y + 1.f); // The window position should usually be offset by -1,-1.
			
			// TODO: Get the widths and padding of the buttons dynamically instead of hardcoding the values.
			auto Offsetx = 5.f;  // 5 pixels from the left side of the window. This is only the padding.
			auto Offsety = 37.f; // 37 pixels (including the padding) from the top of the window.

			// ButtonPosA = The left point of the line to be drawn. The position is dependent on the ID of the currently selected tab. (TAB IDS **MUST** BE IN ORDER OF APPEARANCE, STARTING FROM ZERO!!)
			// ButtonPosB = The right point of the line to be draw. Derives its own position from that of ButtonPosA.
			auto ButtonPosA = ImVec2(WindowPos.x + Offsetx + (BUTTON_WIDTH * pTab->m_iID), WindowPos.y + Offsety);
			auto ButtonPosB = ImVec2(ButtonPosA.x + (BUTTON_WIDTH - 1), ButtonPosA.y);

			// NOTE: The highest byte of the `col` argument is the alpha. If it is not given the compiler assumes zero, as expected, and nothing will be drawn.
			// This draws white currently. Another TODO: Make the bottom of the button's colours configurable.
			pDrawList->AddLine(ButtonPosA, ButtonPosB, 0xFFFFFFFF, 1.f);
		}
		else
		{
			pTab->m_bActive = false;
		}
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopFont();
}

#pragma endregion

#pragma region MENU SYSTEM CODE

Menu* g_pMenuTreeRoot = nullptr;
WindowManager* MenuSystem::g_pInstance;

void MenuSystem::SetInstance(WindowManager* pInstance)
{
	MenuSystem::g_pInstance = pInstance;
}

void MenuSystem::LoadComponents()
{
	g_pMenuTreeRoot->LoadComponent();

	for (auto pChild : g_pMenuTreeRoot->m_vecChildren)
	{
		pChild->LoadComponent();
	}
}

void MenuSystem::ConstructMenuTree()
{
	// Calls every constructor within the tree
	g_pMenuTreeRoot = new Menu();
}

void MenuSystem::DrawTree()
{
	g_pMenuTreeRoot->DrawNext();
}

#pragma endregion