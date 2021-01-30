#include "Tabs.hpp"

TabGuide::TabGuide()
	: MenuTab("Guide", TABID_GUIDE)
{}

TabGuide::~TabGuide()
{}

void TabGuide::Draw()
{
	if (!m_bActive) return;

	ImGui::Text("Tab -> Guide");
}