#pragma once

#include "BaseControl.h"
#include "TextFont.h"

#include <Windows.h>

namespace jojogame {
class CWindowControl;

class  CButtonControl : public CBaseControl
{
public:
    static void RegisterFunctions(lua_State *L);

    CButtonControl();
    ~CButtonControl() override;

    bool IsTransparentBackground();
    bool IsTransparentBorder();
    CTextFont* GetFont();
    std::wstring GetText();

    void SetTransparentBackground(bool isTransparentBackground);
    void SetTransparentBorder(bool isTransparentBorder);
    void SetText(std::wstring text);
    void SetParentWindow(CWindowControl *parent);

    bool Create() override;
    void Destroy() override;

    static WNDPROC GetOriginalProc();
    static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
    CTextFont _font;
    std::wstring _text = L"";
    bool _isTransparentBackground = false;
    bool _isTransparentBorder = false;
    
    static WNDPROC s_originalProc;
};
}
