#pragma comment(lib, "gdiplus.lib")

#include "application.h"

#include <gdiplus.h>
#include <ole2.h>

#include "BaseLib/memory_pool.h"
#include "CommonLib/file_manager.h"
#include "CommonLib/game_manager.h"
#include "CommonLib/me5_file.h"
#include "lua_console.h"
#include "LuaLib/lua_tinker_helper.h"
#include "UILib/control_manager.h"
#include "UILib/layout.h"
#include "UILib/window.h"

using namespace std::chrono_literals;

namespace three_kingdoms {
Application* Application::s_sharedApplication = nullptr;

Application& Application::GetInstance() { return *s_sharedApplication; }

Application::Application(HINSTANCE instance) {
  _hInstance = instance;
  s_sharedApplication = this;
}

Application::~Application() { s_sharedApplication = nullptr; }

HINSTANCE Application::GetHInstance() { return _hInstance; }

void Application::Render() {
  auto layouts = _controlManager->GetLayouts();
  for (auto layout : layouts) {
    layout->Refresh();
  }
}

int Application::Run() {
  MSG message{};
  LuaTinkerHelper& luaTinker = LuaTinkerHelper::GetLuaTinker();
  LuaConsole& luaConsole = LuaConsole::GetInstance();
  bool debug = false;

#ifndef __WIN64
  av_register_all();
#endif
  if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
    return 1;
  }

  ULONG_PTR gpToken;
  Gdiplus::GdiplusStartupInput gpsi;
  if (Gdiplus::GdiplusStartup(&gpToken, &gpsi, nullptr) != Gdiplus::Ok) {
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

  luaTinker.RegisterFunction("OUTPUT", &Console::OutputConsoles);
  luaTinker.RegisterFunction("DEBUG", &LuaConsole::SetDebugFlag);

  luaTinker.Run("./Script/main.lua");

  using clock = std::chrono::high_resolution_clock;

  constexpr std::chrono::nanoseconds timestep(16ms);
  std::chrono::nanoseconds lag(0ns);
  auto time_start = clock::now();

  while (!_gameManager->IsQuit()) {
    auto delta_time = clock::now() - time_start;
    time_start = clock::now();
    lag += std::chrono::duration_cast<std::chrono::nanoseconds>(delta_time);

    if (PeekMessage(&message, nullptr, 0, 0, PM_NOREMOVE)) {
      GetMessage(&message, 0, 0, 0);
      TranslateMessage(&message);
      DispatchMessage(&message);
    }

    while (lag >= timestep) {
      lag -= timestep;

      auto updateEvent = _gameManager->GetUpdateEvent();
      if (updateEvent != LUA_NOREF) {
        LuaTinkerHelper::GetLuaTinker().Call(updateEvent);
      }
    }

    Render();
  }

  MemoryPoolManager::GetInstance().DestroyAllMemoryPool();
  SDL_Quit();

  Gdiplus::GdiplusShutdown(gpToken);

  return (int)message.wParam;
}
}  // namespace three_kingdoms