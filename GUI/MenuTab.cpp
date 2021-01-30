#include "MenuTab.hpp"

MenuTab::MenuTab(std::string Name, std::size_t id)
    :   MenuComponent(),
        m_Name(Name), 
        m_iID(id), 
        m_bActive(false)
{
    if (id == 0) { m_bActive = true; }
}