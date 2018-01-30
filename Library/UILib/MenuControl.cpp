#include "MenuControl.h"



namespace jojogame {
void CMenuItem::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN(CMenuItem, "_MenuItem");
}

CMenuItem::CMenuItem()
{
    _item = CreateMenu();
}

CMenuItem::~CMenuItem()
{
    if (_item)
    {
        DestroyMenu(_item);
    }
}

HMENU CMenuItem::item()
{
    return _item;
}

std::wstring CMenuItem::text()
{
    return _text;
}

std::wstring CMenuItem::clickFunction()
{
    return _clickEvent;
}

void CMenuItem::setText(std::wstring text)
{
    _text = text;
}

void CMenuItem::setClickFunction(std::wstring clickFunction)
{
    _clickEvent = clickFunction;
}

void CMenuItem::setChildMenuItem(CMenuItem* child)
{
}

/****************************************
                CMenubar
*****************************************/

void CMenubar::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN(CMenubar, "_Menu");
}

CMenubar::CMenubar()
{
    _menu = CreateMenu();
}

CMenubar::~CMenubar()
{
    if (_menu)
    {
        DestroyMenu(_menu);
    }
}

void CMenubar::AddMenuItem(CMenuItem* menuItem)
{
    if (menuItem->item())
    {
        AppendMenu(_menu, MF_STRING | MF_POPUP, (UINT)menuItem->item(), menuItem->text().c_str());
    }
    else
    {
        // AppendMenu(_menu, MF_STRING, 10, menuItem->text().c_str());
    }
}
}