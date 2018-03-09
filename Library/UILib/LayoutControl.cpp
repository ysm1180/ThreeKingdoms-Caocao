#include "LayoutControl.h"

#include "ImageControl.h"


namespace jojogame {
void CLayoutControl::RegisterFunctions(lua_State * L)
{
    LUA_BEGIN(CLayoutControl, "_Layout");


    LUA_METHOD(SetX);
    LUA_METHOD(SetY);
    LUA_METHOD(SetWidth);
    LUA_METHOD(SetHeight);
    LUA_METHOD(SetRatioX);
    LUA_METHOD(SetRatioY);

    LUA_METHOD(AddImage);
    LUA_METHOD(DeleteImage);
    LUA_METHOD(DeleteImageByImageIndex);
}

CLayoutControl::CLayoutControl()
{
    _dc = CreateCompatibleDC(nullptr);
    _size.cx = 0;
    _size.cy = 0;
    _position.x = 0;
    _position.y = 0;
}

CLayoutControl::~CLayoutControl()
{
    for (auto imageInfo : _images)
    {
        SelectObject(imageInfo.imageDC, imageInfo.oldBitmap);
        DeleteDC(imageInfo.imageDC);
    }
}

int CLayoutControl::GetX()
{
    return _position.x;
}

int CLayoutControl::GetY()
{
    return _position.y;
}

int CLayoutControl::GetWidth()
{
    return _size.cx;
}

int CLayoutControl::GetHeight()
{
    return _size.cy;
}

void CLayoutControl::SetX(int x)
{
    _position.x = x;
}

void CLayoutControl::SetY(int y)
{
    _position.y = y;
}

void CLayoutControl::SetWidth(int width)
{
    _size.cx = width;
}

void CLayoutControl::SetHeight(int height)
{
    _size.cy = height;
}

void CLayoutControl::SetRatioX(double ratio)
{
    _ratioX = ratio;
}

void CLayoutControl::SetRatioY(double ratio)
{
    _ratioY = ratio;
}


int CLayoutControl::AddImage(CImageControl * image, int x, int y)
{
    HDC imageDC = CreateCompatibleDC(_dc);
    ImageInformation imageInfo;
    int index = _GetNewIndex();

    imageInfo.oldBitmap = (HBITMAP)SelectObject(imageDC, image->GetImageHandle());
    imageInfo.imageDC = imageDC;
    imageInfo.index = index;
    imageInfo.image = image;
    imageInfo.position.x = x;
    imageInfo.position.y = y;

    _images.push_back(imageInfo);

    return index;
}

void CLayoutControl::DeleteImage(CImageControl * image)
{
    auto iter = std::begin(_images);

    while (iter != std::end(_images))
    {
        if (iter->image == image)
        {
            SelectObject(iter->imageDC, iter->oldBitmap);
            DeleteDC(iter->imageDC);
            _reusingImageIndex.push(iter->index);
            iter = _images.erase(iter);
            break;
        }
        else
        {
            ++iter;
        }
    }
}

void CLayoutControl::DeleteImageByImageIndex(int imageIndex)
{
    auto iter = std::begin(_images);

    while (iter != std::end(_images))
    {
        if (iter->index == imageIndex)
        {
            SelectObject(iter->imageDC, iter->oldBitmap);
            DeleteDC(iter->imageDC);
            _reusingImageIndex.push(iter->index);
            iter = _images.erase(iter);
            break;
        }
        else
        {
            ++iter;
        }
    }
}

void CLayoutControl::Draw(HDC destDC)
{
    for (ImageInformation image : _images)
    {
        int imageX = image.position.x + _position.x;
        int imageY = image.position.y + _position.y;
        int imageWidth = image.image->GetWidth();
        int imageHeight = image.image->GetHeight();

        if (imageX + imageWidth > _size.cx)
        {
            imageWidth = _size.cx - imageX;
        }
        if (imageY + imageHeight > _size.cy)
        {
            imageHeight = _size.cy - imageY;
        }

        if (_ratioX == 1.0 && _ratioY == 1.0)
        {
            BitBlt(destDC, imageX, imageY, imageWidth, imageHeight, image.imageDC, 0, 0, SRCCOPY);
        }
        else
        {
            imageX *= _ratioX;
            imageY *= _ratioY;
            imageWidth *= _ratioX;
            imageHeight *= _ratioY;

            SetStretchBltMode(destDC, COLORONCOLOR);
            StretchBlt(destDC, imageX, imageY, imageWidth, imageHeight, image.imageDC, 0, 0, image.image->GetWidth(), image.image->GetHeight(), SRCCOPY);
        }
    }
}

void CLayoutControl::Erase()
{

}

int CLayoutControl::_GetNewIndex()
{
    int index;

    if (_reusingImageIndex.empty())
    {
        index = _images.size();
    }
    else
    {
        index = _reusingImageIndex.front();
        _reusingImageIndex.pop();
    }

    return index;
}
}