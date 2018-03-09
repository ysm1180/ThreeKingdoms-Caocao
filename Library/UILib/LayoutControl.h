#pragma once

#include "LuaLib\LuaTinker.h"

#include <atlcoll.h>
#include <CommCtrl.h>
#include <windows.h>
#include <queue>
#include <utility>
#include <vector>

namespace jojogame {
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

    int GetX();
    int GetY();
    int GetWidth();
    int GetHeight();

    void SetX(int x);
    void SetY(int y);
    void SetWidth(int width);
    void SetHeight(int height);
    void SetRatioX(double ratio);
    void SetRatioY(double ratio);

    int AddImage(CImageControl *image, int x, int y);
    void DeleteImage(CImageControl *image);
    void DeleteImageByImageIndex(int imageIndex);

    void Draw(HDC destDC);
    void Erase();

private:
    int _GetNewIndex();

    HDC _dc;
    std::vector<ImageInformation> _images;
    std::queue<int> _reusingImageIndex;
    SIZE _size;
    POINT _position;
    double _ratioX = 1.0;
    double _ratioY = 1.0;
};
}
