#pragma once

#include "LuaLib\LuaTinker.h"

#include <Windows.h>

namespace jojogame
{
class CImageControl
{
public:
    static void RegisterFunctions(lua_State *L);

    CImageControl();
    ~CImageControl();

    void ReadJpeg(BYTE *src, int size, COLORREF maskColor, double brightness, bool mirror);

    void ReadPng(BYTE *src, int size, COLORREF maskColor, double brightness, bool mirror);

    int GetClipingTop();
    int GetClipingLeft();
    int GetClipingWidth();
    int GetClipingHeight();
    int GetWidth();
    int GetHeight();
    HBITMAP GetImageHandle();
    HBITMAP GetMirrorImageHandle();
    HBITMAP GetMaskImageHandle();
    HBITMAP GetMaskMirrorImageHandle();
    BITMAPINFO GetBitmapInfo();
    COLORREF GetMaskColor();
    bool IsDisplayMirror();

    void SetDisplayMirror(bool value);

    void SetClipingRect(int left, int top, int right, int bottom);
    void ResetClipingRect();

    void LoadImageFromMe5FileByIndex(std::wstring filePath, int groupIndex, int subIndex, COLORREF maskColor,
                                     double brightness = 1, bool mirror = false);

private:
    SIZE _size;
    HBITMAP _image = nullptr;
    HBITMAP _maskImage = nullptr;
    HBITMAP _mirrorImage = nullptr;
    HBITMAP _maskMirrorImage = nullptr;

    BITMAPINFO _info;

    RECT _clipingRect;
    COLORREF _maskColor;

    bool _isDisplayMirror = false;
};
}; // namespace jojogame
