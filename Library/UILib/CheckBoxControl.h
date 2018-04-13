#pragma once
#include "BaseControl.h"
#include "TextFont.h"

namespace jojogame {
class CCheckBoxControl : public CBaseControl
{
public:
    static void RegisterFunctions(lua_State* L);

    CCheckBoxControl();
    virtual ~CCheckBoxControl();

    bool IsChecked();
    std::wstring GetText();
    CTextFont* GetFont();

    void SetText(std::wstring text);
    void SetChecked(bool checked);

    bool Create() override;

    static WNDPROC GetOriginalProc();
    static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    CTextFont _font;
    std::wstring _text;

    static WNDPROC s_originalProc;
};
}
