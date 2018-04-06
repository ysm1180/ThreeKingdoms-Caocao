#pragma once

#include "BaseControl.h"
#include "TextFont.h"

#include <Windows.h>
#include <Commctrl.h>

namespace jojogame {
class CWindowControl;

struct ButtonStateColor
{
    COLORREF normal;
    COLORREF focused;
    COLORREF pushed;
};

class CButtonControl : public CBaseControl
{
public:
    static void RegisterFunctions(lua_State* L);

    CButtonControl();
    virtual ~CButtonControl();

    bool IsTransparentBackground() const;
    CTextFont* GetFont();
    std::wstring GetText() const;
    ButtonStateColor& GetBackgroundColor();
    ButtonStateColor& GetBorderColor();
    ButtonStateColor& GetTextColor();
    COLORREF GetNormalBackgroundColor() const;
    COLORREF GetFocusedBackgroundColor() const;
    COLORREF GetPushedBackgroundColor() const;
    COLORREF GetNormalBorderColor() const;
    COLORREF GetFocusedBorderColor() const;
    COLORREF GetPushedBorderColor() const;
    COLORREF GetNormalTextColor() const;
    COLORREF GetFocusedTextColor() const;
    COLORREF GetPushedTextColor() const;
    int GetBorderWidth() const;
    CWindowControl* GetParentWindow() const;

    void SetTransparentBackground(bool isTransparentBackground);
    void SetText(std::wstring text);
    void SetParentWindow(CWindowControl* parent);
    void SetBackgroundColor(COLORREF color);
    void SetFocusedBackgroundColor(COLORREF color);
    void SetPushedBackgroundColor(COLORREF color);
    void SetBorderColor(COLORREF color);
    void SetFocusedBorderColor(COLORREF color);
    void SetPushedBorderColor(COLORREF color);
    void SetTextColor(COLORREF color);
    void SetFocusedTextColor(COLORREF color);
    void SetPushedTextColor(COLORREF color);
    void SetBorderWidth(int width);

    bool Create() override;
    void Destroy() override;

    void Show() override;

    static WNDPROC GetOriginalProc();
    static LRESULT CALLBACK OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
    CTextFont _font;
    std::wstring _text = L"";
    bool _isTransparentBackground = false;
    ButtonStateColor _backgroundColor = {RGB(0xE1, 0xE1, 0xE1), RGB(0xE5, 0xF1, 0xFB), RGB(0xCC, 0xE4, 0xF7)};
    ButtonStateColor _borderColor = {RGB(0xAD, 0xAD, 0xAD), RGB(0x00, 0x78, 0xD7), RGB(0x00, 0x54, 0x99)};
    ButtonStateColor _textColor = {RGB(0x00, 0x00, 0x00), RGB(0x00, 0x00, 0x00), RGB(0x00, 0x00, 0x00)};
    int _borderWidth = 1;

    static WNDPROC s_originalProc;
};
}
