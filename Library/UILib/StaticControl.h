#pragma once

#include "WindowChildControl.h"
#include "TextFont.h"

#include "LuaLib/LuaTinker.h"

namespace three_kingdoms
{
class CStaticControl : public CWindowChildControl
{
public:
    static void RegisterFunctions(lua_State *L);

    CStaticControl();
    virtual ~CStaticControl();

    bool IsAutoSize();
    bool IsTransparentBackground();
    int GetAlign();
    std::wstring GetText();
    CTextFont *GetFont();
    COLORREF GetTextColor();
    HBRUSH GetBackgroundBrush();

    void SetAutoSize(bool value);
    void SetTransparentBackground(bool value);
    void SetAlign(int align);
    void SetText(std::wstring text);
    void SetTextColor(COLORREF color);
    void SetBackgroundColor(COLORREF color);

    bool Create() override;

    void Show() override;

    static WNDPROC GetOriginalProc();
    static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    void _AutoSizing();

    CTextFont _font;

    bool _isAutoSize = true;
    bool _isTransparentBackground = true;

    int _align = 0; // 0 : left, 1 : right, 2 : center
    std::wstring _text = L"";

    COLORREF _textColor = RGB(0, 0, 0);
    COLORREF _backgroundColor = GetSysColor(COLOR_3DFACE);
    HBRUSH _backgroundBrush = CreateSolidBrush(GetSysColor(COLOR_3DFACE));

    static WNDPROC s_originalProc;
};

} // namespace three_kingdoms
