#include "GameScriptManager.h"

namespace jojogame {
std::once_flag CGameScriptManager::s_onceFlag;
std::unique_ptr<CGameScriptManager> CGameScriptManager::s_sharedGameScriptManager;

void CGameScriptManager::RegisterFunctions(lua_State *L)
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
                   []
    {
        s_sharedGameScriptManager = std::make_unique<jojogame::CGameScriptManager>();
    });

    return *s_sharedGameScriptManager;
}
}