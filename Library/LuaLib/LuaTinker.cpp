#include "LuaTinker.h"

#include "UILib\WindowControl.h"

namespace jojogame {
std::once_flag CLuaTinker::s_onceFlag;
std::unique_ptr<CLuaTinker> CLuaTinker::s_luaTinker;

int CustomLuaRequire(lua_State *L)
{
    const char *filePath = lua_tostring(L, -1);

    if (!filePath)
    {
    }

    lua_tinker::dofile(L, filePath);

    lua_pop(L, 1);

    return 0;
}

int CustomLuaMessage(lua_State *L)
{
    char c[128] = {0,};

    if (lua_isstring(L, -1))
    {
        std::string str(lua_tostring(L, -1));
        MessageBoxA(GetDesktopWindow(), str.c_str(), "Lua Print", 0);
    } else if (lua_isnumber(L, -1))
    {
        sprintf_s(c, sizeof(c), "%lf", lua_tonumber(L, -1));
        MessageBoxA(GetDesktopWindow(), c, "Lua Print", 0);
    } else if (lua_isnil(L, -1))
    {
        MessageBoxA(GetDesktopWindow(), "nil", "Lua Print", 0);
    }

    lua_pop(L, 1);

    return 0;
}

CLuaTinker::CLuaTinker()
{
    _luaState = luaL_newstate();
    luaL_openlibs(_luaState);
    lua_settop(_luaState, 0);

    RegisterFunction("require", CustomLuaRequire);
    RegisterFunction("print", CustomLuaMessage);
}

CLuaTinker::~CLuaTinker()
{
    if (_luaState)
    {
        lua_close(_luaState);
    }
    _luaState = nullptr;
}

lua_State *CLuaTinker::GetLuaState()
{
    return _luaState;
}

void CLuaTinker::Run(const char *fileName)
{
    lua_tinker::dofile(_luaState, fileName);
}

CLuaTinker& CLuaTinker::GetLuaTinker()
{
    std::call_once(s_onceFlag,
                   []
                   {
                       s_luaTinker.reset(new CLuaTinker);
                   });

    return *s_luaTinker.get();
}
}