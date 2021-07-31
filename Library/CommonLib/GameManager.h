#pragma once

#define WM_STOP_DELAY (WM_USER + 1)

#include "LuaLib\LuaTinker.h"

#include <windows.h>
#include <mutex>
#include <memory>

namespace three_kingdoms
{
class CME5File;

class CGameManager
{
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
    void SetQuit(bool value);

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
} // namespace jojogame
