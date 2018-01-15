#include "LuaMenu.h"
#include "../LuaJojo.h"
#include "../LuaTinker.h"

namespace jojogame {

LuaMenuItem::LuaMenuItem()
{
    _item = CreateMenu();
}

LuaMenuItem::~LuaMenuItem()
{
    if (_item)
    {
        DestroyMenu(_item);
    }
}

HMENU LuaMenuItem::item()
{
    return _item;
}

std::wstring LuaMenuItem::text()
{
    return _text;
}

std::wstring LuaMenuItem::clickFunction()
{
    return _clickFunction;
}

void LuaMenuItem::setText(std::wstring text)
{
    _text = text;
}

void LuaMenuItem::setClickFunction(std::wstring clickFunction)
{
    _clickFunction = clickFunction;
}

void LuaMenuItem::setChildMenuItem(LuaMenuItem* child)
{

}

void LuaMenuItem::registerToLua()
{
    const auto lua = LuaJojo::getInstance()->getLuaState();

    lua_tinker::class_add<LuaMenuItem>(lua, "MenuItem");

    lua_tinker::class_con<LuaMenuItem>(lua, lua_tinker::constructor<LuaMenuItem>);
    lua_tinker::class_def<LuaMenuItem>(lua, "text", &LuaMenuItem::text);
    lua_tinker::class_def<LuaMenuItem>(lua, "clickFunction", &LuaMenuItem::clickFunction);
    lua_tinker::class_def<LuaMenuItem>(lua, "setText", &LuaMenuItem::setText);
    lua_tinker::class_def<LuaMenuItem>(lua, "setClickFunction", &LuaMenuItem::setClickFunction);
    lua_tinker::class_def<LuaMenuItem>(lua, "setChildMenuItem", &LuaMenuItem::setChildMenuItem);

}

LuaMenubar::LuaMenubar()
{
    _menu = CreateMenu();
}

LuaMenubar::~LuaMenubar()
{
    if (_menu)
    {
        DestroyMenu(_menu);
    }
}

void LuaMenubar::addMenuItem(LuaMenuItem* menuItem)
{
    if (menuItem->item()) {
        AppendMenu(_menu, MF_STRING | MF_POPUP, (UINT)menuItem->item(), menuItem->text().c_str());
    }
    else {
        AppendMenu(_menu, MF_STRING, , menuItem->text().c_str());
    }
}

void LuaMenubar::registerToLua()
{
    const auto lua = LuaJojo::getInstance()->getLuaState();

    lua_tinker::class_add<LuaMenubar>(lua, "Menu");

    lua_tinker::class_con<LuaMenubar>(lua, lua_tinker::constructor<LuaMenubar>);
    lua_tinker::class_def<LuaMenubar>(lua, "addMenuItem", &LuaMenubar::addMenuItem);
}
}
