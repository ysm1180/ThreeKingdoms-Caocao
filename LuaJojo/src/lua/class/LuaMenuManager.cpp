#include "LuaMenuManager.h"
#include <new>

namespace jojogame {

LuaMenuManager* LuaMenuManager::s_sharedMenuManager = nullptr;

LuaMenuManager::LuaMenuManager()
{
    if (s_sharedMenuManager)
    {
        
    }
}

LuaMenuManager::~LuaMenuManager()
{
    s_sharedMenuManager = nullptr;
}

int LuaMenuManager::addMenuItem(LuaMenuItem* item)
{
    const auto index = getNewIndex();

    if (index == MENU_INDEX::FULL_INDEX)
    {
        return 0;
    }

    _menuItemStorage.insert(std::map<int, LuaMenuItem*>::value_type(static_cast<int>(index), item));
    return static_cast<int>(index);
}

void LuaMenuManager::deleteMenuItem(const int index)
{
    _reusingIndexStorage.push_back(index);
    _menuItemStorage.erase(index);
}

LuaMenuManager* LuaMenuManager::getInstance()
{
    if (s_sharedMenuManager == nullptr)
    {
        s_sharedMenuManager = new(std::nothrow) LuaMenuManager();
    }

    return s_sharedMenuManager;
}

int LuaMenuManager::getNewIndex()
{
    if (_reusingIndexStorage.empty())
    {
        const auto index = _menuItemStorage.size() + 1;
        if (index > MENU_INDEX::END_INDEX)
        {
            return MENU_INDEX::FULL_INDEX;
        }
        if (index < MENU_INDEX::START_INDEX)
        {
            return MENU_INDEX::START_INDEX;
        }
        return index;
    }

    return _reusingIndexStorage[0];
}
}
