#pragma once

#ifndef COLOURS_HPP
#define COLOURS_HPP

#include "imgui//imgui.h"

struct Colours_t
{
	ImVec4 FullDark = ImVec4(25.f / 255.f, 25.f / 255.f, 25.f / 255.f, 1.f);
	ImVec4 Dark = ImVec4(30.f / 255.f, 30.f / 255.f, 30.f / 255.f, 1.f);
	ImVec4 Lighter = ImVec4(40.f / 255.f, 40.f / 255.f, 40.f / 255.f, 1.f);

	struct Text_t
	{
		ImVec4 Normal = ImVec4(1.f, 1.f, 1.f, 1.f);
		ImVec4 TableAlt = ImVec4(0.75f, 0.75f, 0.75f, 1);
		ImVec4 VeryLight = ImVec4(0.25f, 0.25f, 0.25f, 1);
	} Text;

	struct Buttons_t
	{
		ImVec4 Default = ImVec4(47.f / 255.f, 84.f / 255.f, 115.f / 255.f, 1.f);
		ImVec4 Hovered = ImVec4((47.f / 255.f) + 0.1f, (84.f / 255.f) + 0.1f, (115.f / 255.f) + 0.1f, 1.f);
		ImVec4 Active = ImVec4((47.f / 255.f) + 0.2f, (84.f / 255.f) + 0.2f, (115.f / 255.f) + 0.2f, 1.f);
	} Buttons;
};

extern Colours_t Colours;

#endif