#include "LuaTinker.h"

namespace jojogame {
std::once_flag CLuaTinker::s_onceFlag;
std::unique_ptr<CLuaTinker> CLuaTinker::s_luaTinker;

int CustomLuaRequire(lua_State *L) {
    std::string fileName(lua_tostring(L, -1));
    fileName = "Script\\" + fileName;
    
    lua_tinker::dofile(L, fileName.c_str());

    lua_pop(L, 1);

    return 0;
}

int CustomLuaMessage(lua_State *L) {
    std::string str(lua_tostring(L, -1));
    MessageBoxA(GetDesktopWindow(), str.c_str(), "Debug", 0);

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
    if (_luaState) {
        lua_close(_luaState);
    }
    _luaState = nullptr;
}

void CLuaTinker::Run(const char *fileName)
{
    lua_tinker::dofile(_luaState, fileName);
}

CLuaTinker& CLuaTinker::GetLuaTinker()
{
    std::call_once(s_onceFlag,
        [] {
        s_luaTinker.reset(new CLuaTinker);
    });

    return *s_luaTinker.get();
}
}