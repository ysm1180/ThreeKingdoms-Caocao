#pragma once

#include "LuaLib\LuaTinker.h"

#include <mutex>
#include <memory>
#ifndef _MSVC_LANG
#include "BaseLib\mingw.mutex.h"
#endif

namespace jojogame {
class CGameScriptManager
{
public:
    static void RegisterFunctions(lua_State *L);

    CGameScriptManager();
    virtual ~CGameScriptManager();

    static CGameScriptManager& GetInstance();
protected:
    static std::once_flag s_onceFlag;
    static std::unique_ptr<CGameScriptManager> s_sharedGameScriptManager;
};
}
