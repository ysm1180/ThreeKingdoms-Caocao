#pragma once

#include "base/Ref.h"
#include "class/LuaMenuManager.h"

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

namespace jojogame {
class LuaWindow;
class LuaGame;
class LuaTime;
class LuaColor;

class __declspec(dllexport) LuaJojo : public Ref
{
public:
    LuaJojo();
    ~LuaJojo();

    void init();

    lua_State* getLuaState();

    void run(const char* fileName);

    void end();

    static LuaJojo* getInstance();

protected:
    lua_State* _lua;
    LuaWindow* _window;
    LuaGame* _game;
    LuaTime* _time;
    LuaColor* _color;
    LuaMenuManager* _menuManager;

    static LuaJojo* s_sharedJojoLua;
};
}
