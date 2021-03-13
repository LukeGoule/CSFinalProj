#pragma once

#ifndef TABS_HPP
#define TABS_HPP

#include "MenuTab.hpp"

#define TABID_EMULATOR  0
#define TABID_ASSEMBLER 1
#define TABID_GUIDE 2
#define TABID_ABOUT 3

class TabEmulator 
	: public MenuTab
{
public:
	TabEmulator();
	~TabEmulator();
	
	void LoadComponent();
	void Draw();
};

class TabAssembler
	: public MenuTab
{
public:
	TabAssembler();
	~TabAssembler();

	void LoadComponent();
	void Draw();
};

class TabGuide
	: public MenuTab
{
public:
	TabGuide();
	~TabGuide();

	void Draw();
};

class TabAbout
	: public MenuTab
{
public:
	TabAbout();
	~TabAbout();

	void Draw();
};

#endif