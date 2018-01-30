#include "MenuManager.h"

namespace jojogame {
std::once_flag CMenuManager::s_onceFlag;
std::unique_ptr<CMenuManager> CMenuManager::s_sharedMenuManager;

void CMenuManager::RegisterFunctions(lua_State * L)
{
}

CMenuManager::CMenuManager()
{

}

CMenuManager::~CMenuManager()
{
}

CMenuManager& CMenuManager::GetInstance()
{
    std::call_once(s_onceFlag,
        [] {
        s_sharedMenuManager.reset(new CMenuManager);
    });

    return *s_sharedMenuManager.get();
}

int CMenuManager::AddMenuItem(CMenuItem* item)
{
    const auto index = _GetNewIndex();

    if (index == MENU_INDEX::FULL_INDEX)
    {
        return 0;
    }

    _menuItemStorage.insert(std::map<int, CMenuItem*>::value_type(static_cast<int>(index), item));
    return static_cast<int>(index);
}

void CMenuManager::DeleteMenuItem(const int index)
{
    _reusingIndexStorage.push_back(index);
    _menuItemStorage.erase(index);
}

int CMenuManager::_GetNewIndex()
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