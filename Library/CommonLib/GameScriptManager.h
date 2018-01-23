#pragma once

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
