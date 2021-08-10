#include "game_manager.h"

#include <atomic>
#include <future>

#include "BaseLib/memory_pool.h"
#include "LuaLib/lua_tinker_helper.h"
#include "me5_file.h"

namespace three_kingdoms {
std::once_flag CGameManager::s_onceFlag;
std::unique_ptr<CGameManager> CGameManager::s_sharedGameManager;

void CGameManager::RegisterFunctions(lua_State *L) {
  LUA_BEGIN(CGameManager, "_GameManager");

  LUA_METHOD(GetDesktopWidth);
  LUA_METHOD(GetDesktopHeight);
  LUA_METHOD(Quit);
  LUA_METHOD(Delay);
  LUA_METHOD(StopDelay);
  LUA_METHOD(Color);
  LUA_METHOD(GetNow);
  LUA_METHOD(OpenFile);
  LUA_METHOD(CloseFile);
  LUA_METHOD(SetUpdateEvent);
}

CGameManager::CGameManager() {}

CGameManager::~CGameManager() {}

CGameManager &CGameManager::GetInstance() {
  std::call_once(s_onceFlag, [] {
    s_sharedGameManager = std::make_unique<three_kingdoms::CGameManager>();
  });

  return *s_sharedGameManager;
}

int CGameManager::GetDesktopWidth() {
  RECT rect;
  GetWindowRect(GetDesktopWindow(), &rect);
  return rect.right - rect.left;
}

int CGameManager::GetDesktopHeight() {
  RECT rect;
  GetWindowRect(GetDesktopWindow(), &rect);
  return rect.bottom - rect.top;
}

int CGameManager::GetUpdateEvent() { return _updateEvent; }

int CGameManager::GetNow() { return GetTickCount(); }

bool CGameManager::IsQuit() { return _quit; }

COLORREF CGameManager::Color(int r, int g, int b) { return RGB(r, g, b); }

void CGameManager::Quit() { _quit = true; }

void CGameManager::Delay(int time) {
  const auto starttime = GetTickCount();
  MSG message;
  auto quit = false;

  for (;;) {
    int gap = (GetTickCount() - starttime);
    if (gap >= static_cast<DWORD>(time)) {
      break;
    }

    if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
      if (message.message == WM_QUIT) {
        quit = true;
        break;
      }
      if (message.message == WM_STOP_DELAY) {
        break;
      }
      TranslateMessage(&message);
      DispatchMessage(&message);
    }
  }

  if (quit) {
    PostQuitMessage(0);
  }
}

void CGameManager::StopDelay() { PostMessage(nullptr, WM_STOP_DELAY, 0, 0); }

void CGameManager::SetUpdateEvent() {
  auto l = LuaTinkerHelper::GetLuaTinker().GetLuaState();
  if (lua_isfunction(l, -1)) {
    lua_pushvalue(l, -1);
    _updateEvent = luaL_ref(l, LUA_REGISTRYINDEX);
  }

  lua_pop(l, 1);
}

CME5File *CGameManager::OpenFile(std::wstring path) {
  auto file = CMemoryPool<CME5File>::GetInstance().New();
  file->Open(path);
  return file;
}

void CGameManager::CloseFile(CME5File *file) {
  CMemoryPool<CME5File>::GetInstance().Delete(file);
}
}  // namespace three_kingdoms