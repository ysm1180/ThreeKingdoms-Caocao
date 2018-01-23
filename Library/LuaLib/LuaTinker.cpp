#include "LuaTinker.h"

namespace jojogame {
std::once_flag CLuaTinker::s_onceFlag;
std::unique_ptr<CLuaTinker> CLuaTinker::s_luaTinker;

CLuaTinker::CLuaTinker()
{
    _luaState = luaL_newstate();
    luaL_openlibs(_luaState);
    lua_settop(_luaState, 0);
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