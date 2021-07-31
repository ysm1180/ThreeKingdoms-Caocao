#pragma once
#include "WindowChildControl.h"
#include "TextFont.h"
#include <Uxtheme.h>

namespace three_kingdoms
{
class CCheckBoxControl : public CWindowChildControl
{
public:
    static void RegisterFunctions(lua_State *L);

    CCheckBoxControl();
    virtual ~CCheckBoxControl();

    bool IsChecked();
    std::wstring GetText();
    CTextFont *GetFont();
    HTHEME GetTheme();

    void SetText(std::wstring text);
    void SetChecked(bool checked);

    bool Create() override;

    static WNDPROC GetOriginalProc();
    static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    CTextFont _font;
    std::wstring _text;
    HTHEME _theme;
    bool _isChecked = false;

    static WNDPROC s_originalProc;
};
} // namespace jojogame
