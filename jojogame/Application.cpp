#include "Application.h"
#include "LuaConsole.h"

#include "BaseLib\MemoryPool.h"
#include "CommonLib\GameManager.h"
#include "LuaLib\LuaTinker.h"
#include "UILib\ControlManager.h"

namespace jojogame {
Application *Application::s_sharedApplication = nullptr;

Application& Application::GetInstance()
{
    return *s_sharedApplication;
}

Application::Application(HINSTANCE instance)
{
    _hInstance = instance;
    s_sharedApplication = this;
}

Application::~Application()
{
    s_sharedApplication = nullptr;
}

HINSTANCE Application::GetHInstance()
{
    return _hInstance;
}

int Application::Run()
{
    MSG message;
    CLuaTinker& luaTinker = CLuaTinker::GetLuaTinker();
    CLuaConsole& luaConsole = CLuaConsole::GetInstance();
    bool debug = false;

    _controlManager = &CControlManager::GetInstance();
    _gameManager = &CGameManager::GetInstance();

    _controlManager->Init(_hInstance);

    luaConsole.SetHInstance(_hInstance);
    luaConsole.Create();

    luaTinker.RegisterClassToLua<CGameManager>();

    luaTinker.RegisterVariable("controlManager", _controlManager);
    luaTinker.RegisterVariable("gameManager", _gameManager);

    luaTinker.RegisterFunction("DEBUGLUA", &CLuaConsole::SetDebugFlag);

    luaTinker.Run("Script\\main.lua");
    while (GetMessage(&message, 0, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    CMemoryPoolManager::GetInstance().DestroyAllMemoryPool();

    return (int) message.wParam;
}
}