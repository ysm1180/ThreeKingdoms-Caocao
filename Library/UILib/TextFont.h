#pragma once

#include <Windows.h>
#include <string>

namespace jojogame {
class CBaseControl;

class CTextFont
{
public:
    CTextFont(CBaseControl *control);
    virtual ~CTextFont();

    bool IsBold();
    bool IsItalic();
    bool IsUnderline();
    int GetFontSize();
    std::wstring GetFontName();

    void SetBold(bool isBold);
    void SetItalic(bool isItalic);
    void SetUnderline(bool isUnderline);
    void SetFontSize(int fontSize);
    void SetFontName(std::wstring fontName);

protected:
    void _ResetFont();

    CBaseControl * _control = nullptr;

    HFONT _font = nullptr;
    bool _isBold = false;
    bool _isItalic = false;
    bool _isUnderline = false;
    int _fontSize = 12;
    std::wstring _fontName = LR"(±¼¸²)";
};
}
