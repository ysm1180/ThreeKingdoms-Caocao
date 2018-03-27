#include "LayoutControl.h"

#include "WindowControl.h"
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

int CLayoutControl::GetX() const
{
    return _position.x;
}

int CLayoutControl::GetY() const
{
    return _position.y;
}

int CLayoutControl::GetWidth() const
{
    return _size.cx;
}

int CLayoutControl::GetHeight() const
{
    return _size.cy;
}

void CLayoutControl::SetX(int x, bool isRedraw)
{
    if (_position.x != x)
    {
        if (isRedraw)
        {
            if (!_parents.empty())
            {
                for (auto& parent : _parents)
                {
                    int width = _size.cx;
                    int height = _size.cy;
                    if (width == 0)
                    {
                        width = parent->GetWidth();
                    }
                    if (height == 0)
                    {
                        height = parent->GetHeight();
                    }

                    RECT rect;
                    SetRect(&rect, _position.x, _position.y, _position.x + width * _ratioX, _position.y + height * _ratioY);
                    InvalidateRect(parent->GetHWnd(), &rect, TRUE);
                }
            }

            _position.x = x;

            if (!_parents.empty())
            {
                for (auto& parent : _parents)
                {
                    int width = _size.cx;
                    int height = _size.cy;
                    if (width == 0)
                    {
                        width = parent->GetWidth();
                    }
                    if (height == 0)
                    {
                        height = parent->GetHeight();
                    }

                    RECT rect;
                    SetRect(&rect, _position.x, _position.y, _position.x + width * _ratioX, _position.y + height * _ratioY);
                    InvalidateRect(parent->GetHWnd(), &rect, TRUE);

                    UpdateWindow(parent->GetHWnd());
                }
            }
        }
        else
        {
            _position.x = x;
        }
    }
}

void CLayoutControl::SetY(int y, bool isRedraw)
{
    if (_position.y != y)
    {
        if (isRedraw)
        {


            if (!_parents.empty())
            {
                for (auto& parent : _parents)
                {
                    int width = _size.cx;
                    int height = _size.cy;
                    if (width == 0)
                    {
                        width = parent->GetWidth();
                    }
                    if (height == 0)
                    {
                        height = parent->GetHeight();
                    }

                    RECT rect;
                    SetRect(&rect, _position.x, _position.y, _position.x + width * _ratioX, _position.y + height * _ratioY);
                    InvalidateRect(parent->GetHWnd(), &rect, TRUE);
                }
            }

            _position.y = y;

            if (!_parents.empty())
            {
                for (auto& parent : _parents)
                {
                    int width = _size.cx;
                    int height = _size.cy;
                    if (width == 0)
                    {
                        width = parent->GetWidth();
                    }
                    if (height == 0)
                    {
                        height = parent->GetHeight();
                    }

                    RECT rect;
                    SetRect(&rect, _position.x, _position.y, _position.x + width * _ratioX, _position.y + height * _ratioY);
                    InvalidateRect(parent->GetHWnd(), &rect, TRUE);

                    UpdateWindow(parent->GetHWnd());
                }
            }
        }
        else
        {
            _position.y = y;
        }
    }
}

void CLayoutControl::SetWidth(int cx, bool isRedraw)
{
    if (_size.cx != cx)
    {
        if (isRedraw)
        {
            if (!_parents.empty())
            {
                for (auto& parent : _parents)
                {
                    int width = _size.cx;
                    int height = _size.cy;
                    if (width == 0)
                    {
                        width = parent->GetWidth();
                    }
                    if (height == 0)
                    {
                        height = parent->GetHeight();
                    }

                    RECT rect;
                    SetRect(&rect, _position.x, _position.y, _position.x + width * _ratioX, _position.y + height * _ratioY);
                    InvalidateRect(parent->GetHWnd(), &rect, TRUE);
                }
            }

            _size.cx = cx;

            if (!_parents.empty())
            {
                for (auto& parent : _parents)
                {
                    int width = _size.cx;
                    int height = _size.cy;
                    if (width == 0)
                    {
                        width = parent->GetWidth();
                    }
                    if (height == 0)
                    {
                        height = parent->GetHeight();
                    }

                    RECT rect;
                    SetRect(&rect, _position.x, _position.y, _position.x + width * _ratioX, _position.y + height * _ratioY);
                    InvalidateRect(parent->GetHWnd(), &rect, TRUE);

                    UpdateWindow(parent->GetHWnd());
                }
            }
        }
        else
        {
            _size.cx = cx;
        }
    }
}

void CLayoutControl::SetHeight(int cy, bool isRedraw)
{
    if (isRedraw)
    {
        if (!_parents.empty())
        {
            for (auto& parent : _parents)
            {
                int width = _size.cx;
                int height = _size.cy;
                if (width == 0)
                {
                    width = parent->GetWidth();
                }
                if (height == 0)
                {
                    height = parent->GetHeight();
                }

                RECT rect;
                SetRect(&rect, _position.x, _position.y, _position.x + width * _ratioX, _position.y + height * _ratioY);
                InvalidateRect(parent->GetHWnd(), &rect, TRUE);
            }
        }

        _size.cy = cy;

        if (!_parents.empty())
        {
            for (auto& parent : _parents)
            {
                int width = _size.cx;
                int height = _size.cy;
                if (width == 0)
                {
                    width = parent->GetWidth();
                }
                if (height == 0)
                {
                    height = parent->GetHeight();
                }

                RECT rect;
                SetRect(&rect, _position.x, _position.y, _position.x + width * _ratioX, _position.y + height * _ratioY);
                InvalidateRect(parent->GetHWnd(), &rect, TRUE);

                UpdateWindow(parent->GetHWnd());
            }
        }
    }
    else
    {
        _size.cy = cy;
    }
}

void CLayoutControl::SetRatioX(double ratio, bool isRedraw)
{
    if (isRedraw)
    {
        if (!_parents.empty())
        {
            for (auto& parent : _parents)
            {
                int width = _size.cx;
                int height = _size.cy;
                if (width == 0)
                {
                    width = parent->GetWidth();
                }
                if (height == 0)
                {
                    height = parent->GetHeight();
                }

                RECT rect;
                SetRect(&rect, _position.x, _position.y, _position.x + width * _ratioX, _position.y + height * _ratioY);
                InvalidateRect(parent->GetHWnd(), &rect, TRUE);
            }
        }

        _ratioX = ratio;

        if (!_parents.empty())
        {
            for (auto& parent : _parents)
            {
                int width = _size.cx;
                int height = _size.cy;
                if (width == 0)
                {
                    width = parent->GetWidth();
                }
                if (height == 0)
                {
                    height = parent->GetHeight();
                }

                RECT rect;
                SetRect(&rect, _position.x, _position.y, _position.x + width * _ratioX, _position.y + height * _ratioY);
                InvalidateRect(parent->GetHWnd(), &rect, TRUE);

                UpdateWindow(parent->GetHWnd());
            }
        }
    }
    else
    {
        _ratioX = ratio;
    }
}

void CLayoutControl::SetRatioY(double ratio, bool isRedraw)
{
    if (isRedraw)
    {
        if (!_parents.empty())
        {
            for (auto& parent : _parents)
            {
                int width = _size.cx;
                int height = _size.cy;
                if (width == 0)
                {
                    width = parent->GetWidth();
                }
                if (height == 0)
                {
                    height = parent->GetHeight();
                }

                RECT rect;
                SetRect(&rect, _position.x, _position.y, _position.x + width * _ratioX, _position.y + height * _ratioY);
                InvalidateRect(parent->GetHWnd(), &rect, TRUE);
            }
        }

        _ratioY = ratio;

        if (!_parents.empty())
        {
            for (auto& parent : _parents)
            {
                int width = _size.cx;
                int height = _size.cy;
                if (width == 0)
                {
                    width = parent->GetWidth();
                }
                if (height == 0)
                {
                    height = parent->GetHeight();
                }

                RECT rect;
                SetRect(&rect, _position.x, _position.y, _position.x + width * _ratioX, _position.y + height * _ratioY);
                InvalidateRect(parent->GetHWnd(), &rect, TRUE);

                UpdateWindow(parent->GetHWnd());
            }
        }
    }
    else
    {
        _ratioY = ratio;
    }
}

void CLayoutControl::AddParentWindow(CWindowControl* parent)
{
    _parents.push_back(parent);
}

int CLayoutControl::AddImage(CImageControl *image, int x, int y, bool isUpdate)
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

    if (!_parents.empty())
    {
        for (auto& parent : _parents)
        {
            int imageX = (x * _ratioX) + _position.x;
            int imageY = (y * _ratioY) + _position.y;

            RECT rect;
            SetRect(&rect, imageX, imageY, imageX + image->GetWidth() * _ratioX, imageY + image->GetHeight() * _ratioY);
            InvalidateRect(parent->GetHWnd(), &rect, TRUE);

            if (isUpdate)
            {
                UpdateWindow(parent->GetHWnd());
            }
        }
    }

    return index;
}

void CLayoutControl::DeleteImage(CImageControl * image, bool isUpdate)
{
    auto iter = std::begin(_images);

    while (iter != std::end(_images))
    {
        if (iter->image == image)
        {
            auto position = iter->position;

            SelectObject(iter->imageDC, iter->oldBitmap);
            DeleteDC(iter->imageDC);
            _reusingImageIndex.push(iter->index);
            iter = _images.erase(iter);

            if (!_parents.empty())
            {
                for (auto& parent : _parents)
                {
                    int imageX = (position.x * _ratioX) + _position.x;
                    int imageY = (position.y * _ratioY) + _position.y;
                    RECT rect;

                    SetRect(&rect, imageX, imageY, imageX + image->GetWidth() * _ratioX, imageY + image->GetHeight() * _ratioY);
                    InvalidateRect(parent->GetHWnd(), &rect, TRUE);

                    if (isUpdate)
                    {
                        UpdateWindow(parent->GetHWnd());
                    }
                }
            }

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
        int imageX = (image.position.x * _ratioX) + _position.x;
        int imageY = (image.position.y * +_ratioY) + _position.y;
        int imageWidth = image.image->GetWidth() * _ratioX;
        int imageHeight = image.image->GetHeight() * _ratioY;

        if (_ratioX == 1.0 && _ratioY == 1.0)
        {
            if (imageX + imageWidth > _size.cx)
            {
                imageWidth = _size.cx - imageX;
            }
            if (imageY + imageHeight > _size.cy)
            {
                imageHeight = _size.cy - imageY;
            }

            BitBlt(destDC, imageX, imageY, imageWidth, imageHeight, image.imageDC, 0, 0, SRCCOPY);
        }
        else
        {
            auto memDc = CreateCompatibleDC(destDC);
            auto memBitmap = CreateCompatibleBitmap(destDC, imageWidth, imageHeight);
            auto oldBitmap = SelectBitmap(memDc, memBitmap);

            SetStretchBltMode(memDc, COLORONCOLOR);
            StretchBlt(memDc, 0, 0, imageWidth, imageHeight, image.imageDC, 0, 0, image.image->GetWidth(), image.image->GetHeight(), SRCCOPY);

            if (imageX + imageWidth > _size.cx)
            {
                imageWidth = _size.cx - imageX;
            }
            if (imageY + imageHeight > _size.cy)
            {
                imageHeight = _size.cy - imageY;
            }

            BitBlt(destDC, imageX, imageY, imageWidth, imageHeight, memDc, 0, 0, SRCCOPY);

            SelectBitmap(memDc, oldBitmap);
            DeleteBitmap(memBitmap);
            DeleteDC(memDc);
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