#pragma once

#ifndef MENU_TAB_HPP
#define MENU_TAB_HPP

#include <iostream>

#include "MenuComponent.hpp"

/* programmer must define MenuTab::Draw by deriving from this!! */
class MenuTab 
    : public MenuComponent
{
public:

    std::size_t m_iID = (std::size_t)(-1);
    std::string m_Name;
    bool m_bActive = false;

    MenuTab(std::string Name, std::size_t id);
};

#endif