#pragma once

#ifndef TABS_HPP
#define TABS_HPP

#include "MenuTab.hpp"

#define TABID_HOME  0
#define TABID_GUIDE 1
#define TABID_ABOUT 2

class TabHome 
	: public MenuTab
{
public:
	TabHome();
	~TabHome();
	
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