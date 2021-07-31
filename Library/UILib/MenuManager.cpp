#include "MenuManager.h"

#include "MenuControl.h"

namespace three_kingdoms
{
std::once_flag CMenuManager::s_onceFlag;
std::unique_ptr<CMenuManager> CMenuManager::s_sharedMenuManager;

void CMenuManager::RegisterFunctions(lua_State *L)
{
}

CMenuManager::CMenuManager()
{
}

CMenuManager::~CMenuManager()
{
}

CMenuManager &CMenuManager::GetInstance()
{
    std::call_once(s_onceFlag,
                   [] {
                       s_sharedMenuManager = std::make_unique<three_kingdoms::CMenuManager>();
                   });

    return *s_sharedMenuManager;
}

int CMenuManager::AddMenuItemByHandle(CMenuItem *item, HMENU handle)
{
    auto index = handle;

    _menuItemStorage.insert(std::map<int, CMenuItem *>::value_type((int)index, item));
    return (int)(index);
}

int CMenuManager::AddMenuItem(CMenuItem *item)
{
    int index = _GetNewIndex();

    if (index == MENU_INDEX::FULL_INDEX)
    {
        return 0;
    }

    _menuItemStorage.insert(std::map<int, CMenuItem *>::value_type(index, item));
    return index;
}

CMenuItem *CMenuManager::GetMenuItem(int id)
{
    auto iter = _menuItemStorage.find(id);
    if (iter != _menuItemStorage.end())
    {
        return iter->second;
    }
    else
    {
        return nullptr;
    }
}

void CMenuManager::DeleteMenuItem(CMenuItem *item)
{
    _reusingIndexStorage.push(item->GetIndex());
    _menuItemStorage.erase(item->GetIndex());
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

    auto result = _reusingIndexStorage.front();
    _reusingIndexStorage.pop();
    return result;
}
} // namespace jojogame
