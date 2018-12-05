#include "Application.h"
#include "LuaConsole.h"

#include "BaseLib/MemoryPool.h"
#include "CommonLib/GameManager.h"
#include "CommonLib/FileManager.h"
#include "LuaLib/LuaTinker.h"
#include "UILib/ControlManager.h"
#include "CommonLib/ME5File.h"

namespace jojogame {
Application* Application::s_sharedApplication = nullptr;

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
    MSG message{};
    CLuaTinker& luaTinker = CLuaTinker::GetLuaTinker();
    CLuaConsole& luaConsole = CLuaConsole::GetInstance();
    bool debug = false;

    av_register_all();
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0)
    {
        return 1;
    }

    _controlManager = &CControlManager::GetInstance();
    _gameManager = &CGameManager::GetInstance();
    _fileManager = &CFileManager::GetInstance();

    _controlManager->Init(_hInstance);

    luaConsole.Create(_hInstance);

    luaTinker.RegisterClassToLua<CME5File>();
    luaTinker.RegisterClassToLua<CGameManager>();
    luaTinker.RegisterClassToLua<CFileManager>();

    luaTinker.RegisterVariable("controlManager", _controlManager);
    luaTinker.RegisterVariable("gameManager", _gameManager);
    luaTinker.RegisterVariable("fileManager", _fileManager);

    luaTinker.RegisterFunction("OUTPUT", &CConsoleOutput::OutputConsoles);
    luaTinker.RegisterFunction("DEBUG", &CLuaConsole::SetDebugFlag);

    luaTinker.Run("./Script/main.lua");

    while (WM_QUIT != message.message)
    {
        if (PeekMessage(&message, nullptr, 0, 0, PM_NOREMOVE))
        {
            GetMessage(&message, nullptr, 0, 0);
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        else
        {
            int idleEvent = CGameManager::GetInstance().GetIdleEvent();
            if (idleEvent != LUA_NOREF)
            {
                luaTinker.Call(idleEvent);
            }
        }
    }

    _gameManager->SetQuit(true);
    CMemoryPoolManager::GetInstance().DestroyAllMemoryPool();
    SDL_Quit();

    return (int)message.wParam;
}
}
