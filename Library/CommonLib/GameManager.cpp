#include "GameManager.h"
#include "BaseLib/MemoryPool.h"
#include "ME5File.h"
#include <atomic>
#include <future>

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
    LUA_METHOD(SetInterval);
    LUA_METHOD(ClearInterval);
}

CGameManager::CGameManager()
{
}

CGameManager::~CGameManager()
{

}

int CGameManager::GetTokenRef(int tokenId)
{
    auto iter = _tokens.find(tokenId);
    if (iter == _tokens.end())
    {
        return -1;
    }
    else
    {
        return iter->second.second;
    }
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
        int gap = (GetTickCount() - starttime);
        if (gap >= static_cast<DWORD>(time))
        {
            break;
        }

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

template <class F, class... Args>
std::thread* setInterval(std::atomic_bool& cancelToken, size_t interval, F&& f, Args&&... args)
{
    cancelToken.store(true);
    auto cb = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
    std::packaged_task<void()> task([=, &cancelToken]()
    {
        while (cancelToken.load())
        {
            cb();
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    });
    auto fw = task.get_future();
    std::thread* thread = new std::thread(std::move(task));
    return thread;
}

void runIntervalFunction(int tokenId)
{
    auto gameManager = CGameManager::GetInstance();
    auto ref = gameManager.GetTokenRef(tokenId);
    CLuaTinker::GetLuaTinker().Call(ref);
}

int CGameManager::SetInterval()
{
    auto l = CLuaTinker::GetLuaTinker().GetLuaState();
    int interval = 0;
    int ref = -1;
    if (lua_isfunction(l, -2))
    {
        interval = lua_tonumber(l, -1);
        lua_pushvalue(l, -2);
        ref = luaL_ref(l, LUA_REGISTRYINDEX);
    }
    lua_pop(l, 2);

    std::atomic_bool *token = CMemoryPool<std::atomic_bool>::GetInstance().New();

    _tokens[_tokenId] = std::pair<std::atomic_bool *, int>(token, ref);
        
    if (interval != 0)
    {
        int tokenId = _tokenId;
        auto thread = setInterval(*token, interval, runIntervalFunction, tokenId);
        _threads[tokenId] = thread;
    }
    
    return _tokenId++;
}

void CGameManager::ClearInterval(int tokenId)
{
    _tokens[tokenId].first->store(false);

    _threads[tokenId]->join();
    delete _threads[tokenId];

    CMemoryPool<std::atomic_bool>::GetInstance().Delete(_tokens[tokenId].first);

    auto l = CLuaTinker::GetLuaTinker().GetLuaState();
    luaL_unref(l, LUA_REGISTRYINDEX, _tokens[tokenId].second);

    _tokens.erase(tokenId);
}

void CGameManager::AllClearInterval()
{
    for (auto token: _tokens)
    {
        token.second.first->store(false);

        _threads[token.first]->join();
        delete _threads[token.first];

        CMemoryPool<std::atomic_bool>::GetInstance().Delete(token.second.first);

        auto l = CLuaTinker::GetLuaTinker().GetLuaState();
        luaL_unref(l, LUA_REGISTRYINDEX, token.second.second);
    }
    _tokens.clear();
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
