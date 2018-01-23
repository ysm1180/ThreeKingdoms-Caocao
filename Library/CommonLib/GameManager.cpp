#include "stdafx.h"

#include "GameManager.h"

namespace jojogame {

std::once_flag CGameManager::s_onceFlag;
std::unique_ptr<CGameManager> CGameManager::s_sharedGameManager;

void CGameManager::RegisterFunctions(lua_State * L)
{
    LUA_BEGIN(CGameManager, "GameManager");

    LUA_METHOD(GetDesktopWidth);
    LUA_METHOD(GetDesktopHeight);
    LUA_METHOD(Quit);
    LUA_METHOD(Delay);
    LUA_METHOD(StopDelay);
    LUA_METHOD(Color);
}

CGameManager::CGameManager()
{
}

CGameManager::~CGameManager()
{
}

CGameManager& CGameManager::GetInstance()
{
    std::call_once(s_onceFlag,
        [] {
        s_sharedGameManager.reset(new CGameManager);
    });

    return *s_sharedGameManager.get();
}

int CGameManager::GetDesktopWidth()
{
    RECT rect;
    GetWindowRect(GetDesktopWindow(), &rect);
    return rect.right - rect.left;
}

int CGameManager::GetDesktopHeight()
{
    RECT rect;
    GetWindowRect(GetDesktopWindow(), &rect);
    return rect.bottom - rect.top;
}

COLORREF CGameManager::Color(int r, int g, int b)
{
    return RGB(r, g, b);
}

void CGameManager::Quit()
{
    PostQuitMessage(0);
}


void CGameManager::Delay(int time)
{
    const auto starttime = GetTickCount();
    MSG message;
    auto quit = false;

    for (;;)
    {
        if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
        {
            if (message.message == WM_QUIT)
            {
                quit = true;
                break;
            }
            if (message.message == WM_STOP_DELAY)
            {
                break;
            }
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        else
        {
            if ((GetTickCount() - starttime) >= static_cast<DWORD>(time))
            {
                break;
            }
        }
    }

    if (quit)
    {
        PostQuitMessage(0);
    }
}

void CGameManager::StopDelay()
{
    PostMessage(nullptr, WM_STOP_DELAY, 0, 0);
}
}