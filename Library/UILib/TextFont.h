#pragma once

#include "LuaLib/LuaTinker.h"

#include <Windows.h>
#include <string>

namespace jojogame {
class CBaseControl;

class  CTextFont
{
public:
    static void RegisterFunctions(lua_State *L);

    CTextFont();
    CTextFont(CBaseControl *control);
    virtual ~CTextFont();

    HFONT GetHFont() const;

    bool IsBold() const;
    bool IsItalic() const;
    bool IsUnderline() const;
    int GetFontSize() const;
    std::wstring GetFontName() const;

    void SetBold(bool isBold);
    void SetItalic(bool isItalic);
    void SetUnderline(bool isUnderline);
    void SetFontSize(int fontSize);
    void SetFontName(std::wstring fontName);

    void ResetFont();

protected:
    CBaseControl * _control = nullptr;

    HFONT _font = nullptr;
    bool _isBold = false;
    bool _isItalic = false;
    bool _isUnderline = false;
    int _fontSize = 10;
    std::wstring _fontName;
};
}
