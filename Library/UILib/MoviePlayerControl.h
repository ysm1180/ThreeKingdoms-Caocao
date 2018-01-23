#pragma comment(lib, "vfw32.lib")

#pragma once

#include "BaseControl.h"

namespace jojogame {
class CWindowControl;

class CMoviePlayerControl : public CBaseControl
{
public:
    static void RegisterFunctions(lua_State *L);

    CMoviePlayerControl(CWindowControl *parent, std::wstring fileName);
    virtual ~CMoviePlayerControl();

    WNDPROC GetOldProc();

    void Play();
    void Stop();

    void Create() override;
    void Destroy() override;

    static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
    WNDPROC _oldProc;
    std::wstring _fileName = L"";
};
}
