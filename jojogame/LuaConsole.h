#pragma once

#include "BaseLib\ConsoleOutput.h"
#ifndef _MSVC_LANG
#include "BaseLib\mingw.mutex.h"
#endif

#include "resource.h"

#include <memory>
#include <mutex>
#include <string>

#include <Windows.h>

namespace jojogame {
class CLuaConsole : public CConsoleOutput
{
public:
    CLuaConsole();
    virtual ~CLuaConsole();

    HWND GetHWnd();

    void SetHInstance(HINSTANCE hInstance);

    void Output(std::string msg);
    void AppendOutput(std::string msg);

    void Create();

    static CLuaConsole& GetInstance();
    static void SetDebugFlag(bool flag);
private:
    HWND _hWnd;
    HINSTANCE _hInstance;

    static std::once_flag s_onceFlag;
    static std::unique_ptr<CLuaConsole> s_sharedLuaConsole;
};
}

