#pragma once
#pragma comment(lib, "vfw32.lib")

#include "BaseControl.h"

#include <string>
#include <Windows.h>

namespace jojogame {
class CWindowControl;

class CMoviePlayerControl : public CBaseControl
{
public:
    static void RegisterFunctions(lua_State *L);

    CMoviePlayerControl(CWindowControl *parent, std::wstring fileName);
    virtual ~CMoviePlayerControl();

    WNDPROC GetOldProc();

    bool IsPlaying();

    void SetFileName(std::wstring fileName);

    void Play();
    void WaitForPlay();
    void Stop();

    bool Create() override;
    void Destroy() override;

    static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
    std::wstring _fileName = L"";
    bool _playing = false;
    
    WNDPROC _originalProc;
};
}
