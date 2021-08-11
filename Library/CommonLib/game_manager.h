#pragma once

#define WM_STOP_DELAY (WM_USER + 1)

#include <windows.h>

#include <memory>
#include <mutex>

#include "LuaLib\lua_tinker_helper.h"

namespace three_kingdoms {
class CME5File;

class CGameManager {
 public:
  static void RegisterFunctions(lua_State *L);

  CGameManager();
  ~CGameManager();

  int GetDesktopWidth();
  int GetDesktopHeight();
  int GetUpdateEvent();
  int GetNow();

  bool IsQuit();

  COLORREF Color(int r, int g, int b);

  void Quit();

  void Delay(int time);
  void StopDelay();

  void SetUpdateEvent();

  CME5File *OpenFile(std::wstring path);
  void CloseFile(CME5File *file);

  static CGameManager &GetInstance();

 private:
  static std::once_flag s_onceFlag;
  static std::unique_ptr<CGameManager> s_sharedGameManager;

  int _updateEvent = LUA_NOREF;

  bool _quit = false;
};
}  // namespace three_kingdoms
