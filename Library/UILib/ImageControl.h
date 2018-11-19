#pragma once

#include "LuaLib\LuaTinker.h"

#include <Windows.h>

namespace jojogame {
class CImageControl
{
public:
    static void RegisterFunctions(lua_State* L);

    CImageControl();
    ~CImageControl();

    void ReadJpeg(BYTE * src, int size, COLORREF maskColor, double brightness);

    void ReadPng(BYTE * src, int size, COLORREF maskColor, double brightness);

    int GetClipingTop();
    int GetClipingLeft();
    int GetClipingWidth();
    int GetClipingHeight();
    int GetWidth();
    int GetHeight();
    HBITMAP GetImageHandle();
    HBITMAP GetMaskImageHandle();
    BITMAPINFO GetBitmapInfo();
    COLORREF GetMaskColor();

    void SetClipingRect(int left, int top, int right, int bottom);
    void ResetClipingRect();

    void LoadImageFromMe5FileByIndex(std::wstring filePath, int groupIndex, int subIndex, COLORREF maskColor,
                                     double brightness = 1);

private:
    SIZE _size;
    HBITMAP _image = nullptr;
    HBITMAP _maskImage = nullptr;

    BITMAPINFO _info;

    RECT _clipingRect;
    COLORREF _maskColor;
};
};
