#include "MenuTab.hpp"

MenuTab::MenuTab(std::string Name, std::size_t id)
    :   MenuComponent(id)/*fix march 30: component ID was not passed!*/,
        m_Name(Name), 
        m_iID(id), 
        m_bActive(false)
{
    if (id == 0) { m_bActive = true; }
}