#include "Menu.hpp"
#include "Tabs.hpp"
#include "WindowManager.hpp"
#include "Colours.hpp"

#include <Windows.h>

Menu::Menu()
	: MenuComponent(0L, nullptr, {
		new TabEmulator(),
		new TabAssembler(),
		new TabGuide(), 
		new TabAbout()
	})
{
	m_iCurrentTab = TABID_EMULATOR;
	m_iEndTabID = TABID_ABOUT;
}

Menu::~Menu()
{

}

void Menu::Draw()
{
	DrawTabs();

	ImGui::Spacing();
}

void Menu::DrawTabs()
{
	ImGui::PushFont(WIN->m_pFontVerdana15px);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 10));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_Button, Colours.FullDark);
	for (auto pChild : m_vecChildren)
	{
		// Cast the item in the vector properly to it's superclass.
		// If an item in this vector is NOT a child of MenuTab then
		// we have a serious OOP issue.
		MenuTab* pTab = reinterpret_cast<MenuTab*>(pChild);

		// Check if it is a null pointer.
		if (!pTab) continue;

		// When it's clicked, set the current tab to it.
		if (ImGui::Button(pTab->m_Name.c_str()))
		{
			this->m_iCurrentTab = pTab->m_iID;
		}

		// Prevent the last item from causing styling issues.
		if (pTab->m_iID != m_iEndTabID)
		{
			ImGui::SameLine();
		}

		if (pTab->m_iID == m_iCurrentTab)
		{
			pTab->m_bActive = true;
		}
		else
		{
			pTab->m_bActive = false;
		}
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopFont();
}

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