#pragma once

#ifndef MENU_COMPONENT_HPP
#define MENU_COMPONENT_HPP

#include <vector>

#include "imgui/imgui.h" // Include here so it's included far down the line/tree

class MenuComponent
{
protected:

	long long m_iID = -(1L);

public:

	MenuComponent* m_pParent;
	std::vector<MenuComponent*> m_vecChildren;

	MenuComponent(long long ID = 0, MenuComponent* pParent = nullptr, std::vector<MenuComponent*> vecChildren = {});
	~MenuComponent();

	MenuComponent* GetParent();
	MenuComponent* GetChild(std::size_t index);
	std::size_t GetChildrenCount();

	void DrawNext();

	virtual void Draw() = 0;

	virtual void LoadComponent();
};

#endif