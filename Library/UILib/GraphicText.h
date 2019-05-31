#pragma once
#include "TextFont.h"

namespace jojogame
{
class CGraphicText
{
public:
    static void RegisterFunctions(lua_State *L);

    CGraphicText();
    virtual ~CGraphicText();

    CTextFont *GetFont();
    std::wstring GetText();
    COLORREF GetTextColor();
    int GetWidth(HDC hdc);
    int GetHeight(HDC hdc);

    void SetText(std::wstring text);
    void SetTextColor(COLORREF color);

    void Draw(HDC hdc, POINT position);

private:
    std::wstring _text = L"";
    COLORREF _textColor = RGB(0, 0, 0);
    CTextFont _font;
};
} // namespace jojogame
