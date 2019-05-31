#include "GraphicText.h"
#include <windowsx.h>

namespace jojogame
{
void CGraphicText::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN(CGraphicText, "_GraphicText");

    LUA_METHOD(GetFont);
    LUA_METHOD(GetText);
    LUA_METHOD(GetTextColor);

    LUA_METHOD(SetText);
    LUA_METHOD(SetTextColor);
}

CGraphicText::CGraphicText()
{
}

CGraphicText::~CGraphicText()
{
}

CTextFont *CGraphicText::GetFont()
{
    return &_font;
}

std::wstring CGraphicText::GetText()
{
    return _text;
}

COLORREF CGraphicText::GetTextColor()
{
    return _textColor;
}

int CGraphicText::GetWidth(HDC hdc)
{
    auto originalFont = SelectFont(hdc, _font.GetHFont());

    RECT rect{0, 0, 0, 0};
    DrawText(hdc, _text.c_str(), _text.length(), &rect, DT_CALCRECT);
    SelectFont(hdc, originalFont);

    return rect.right;
}

int CGraphicText::GetHeight(HDC hdc)
{
    auto originalFont = SelectFont(hdc, _font.GetHFont());

    RECT rect{0, 0, 0, 0};
    DrawText(hdc, _text.c_str(), _text.length(), &rect, DT_CALCRECT);
    SelectFont(hdc, originalFont);

    return rect.bottom;
}

void CGraphicText::SetText(std::wstring text)
{
    _text = text;
}

void CGraphicText::SetTextColor(COLORREF color)
{
    _textColor = color;
}

void CGraphicText::Draw(HDC hdc, POINT position)
{
    auto originalFont = SelectFont(hdc, _font.GetHFont());
    auto originalTextColor = ::SetTextColor(hdc, _textColor);
    SetBkMode(hdc, TRANSPARENT);

    RECT rect{0, 0, 0, 0};
    DrawText(hdc, _text.c_str(), _text.length(), &rect, DT_CALCRECT);
    SetRect(&rect, position.x, position.y, position.x + rect.right, position.y + rect.bottom);
    DrawText(hdc, _text.c_str(), _text.length(), &rect, DT_NOCLIP);

    ::SetTextColor(hdc, originalTextColor);
    SelectFont(hdc, originalFont);
}

} // namespace jojogame
