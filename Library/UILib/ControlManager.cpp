#include "stdafx.h"
#include "ControlManager.h"

#include "BaseControl.h"
#include "WindowControl.h"
#include "MoviePlayerControl.h"

namespace jojogame {
std::once_flag CControlManager::s_onceFlag;
std::unique_ptr<CControlManager> CControlManager::s_controlManager;

void CControlManager::RegisterFunctions(lua_State* L)
{
    LUA_BEGIN(CControlManager, "ControlManager");

    LUA_METHOD(CreateWindowForm);
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

    CLuaTinker::GetLuaTinker().RegisterClass<CControlManager>();
    CLuaTinker::GetLuaTinker().RegisterClass<CBaseControl>();
    CLuaTinker::GetLuaTinker().RegisterClass<CWindowControl>();
    CLuaTinker::GetLuaTinker().RegisterClass<CMoviePlayerControl>();
}

CWindowControl* CControlManager::CreateWindowForm(CWindowControl *parent)
{
    auto newWindow = new CWindowControl();
    newWindow->SetParentWindow(parent);
    return newWindow;
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
    std::call_once(s_onceFlag, [] {
        s_controlManager.reset(new CControlManager);
    });

    return *s_controlManager.get();
}
}