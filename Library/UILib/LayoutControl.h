#pragma once

#include "LuaLib\LuaTinker.h"

#include <windows.h>
#include <queue>
#include <vector>

namespace jojogame {
class CWindowControl;
class CImageControl;

struct ImageInformation
{
    int index;
    HDC imageDC;
    HBITMAP oldBitmap;
    CImageControl *image;
    POINT position;
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

    void SetX(int x, bool isRedraw = false);
    void SetY(int y, bool isRedraw = false);
    void SetWidth(int cx, bool isRedraw = false);
    void SetHeight(int cy, bool isRedraw = false);
    void SetRatioX(double ratio, bool isRedraw = false);
    void SetRatioY(double ratio, bool isRedraw = false);

    void AddParentWindow(CWindowControl *parent);

    int AddImage(CImageControl *image, int x, int y, bool isUpdate);
    void DeleteImage(CImageControl *image, bool isUpdate);

    void Draw(HDC destDC);
    void Draw(HDC destDC, RECT& rect);
    void Erase();

private:
    int _GetNewIndex();

    HDC _dc;
    std::vector<CWindowControl *> _parents;
    std::vector<ImageInformation> _images;
    std::queue<int> _reusingImageIndex;
    SIZE _size;
    POINT _position;
    double _ratioX = 1.0;
    double _ratioY = 1.0;
};
}
