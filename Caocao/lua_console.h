#pragma once

#define OUTPUT_MAX_LENGTH 4096

#include <Windows.h>

#include <memory>
#include <mutex>
#include <string>

#include "BaseLib\console.h"

namespace three_kingdoms {
class LuaConsole : public Console {
 public:
  LuaConsole();
  virtual ~LuaConsole();

  HWND GetHWnd();

  void Output(std::wstring msg);
  void AppendOutput(std::wstring msg);

  void Create(HINSTANCE hInstance);

  static LuaConsole &GetInstance();
  static void SetDebugFlag(bool flag);

 private:
  HWND _hWnd;
  HINSTANCE _hInstance;

  static std::once_flag s_onceFlag;
  static std::unique_ptr<LuaConsole> s_sharedLuaConsole;
};
}  // namespace three_kingdoms
