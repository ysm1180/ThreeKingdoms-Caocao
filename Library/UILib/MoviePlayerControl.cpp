#include "stdafx.h"

#include "MoviePlayerControl.h"
#include "ControlManager.h"
#include "WindowControl.h"

#include <MMSystem.h>
#include <Vfw.h>

namespace jojogame {
LRESULT CALLBACK CMoviePlayerControl::OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    auto moviePlayer = reinterpret_cast<CMoviePlayerControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (msg)
    {
    case WM_LBUTTONUP:
    {
        CLuaTinker::GetLuaTinker().Call<void>(moviePlayer->GetClickFunction().c_str());
    }
    }

    return CallWindowProc(moviePlayer->GetOldProc(), hWnd, msg, wParam, lParam);
}

void CMoviePlayerControl::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN_CHILD(CMoviePlayerControl, "MoviePlayer", CBaseControl);

    LUA_METHOD(Play);
    LUA_METHOD(Stop);
    LUA_METHOD(Create);
    LUA_METHOD(Destroy);
}

CMoviePlayerControl::CMoviePlayerControl(CWindowControl* parent, std::wstring fileName)
{
    _parentHWnd = parent->GetHWnd();
    _fileName = fileName;
}

CMoviePlayerControl::~CMoviePlayerControl()
{
    CMoviePlayerControl::Destroy();
}

WNDPROC CMoviePlayerControl::GetOldProc()
{
    return _oldProc;
}

void CMoviePlayerControl::Play()
{
    auto quit = false;

    MCIWndPlay(_hWnd);

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
        else if (_hWnd == nullptr)
        {
            break;
        }
    }

    Stop();
    if (quit)
    {
        PostQuitMessage(0);
    }
}

void CMoviePlayerControl::Stop()
{
    if (_hWnd != nullptr)
    {
        MCIWndStop(_hWnd);
        MCIWndClose(_hWnd);
    }
}

void CMoviePlayerControl::Create()
{
    if (_hWnd != nullptr)
    {
        Destroy();
    }

    _hWnd = MCIWndCreate(_parentHWnd, CControlManager::GetInstance().GetHInstance(),
        MCIWNDF_NOTIFYANSI | MCIWNDF_NOMENU | MCIWNDF_NOTIFYALL | MCIWNDF_NOPLAYBAR | MCI_WAIT,
        _fileName.c_str());
    _oldProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(_hWnd, GWLP_WNDPROC, (LONG_PTR)OnControlProc));
    SetWindowLongPtr(_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
}

void CMoviePlayerControl::Destroy()
{
    if (_hWnd != nullptr)
    {
        SetWindowLongPtr(_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(_oldProc));
        MCIWndStop(_hWnd);
        MCIWndClose(_hWnd);
        MCIWndDestroy(_hWnd);
        _hWnd = nullptr;
    }
}


}