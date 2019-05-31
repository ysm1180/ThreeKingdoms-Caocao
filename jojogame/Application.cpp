#include <ole2.h>
#include <gdiplus.h>
#include <chrono>

#pragma comment(lib, "gdiplus.lib")

#include "Application.h"
#include "LuaConsole.h"

#include "BaseLib/MemoryPool.h"
#include "CommonLib/GameManager.h"
#include "CommonLib/FileManager.h"
#include "LuaLib/LuaTinker.h"
#include "UILib/ControlManager.h"
#include "UILib/WindowControl.h"
#include "UILib/LayoutControl.h"
#include "CommonLib/ME5File.h"

using namespace std::chrono_literals;

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

void Application::Render()
{
    auto layouts = _controlManager->GetLayouts();
    for (auto layout : layouts)
    {
        layout->Refresh();
    }
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

    ULONG_PTR gpToken;
    Gdiplus::GdiplusStartupInput gpsi;
    if (Gdiplus::GdiplusStartup(&gpToken, &gpsi, nullptr) != Gdiplus::Ok)
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

    using clock = std::chrono::high_resolution_clock;

    constexpr std::chrono::nanoseconds timestep(16ms);
    std::chrono::nanoseconds lag(0ns);
    auto time_start = clock::now();

    while (WM_QUIT != message.message)
    {
        auto delta_time = clock::now() - time_start;
        time_start = clock::now();
        lag += std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);

        if (PeekMessage(&message, nullptr, 0, 0, PM_NOREMOVE))
        {
            GetMessage(&message, 0, 0, 0);
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        
        while (lag >= timestep)
        {
            lag -= timestep;

            auto updateEvent = _gameManager->GetUpdateEvent();
            if (updateEvent != LUA_NOREF)
            {
                CLuaTinker::GetLuaTinker().Call(updateEvent);
            }
        }

        Render();
    }

    _gameManager->SetQuit(true);
    CMemoryPoolManager::GetInstance().DestroyAllMemoryPool();
    SDL_Quit();

    Gdiplus::GdiplusShutdown(gpToken);

    return (int)message.wParam;
}
}
