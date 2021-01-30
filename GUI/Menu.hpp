#pragma once

#ifndef MENU_HPP
#define MENU_HPP

#include "MenuComponent.hpp"

// forward def
class WindowManager;

class Menu 
	: public MenuComponent
{
	std::size_t m_iCurrentTab = 0, m_iEndTabID = 0;

public:

	Menu();
	~Menu();

	void Draw(); // overrides MenuComponent->Draw
	void DrawTabs();
};

namespace MenuSystem
{
	extern WindowManager* g_pInstance;

	void SetInstance(WindowManager* pInstance);
	void LoadComponents();
	void ConstructMenuTree();
	void DrawTree();
}

#define WIN MenuSystem::g_pInstance

#endif