#pragma once

#include "LuaLib\LuaTinker.h"
#include "CommonLib/lodepng.h"

#include <Windows.h>

namespace jojogame {
class CImageControl
{
public:
    static void RegisterFunctions(lua_State* L);

    CImageControl();
    ~CImageControl();

    int GetWidth();
    int GetHeight();
    HBITMAP GetImageHandle();
    HBITMAP GetMaskImageHandle();

    void LoadImageFromMe5FileByIndex(std::wstring filePath, int groupIndex, int subIndex, COLORREF maskColor);

private:
    SIZE _size;
    HBITMAP _image = nullptr;
    HBITMAP _maskImage = nullptr;
};
};
