#pragma once

#define OUTPUT_MAX_LENGTH 4096

#include <Windows.h>

#include <memory>
#include <mutex>
#include <string>

#include "BaseLib\ConsoleOutput.h"

namespace three_kingdoms {
class CLuaConsole : public CConsoleOutput {
 public:
  CLuaConsole();
  virtual ~CLuaConsole();

  HWND GetHWnd();

  void Output(std::wstring msg);
  void AppendOutput(std::wstring msg);

  void Create(HINSTANCE hInstance);

  static CLuaConsole &GetInstance();
  static void SetDebugFlag(bool flag);

 private:
  HWND _hWnd;
  HINSTANCE _hInstance;

  static std::once_flag s_onceFlag;
  static std::unique_ptr<CLuaConsole> s_sharedLuaConsole;
};
}  // namespace three_kingdoms
