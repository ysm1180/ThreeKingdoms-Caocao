#include "TextFont.h"
#include "BaseControl.h"

namespace jojogame {
CTextFont::CTextFont(CBaseControl *control)
{
    _control = control;
}

CTextFont::~CTextFont()
{
}

bool CTextFont::IsBold()
{
    return _isBold;
}

bool CTextFont::IsItalic()
{
    return _isItalic;
}

bool CTextFont::IsUnderline()
{
    return _isUnderline;
}

int CTextFont::GetFontSize()
{
    return _fontSize;
}

std::wstring CTextFont::GetFontName()
{
    return _fontName;
}

void CTextFont::SetBold(bool isBold)
{
    _isBold = isBold;
    _ResetFont();
}

void CTextFont::SetItalic(bool isItalic)
{
    _isItalic = isItalic;
    _ResetFont();
}

void CTextFont::SetUnderline(bool isUnderline)
{
    _isUnderline = isUnderline;
    _ResetFont();
}

void CTextFont::SetFontSize(int fontSize)
{
    _fontSize = fontSize;
    _ResetFont();
}

void CTextFont::SetFontName(std::wstring fontName)
{
    _fontName = fontName;
    _ResetFont();
}

void CTextFont::_ResetFont()
{
    if (_font != nullptr)
    {
        DeleteFont(_font);
    }

    _font = CreateFontW(_fontSize,
                        0,
                        0,
                        0,
                        FW_NORMAL,
                        _isItalic,
                        _isUnderline,
                        _isBold,
                        HANGEUL_CHARSET,
                        0,
                        0,
                        0,
                        VARIABLE_PITCH | FF_ROMAN,
                        _fontName.c_str());

    HWND controlHWnd = _control->GetHWnd();
    if (controlHWnd != nullptr)
    {
        SendMessage(controlHWnd, WM_SETFONT, (WPARAM) _font, (LPARAM) TRUE);

        RECT rect;
        SetRect(&rect, _control->GetX(), _control->GetY(), _control->GetX() + _control->GetWidth(),
                _control->GetY() + _control->GetHeight());
        InvalidateRect(controlHWnd, &rect, true);
    }
}
}