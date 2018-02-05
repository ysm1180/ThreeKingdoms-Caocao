#pragma once

#include "BaseControl.h"
#include "TextFont.h"

#include <Windows.h>

namespace jojogame {
class  CButtonControl : public CBaseControl
{
public:
    static void RegisterFunctions(lua_State *L);

    CButtonControl();
    virtual ~CButtonControl();

    CTextFont& GetFont();
    std::wstring GetText();

    void SetText(std::wstring text);

    bool Create() override;
    void Destroy() override;

    static WNDPROC GetOriginalProc();
    static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
    CTextFont _font;
    std::wstring _text = L"";
    
    static WNDPROC s_originalProc;
};
}
