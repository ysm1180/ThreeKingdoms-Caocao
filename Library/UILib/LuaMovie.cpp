#pragma comment(lib, "vfw32.lib")

#include "LuaMovie.h"
#include "LuaForm.h"
#include "Application.h"
#include "../LuaJojo.h"
#include "../LuaTinker.h"

#include <windows.h>
#include <MMSystem.h>
#include <Vfw.h>

namespace jojogame {
LRESULT CALLBACK MovieProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
    LuaMovie* playedMovie = reinterpret_cast<LuaMovie *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (iMessage)
    {
        case WM_LBUTTONUP:
        {
            lua_tinker::call<void>(LuaJojo::getInstance()->getLuaState(), playedMovie->clickFunction().c_str());
        }
    }

    return CallWindowProc(playedMovie->oldProc(), hwnd, iMessage, wParam, lParam);
}

LuaMovie::LuaMovie(CWindow* parent)
{
    _parentHwnd = parent->parentHwnd();
}

LuaMovie::~LuaMovie()
{
    LuaMovie::destroy();
}

WNDPROC LuaMovie::oldProc()
{
    return _oldProc;
}

void LuaMovie::play()
{
    auto quit = false;

    MCIWndPlay(_hwnd);

    // Loop
    MSG message;
    for (;;)
    {
        if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
        {
            if (message.message == WM_QUIT)
            {
                quit = true;
                break;
            }

            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        else if (_hwnd == nullptr)
        {
            break;
        }
    }

    stop();
    if (quit)
    {
        PostQuitMessage(0);
    }
}

void LuaMovie::stop()
{
    if (_hwnd != nullptr)
    {
        MCIWndStop(_hwnd);
        MCIWndClose(_hwnd);
    }
}

void LuaMovie::create(std::wstring fileName)
{
    if (_hwnd != nullptr)
    {
        destroy();
    }

    _hwnd = MCIWndCreate(_parentHwnd, Application::getInstance()->getHInstance(),
                         MCIWNDF_NOTIFYANSI | MCIWNDF_NOMENU | MCIWNDF_NOTIFYALL | MCIWNDF_NOPLAYBAR | MCI_WAIT,
                         fileName.c_str());
    _oldProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(MovieProc)));
    SetWindowLongPtr(_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

void LuaMovie::destroy()
{
    if (_hwnd != nullptr)
    {
        SetWindowLongPtr(_hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(_oldProc));
        MCIWndStop(_hwnd);
        MCIWndClose(_hwnd);
        MCIWndDestroy(_hwnd);
        _hwnd = nullptr;
    }
}

void LuaMovie::registerToLua()
{
    lua_State* lua = LuaJojo::getInstance()->getLuaState();

    lua_tinker::class_add<LuaMovie>(lua, "Movie");

    lua_tinker::class_inh<LuaMovie, CControl>(lua);
    lua_tinker::class_con<LuaMovie>(lua, lua_tinker::constructor<LuaMovie, CWindow*>);
    lua_tinker::class_def<LuaMovie>(lua, "play", &LuaMovie::play);
    lua_tinker::class_def<LuaMovie>(lua, "stop", &LuaMovie::stop);
    lua_tinker::class_def<LuaMovie>(lua, "create", &LuaMovie::create);
    lua_tinker::class_def<LuaMovie>(lua, "destroy", &LuaMovie::destroy);
}

}
