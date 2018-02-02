#include "MoviePlayerControl.h"
#include "ControlManager.h"
#include "WindowControl.h"


#include "BaseLib\ConsoleOutput.h"

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
        CLuaTinker::GetLuaTinker().Call(moviePlayer->GetMouseLButtonUpEvent().c_str(), moviePlayer);
    }
    case WM_LBUTTONDOWN:
    {
        CLuaTinker::GetLuaTinker().Call(moviePlayer->GetMouseLButtonDownEvent().c_str(), moviePlayer);
    }
    }

    return CallWindowProc(moviePlayer->GetOldProc(), hWnd, msg, wParam, lParam);
}

void CMoviePlayerControl::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN_CHILD(CMoviePlayerControl, "_MoviePlayer", CBaseControl);

    LUA_METHOD(IsPlaying);
    LUA_METHOD(SetEndEvent);

    LUA_METHOD(Play);
    LUA_METHOD(WaitForPlay);
    LUA_METHOD(Stop);
    LUA_METHOD(Create);
    LUA_METHOD(Destroy);


}

CMoviePlayerControl::CMoviePlayerControl(CWindowControl* parent, std::wstring fileName)
{
    if (parent)
    {
        _parentHWnd = parent->GetHWnd();
    }
    _fileName = fileName;
}

CMoviePlayerControl::~CMoviePlayerControl()
{
    CMoviePlayerControl::Destroy();
}

WNDPROC CMoviePlayerControl::GetOldProc()
{
    return _originalProc;
}

bool CMoviePlayerControl::IsPlaying()
{
    return _playing;
}

std::wstring CMoviePlayerControl::GetEndEvent()
{
    return _endEvent;
}

void CMoviePlayerControl::SetEndEvent(std::wstring endEvent)
{
    _endEvent = endEvent;
}

void CMoviePlayerControl::SetFileName(std::wstring fileName)
{
    _fileName = fileName;
}

void CMoviePlayerControl::Play()
{
    MCIWndOpen(_hWnd, _fileName.c_str(), 0);
    MCIWndPlay(_hWnd);
    _playing = true;
}

void CMoviePlayerControl::WaitForPlay()
{
    auto quit = false;
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
        else if (!_playing)
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
        _playing = false;
    }
}

bool CMoviePlayerControl::Create()
{
    if (_hWnd != nullptr)
    {
        Destroy();
    }

    AVIFileInit();

    PAVIFILE aviFile;
    int error = AVIFileOpen(&aviFile, _fileName.c_str(), OF_READ, NULL);
    if (error) {
        AVIFileExit();
        return false;
    }

    AVIFILEINFO aviInfo;
    AVIFileInfo(aviFile, &aviInfo, sizeof(aviInfo));

    int playRate = aviInfo.dwRate / aviInfo.dwScale;
    PAVISTREAM aviStream;
    AVIFileGetStream(aviFile, &aviStream, 0, 0);
    AVISTREAMINFO aviStreamInfo;
    AVIStreamInfo(aviStream, &aviStreamInfo, sizeof(aviStreamInfo));


    //_hWnd = MCIWndCreateW(_parentHWnd, CControlManager::GetInstance().GetHInstance(),
    //    MCIWNDF_NOTIFYANSI | MCIWNDF_NOMENU | MCIWNDF_NOTIFYPOS | MCIWNDF_NOPLAYBAR | MCI_WAIT,
    //    _fileName.c_str());
    
    RECT rect;
    GetClientRect(_hWnd, &rect);
    _size.cx = rect.right - rect.left;
    _size.cy = rect.bottom - rect.top;

    _originalProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(_hWnd, GWLP_WNDPROC, (LONG_PTR)CMoviePlayerControl::OnControlProc));
    SetWindowLongPtr(_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    return true;
}

void CMoviePlayerControl::Destroy()
{
    if (_hWnd != nullptr)
    {
        SetWindowLongPtr(_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(_originalProc));
        MCIWndStop(_hWnd);
        MCIWndClose(_hWnd);
        MCIWndDestroy(_hWnd);
        _playing = false;
        _hWnd = nullptr;
    }
}
}