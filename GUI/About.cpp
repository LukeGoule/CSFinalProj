#include "Tabs.hpp"

TabAbout::TabAbout()
	: MenuTab("About", TABID_ABOUT)
{}

TabAbout::~TabAbout()
{}

void TabAbout::Draw()
{
	if (!m_bActive) return;
	ImGui::Text("Tab -> About");
}