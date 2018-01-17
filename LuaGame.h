#pragma once

#include "base/Ref.h"

namespace jojogame {
class __declspec(dllexport) LuaGame : public Ref
{
public:
    LuaGame();
    ~LuaGame();

    void quit();

    static LuaGame* getInstance();
private:
    static LuaGame* s_sharedGame;
};
}
