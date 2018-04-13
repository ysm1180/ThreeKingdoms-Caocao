#include "GameManager.h"
#include "BaseLib/MemoryPool.h"
#include "ME5File.h"

namespace jojogame {
std::once_flag CGameManager::s_onceFlag;
std::unique_ptr<CGameManager> CGameManager::s_sharedGameManager;

void CGameManager::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN(CGameManager, "_GameManager");

    LUA_METHOD(GetDesktopWidth);
    LUA_METHOD(GetDesktopHeight);
    LUA_METHOD(Quit);
    LUA_METHOD(Delay);
    LUA_METHOD(StopDelay);
    LUA_METHOD(Color);
    LUA_METHOD(Clock);
    LUA_METHOD(OpenFile);
    LUA_METHOD(CloseFile);
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
                   []
    {
        s_sharedGameManager = std::make_unique<jojogame::CGameManager>();
    });

    return *s_sharedGameManager;
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

int CGameManager::Clock()
{
    return GetTickCount();
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

CME5File* CGameManager::OpenFile(std::wstring path)
{
    auto file = CMemoryPool<CME5File>::GetInstance().New();
    file->Open(path);
    return file;
}

void CGameManager::CloseFile(CME5File* file)
{
    CMemoryPool<CME5File>::GetInstance().Delete(file);
}
}
