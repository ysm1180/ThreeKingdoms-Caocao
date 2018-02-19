#include "ControlManager.h"

#include "BaseLib\MemoryPool.h"

#include "BaseControl.h"
#include "WindowControl.h"
#include "MoviePlayerControl.h"
#include "ButtonControl.h"

namespace jojogame {
std::once_flag CControlManager::s_onceFlag;
std::unique_ptr<CControlManager> CControlManager::s_controlManager;

void CControlManager::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN(CControlManager, "_ControlManager");

    LUA_METHOD(CreateWindowForm);
    LUA_METHOD(CreateMoviePlayer);
    LUA_METHOD(CreateButton);
}

CControlManager::CControlManager()
{
}

CControlManager::~CControlManager()
{
}

void CControlManager::Init(HINSTANCE hInstance)
{
    _hInstance = hInstance;

    CLuaTinker::GetLuaTinker().RegisterClassToLua<CControlManager>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CBaseControl>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CWindowControl>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CMoviePlayerControl>();
    CLuaTinker::GetLuaTinker().RegisterClassToLua<CButtonControl>();
}

CWindowControl *CControlManager::CreateWindowForm(CWindowControl *parent)
{
    auto newWindow = CMemoryPool<CWindowControl>::GetInstance().New();
    newWindow->SetParentWindow(parent);
    return newWindow;
}

CMoviePlayerControl *CControlManager::CreateMoviePlayer(CWindowControl *parent, std::string fileName)
{
    auto newMoviePlayer = CMemoryPool<CMoviePlayerControl>::GetInstance().New(parent, fileName);
    return newMoviePlayer;
}

CButtonControl * CControlManager::CreateButton(CWindowControl *parent)
{
    auto newButton = CMemoryPool<CButtonControl>::GetInstance().New();
    newButton->SetParentWindow(parent);
    return newButton;
}

HINSTANCE CControlManager::GetHInstance()
{
    return _hInstance;
}

void CControlManager::SetHInstance(HINSTANCE hInstance)
{
    _hInstance = hInstance;
}

CControlManager& CControlManager::GetInstance()
{
    std::call_once(s_onceFlag, []
    {
        s_controlManager.reset(new CControlManager);
    });

    return *s_controlManager.get();
}
}