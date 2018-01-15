#pragma once
#include "base/Ref.h"
#include <map>
#include <vector>

namespace jojogame {
enum MENU_INDEX
{
    FULL_INDEX = -1,
    START_INDEX = 1,
    END_INDEX = 255
};

class LuaMenuItem;

class __declspec(dllexport) LuaMenuManager : public Ref
{
public:
    LuaMenuManager();
    ~LuaMenuManager();

    int addMenuItem(LuaMenuItem *item);

    void deleteMenuItem(const int index);

    static LuaMenuManager* getInstance();

private:
    int getNewIndex();

    std::map<int, LuaMenuItem*> _menuItemStorage;
    std::vector<int> _reusingIndexStorage;
    static LuaMenuManager* s_sharedMenuManager;
};
}
