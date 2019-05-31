#pragma once

#include "LuaLib\LuaTinker.h"

#include <windows.h>
#include <queue>
#include <vector>

namespace jojogame
{
class CWindowControl;
class CImageControl;
class CGraphicText;

struct ImageInformation
{
    int index;
    HDC imageDC;
    HDC mirrorDC;
    HBITMAP oldBitmap;
    HBITMAP oldMirrorBitmap;
    CImageControl *image;
    POINT position;
    bool isHide;
    bool isRefresh;
};

struct TextInformation
{
    int index;
    CGraphicText *text;
    POINT position;
    bool isHide;
    bool isRefresh;
};

class CLayoutControl
{
public:
    static void RegisterFunctions(lua_State *L);

    CLayoutControl();
    virtual ~CLayoutControl();

    int GetX() const;
    int GetY() const;
    int GetWidth() const;
    int GetHeight() const;
    bool IsHide() const;

    void SetX(int x, bool isRedraw = false);
    void SetY(int y, bool isRedraw = false);
    void SetWidth(int cx, bool isRedraw = false);
    void SetHeight(int cy, bool isRedraw = false);
    void SetRatioX(double ratio, bool isRedraw = false);
    void SetRatioY(double ratio, bool isRedraw = false);
    void SetHide(bool value);

    void AddParentWindow(CWindowControl *parent);
    void RemoveParentWIndow(CWindowControl *parent);

    int AddImage(CImageControl *image, int x, int y, bool isShow);
    void DeleteImage(int index, bool isUpdate);
    void MoveImage(int index, int x, int y, bool isUpdate);
    void HideImage(int index, bool isUpdate);
    void ShowImage(int index, bool isUpdate);

    int AddText(CGraphicText *text, int x, int y, bool isShow);
    void DeleteText(int index, bool isUpdate);
    void MoveText(int index, int x, int y, bool isUpdate);
    void HideText(int index, bool isUpdate);
    void ShowText(int index, bool isUpdate);

    void Draw(HDC destDC);
    void Draw(HDC destDC, RECT &rect);
    void Draw(HDC destDC, RECT &rect, COLORREF mixedColor);
    void Erase();

    void Refresh();

private:
    int _GetNewImageIndex();
    int _GetNewTextIndex();

    HDC _dc;
    std::vector<CWindowControl *> _parents;
    std::vector<ImageInformation> _images;
    std::vector<TextInformation> _texts;

    std::queue<int> _reusingImageIndex;
    std::queue<int> _reusingTextIndex;

    std::vector<RECT> _refreshRect;

    SIZE _size{};
    POINT _position{};
    double _ratioX = 1.0;
    double _ratioY = 1.0;
    bool _isHide = false;
};
} // namespace jojogame
