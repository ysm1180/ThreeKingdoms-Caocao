#pragma once

#define WM_STOP_DELAY (WM_USER + 1)

#include "LuaLib\LuaTinker.h"

#include <windows.h>
#include <mutex>
#include <memory>
#ifndef _MSVC_LANG
#include "BaseLib\mingw.mutex.h"
#endif

namespace jojogame {
class CGameManager
{
public:
    static void RegisterFunctions(lua_State *L);

    CGameManager();
    ~CGameManager();

    int GetDesktopWidth();
    int GetDesktopHeight();

    COLORREF Color(int r, int g, int b);

    void Quit();

    void Delay(int time);
    void StopDelay();

    static CGameManager& GetInstance();
private:
    static std::once_flag s_onceFlag;
    static std::unique_ptr<CGameManager> s_sharedGameManager;
};
}
