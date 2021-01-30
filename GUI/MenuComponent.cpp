#include "MenuComponent.hpp"

#include <Windows.h>

MenuComponent::MenuComponent(long long ID, MenuComponent* pParent, std::vector<MenuComponent*> vecChildren)
	:	m_iID(ID), 
		m_pParent(pParent), 
		m_vecChildren(vecChildren)
{}

MenuComponent::~MenuComponent()
{}

[[nodiscard]]
MenuComponent* MenuComponent::GetParent()
{
	return m_pParent;
}

[[nodiscard]] 
MenuComponent* MenuComponent::GetChild(std::size_t index)
{
	if (index > m_vecChildren.size()) return nullptr;

	return m_vecChildren[index];
}

[[nodiscard]] 
std::size_t MenuComponent::GetChildrenCount()
{
	return m_vecChildren.size();
}

void MenuComponent::DrawNext()
{
	this->Draw();

	for (auto pChild : this->m_vecChildren)
	{
		/* 
		checked on tree construction, not neede here.
	
		if (pChild == this->m_pParent)
		{
			//err
		}
		*/

		if (!pChild)
		{
			MessageBoxA(0, "pChild was NULL!", "FATAL ERROR", MB_ICONERROR);
			abort();
			return;
		}

		pChild->DrawNext();
	}
}

void MenuComponent::LoadComponent()
{
	// Nothing to be done.
}