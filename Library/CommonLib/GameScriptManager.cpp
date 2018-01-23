#include "stdafx.h"
#include "GameScriptManager.h"

namespace jojogame {
std::once_flag s_onceFlag;
std::unique_ptr<CGameScriptManager> s_sharedGameScriptManager;

void CGameScriptManager::RegisterFunctions(lua_State * L)
{
}

CGameScriptManager::CGameScriptManager()
{
}


CGameScriptManager::~CGameScriptManager()
{
}
CGameScriptManager& CGameScriptManager::GetInstance()
{
    std::call_once(s_onceFlag,
        [] {
        s_sharedGameScriptManager.reset(new CGameScriptManager);
    });

    return *s_sharedGameScriptManager.get();
}
}
