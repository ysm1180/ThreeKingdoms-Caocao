#pragma once

#include <map>
#include <vector>

namespace jojogame {
enum MENU_INDEX
{
    FULL_INDEX = -1,
    START_INDEX = 1,
    END_INDEX = 255
};

class CMenuItem;

class CMenuManager
{
public:
    static void RegisterFunctions(lua_State* L);

    CMenuManager();
    ~CMenuManager();

    int AddMenuItem(CMenuItem *item);

    void DeleteMenuItem(const int index);

    static CMenuManager& GetInstance();
private:
    int _GetNewIndex();

    std::map<int, CMenuItem*> _menuItemStorage;
    std::vector<int> _reusingIndexStorage;

    static std::once_flag s_onceFlag;
    static std::unique_ptr<CMenuManager> s_sharedMenuManager;
};
}
