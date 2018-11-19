#include "LayoutControl.h"

#include "WindowControl.h"
#include "ImageControl.h"
#include "BaseLib/Color.h"
#include "CommonLib/GameManager.h"
#include "ControlManager.h"

namespace jojogame {
void CLayoutControl::RegisterFunctions(lua_State* L)
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
    LUA_METHOD(MoveImage);
    LUA_METHOD(HideImage);
    LUA_METHOD(ShowImage);

    LUA_METHOD(Refresh);
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
        SelectBitmap(imageInfo.imageDC, imageInfo.oldBitmap);
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
                    SetRect(&rect, _position.x, _position.y, _position.x + width * _ratioX,
                            _position.y + height * _ratioY);
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
                    SetRect(&rect, _position.x, _position.y, _position.x + width * _ratioX,
                            _position.y + height * _ratioY);
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
                    SetRect(&rect, _position.x, _position.y, _position.x + width * _ratioX,
                            _position.y + height * _ratioY);
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
                    SetRect(&rect, _position.x, _position.y, _position.x + width * _ratioX,
                            _position.y + height * _ratioY);
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
                    SetRect(&rect, _position.x, _position.y, _position.x + width * _ratioX,
                            _position.y + height * _ratioY);
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
                    SetRect(&rect, _position.x, _position.y, _position.x + width * _ratioX,
                            _position.y + height * _ratioY);
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
                InvalidateRect(parent->GetHWnd(), &rect, FALSE);
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
                InvalidateRect(parent->GetHWnd(), &rect, FALSE);

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
                InvalidateRect(parent->GetHWnd(), &rect, FALSE);
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
                InvalidateRect(parent->GetHWnd(), &rect, FALSE);

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

void CLayoutControl::RemoveParentWIndow(CWindowControl* parent)
{
    _parents.erase(std::remove(_parents.begin(), _parents.end(), parent), _parents.end());
}

int CLayoutControl::AddImage(CImageControl* image, int x, int y, bool isUpdate)
{
    HDC imageDC = CreateCompatibleDC(_dc);
    ImageInformation imageInfo;
    int index = _GetNewIndex();

    imageInfo.oldBitmap = SelectBitmap(imageDC, image->GetImageHandle());
    imageInfo.imageDC = imageDC;
    imageInfo.index = index;
    imageInfo.image = image;
    imageInfo.position.x = x;
    imageInfo.position.y = y;
    imageInfo.isHide = false;

    _images.push_back(imageInfo);

    if (!_parents.empty())
    {
        for (auto& parent : _parents)
        {
            int imageX = (x * _ratioX) + _position.x;
            int imageY = (y * _ratioY) + _position.y;

            RECT rect;
            SetRect(&rect, imageX, imageY, imageX + image->GetClipingWidth() * _ratioX,
                    imageY + image->GetClipingHeight() * _ratioY);
            InvalidateRect(parent->GetHWnd(), &rect, FALSE);

            if (isUpdate)
            {
                UpdateWindow(parent->GetHWnd());
            }
        }
    }

    return index;
}

void CLayoutControl::DeleteImage(int index, bool isUpdate)
{
    auto iter = std::begin(_images);

    while (iter != std::end(_images))
    {
        if (iter->index == index)
        {
            auto position = iter->position;
            auto image = iter->image;

            SelectBitmap(iter->imageDC, iter->oldBitmap);
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

                    SetRect(&rect, imageX, imageY, imageX + image->GetWidth() * _ratioX,
                            imageY + image->GetHeight() * _ratioY);
                    InvalidateRect(parent->GetHWnd(), &rect, FALSE);

                    if (isUpdate)
                    {
                        UpdateWindow(parent->GetHWnd());
                    }
                }
            }

            break;
        }
        ++iter;
    }
}

void CLayoutControl::MoveImage(int index, int x, int y, bool isUpdate)
{
    auto iter = std::begin(_images);

    while (iter != std::end(_images))
    {
        if (iter->index == index)
        {
            auto position = iter->position;
            auto image = iter->image;

            iter->position.x = x;
            iter->position.y = y;

            if (!iter->isHide)
            {
                if (!_parents.empty())
                {
                    for (auto& parent : _parents)
                    {
                        int originalImageX = (position.x * _ratioX) + _position.x;
                        int originalImageY = (position.y * _ratioY) + _position.y;
                        int imageX = (iter->position.x * _ratioX) + _position.x;
                        int imageY = (iter->position.y * _ratioY) + _position.y;
                        RECT rect;

                        SetRect(&rect, originalImageX, originalImageY, originalImageX + image->GetClipingWidth() * _ratioX,
                                originalImageY + image->GetClipingHeight() * _ratioY);
                        InvalidateRect(parent->GetHWnd(), &rect, FALSE);
                        SetRect(&rect, imageX, imageY, imageX + image->GetClipingWidth() * _ratioX,
                                imageY + image->GetClipingHeight() * _ratioY);
                        InvalidateRect(parent->GetHWnd(), &rect, FALSE);

                        if (isUpdate)
                        {
                            UpdateWindow(parent->GetHWnd());
                        }
                    }
                }
            }

            break;
        }
        ++iter;
    }
}

void CLayoutControl::HideImage(int index, bool isUpdate)
{
    auto iter = std::begin(_images);

    while (iter != std::end(_images))
    {
        if (iter->index == index)
        {
            auto position = iter->position;
            auto image = iter->image;

            iter->isHide = true;

            if (!_parents.empty())
            {
                for (auto& parent : _parents)
                {
                    int originalImageX = (position.x * _ratioX) + _position.x;
                    int originalImageY = (position.y * _ratioY) + _position.y;
                    int imageX = (iter->position.x * _ratioX) + _position.x;
                    int imageY = (iter->position.y * _ratioY) + _position.y;
                    RECT rect;

                    SetRect(&rect, originalImageX, originalImageY, originalImageX + image->GetClipingWidth() * _ratioX,
                            originalImageY + image->GetClipingHeight() * _ratioY);
                    InvalidateRect(parent->GetHWnd(), &rect, FALSE);
                    SetRect(&rect, imageX, imageY, imageX + image->GetClipingWidth() * _ratioX,
                            imageY + image->GetClipingHeight() * _ratioY);
                    InvalidateRect(parent->GetHWnd(), &rect, FALSE);

                    if (isUpdate)
                    {
                        UpdateWindow(parent->GetHWnd());
                    }
                }
            }

            break;
        }
        ++iter;
    }
}

void CLayoutControl::ShowImage(int index, bool isUpdate)
{
    auto iter = std::begin(_images);

    while (iter != std::end(_images))
    {
        if (iter->index == index)
        {
            auto position = iter->position;
            auto image = iter->image;

            iter->isHide = false;

            if (!_parents.empty())
            {
                for (auto& parent : _parents)
                {
                    int originalImageX = (position.x * _ratioX) + _position.x;
                    int originalImageY = (position.y * _ratioY) + _position.y;
                    int imageX = (iter->position.x * _ratioX) + _position.x;
                    int imageY = (iter->position.y * _ratioY) + _position.y;
                    RECT rect;

                    SetRect(&rect, originalImageX, originalImageY, originalImageX + image->GetClipingWidth() * _ratioX,
                            originalImageY + image->GetClipingHeight() * _ratioY);
                    InvalidateRect(parent->GetHWnd(), &rect, FALSE);
                    SetRect(&rect, imageX, imageY, imageX + image->GetClipingWidth() * _ratioX,
                            imageY + image->GetClipingHeight() * _ratioY);
                    InvalidateRect(parent->GetHWnd(), &rect, FALSE);

                    if (isUpdate)
                    {
                        UpdateWindow(parent->GetHWnd());
                    }
                }
            }

            break;
        }
        ++iter;
    }
}

void CLayoutControl::Draw(HDC destDC)
{
    for (ImageInformation image : _images)
    {
        if (!image.isHide)
        {
            int imageX = (image.position.x * _ratioX) + _position.x;
            int imageY = (image.position.y * +_ratioY) + _position.y;
            int imageWidth = image.image->GetClipingWidth() * _ratioX;
            int imageHeight = image.image->GetClipingHeight() * _ratioY;

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

                auto maskDC = CreateCompatibleDC(destDC);
                auto oldMask = SelectBitmap(maskDC, image.image->GetMaskImageHandle());
                auto oldColor = SetBkColor(image.imageDC, image.image->GetMaskColor());

                BitBlt(destDC, imageX, imageY, imageWidth, imageHeight, image.imageDC, image.image->GetClipingLeft(),
                       image.image->GetClipingTop(), SRCINVERT);
                BitBlt(destDC, imageX, imageY, imageWidth, imageHeight, maskDC, image.image->GetClipingLeft(),
                       image.image->GetClipingTop(), SRCAND);
                BitBlt(destDC, imageX, imageY, imageWidth, imageHeight, image.imageDC, image.image->GetClipingLeft(),
                       image.image->GetClipingTop(), SRCINVERT);

                SetBkColor(image.imageDC, oldColor);
                SelectBitmap(maskDC, oldMask);
                DeleteDC(maskDC);
            }
            else
            {
                int originalImageWidth = imageWidth;
                int originalImageHeight = imageHeight;

                auto memDC = CreateCompatibleDC(destDC);
                auto memBitmap = CreateCompatibleBitmap(destDC, originalImageWidth, originalImageHeight);
                auto oldBitmap = SelectBitmap(memDC, memBitmap);

                auto maskBitmap = CreateBitmap(imageWidth, imageHeight, 1, 1, nullptr);
                auto maskDC = CreateCompatibleDC(destDC);
                auto oldMask = SelectBitmap(maskDC, maskBitmap);

                SetStretchBltMode(memDC, COLORONCOLOR);
                StretchBlt(memDC, 0, 0, imageWidth, imageHeight, image.imageDC, image.image->GetClipingLeft(),
                           image.image->GetClipingTop(), image.image->GetClipingWidth(),
                           image.image->GetClipingHeight(), SRCCOPY);

                auto oldColor = SetBkColor(memDC, image.image->GetMaskColor());
                BitBlt(maskDC, 0, 0, originalImageWidth, originalImageHeight, memDC, 0, 0, SRCCOPY);

                if (imageX + imageWidth > _size.cx)
                {
                    imageWidth = _size.cx - imageX;
                }
                if (imageY + imageHeight > _size.cy)
                {
                    imageHeight = _size.cy - imageY;
                }

                BitBlt(destDC, imageX, imageY, imageWidth, imageHeight, memDC, 0, 0, SRCINVERT);
                BitBlt(destDC, imageX, imageY, imageWidth, imageHeight, maskDC, 0, 0, SRCAND);
                BitBlt(destDC, imageX, imageY, imageWidth, imageHeight, memDC, 0, 0, SRCINVERT);

                SetBkColor(memDC, oldColor);
                SelectBitmap(memDC, oldBitmap);
                DeleteBitmap(memBitmap);
                SelectBitmap(maskDC, oldMask);
                DeleteDC(memDC);
                DeleteDC(maskDC);
            }
        }
    }
}

void CLayoutControl::Draw(HDC destDC, RECT& clipingRect)
{
    for (ImageInformation image : _images)
    {
        if (!image.isHide)
        {
            RECT realClipingRect;
            RECT layoutRect;
            SetRect(&layoutRect, _position.x, _position.y, _position.x + _size.cx, _position.y + _size.cy);
            if (!IntersectRect(&realClipingRect, &layoutRect, &clipingRect))
            {
                continue;
            }

            int imageX = (image.position.x * _ratioX) + _position.x;
            int imageY = (image.position.y * +_ratioY) + _position.y;
            int imageWidth = image.image->GetClipingWidth() * _ratioX;
            int imageHeight = image.image->GetClipingHeight() * _ratioY;

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

                RECT imageRect;
                RECT realDrawRect;
                SetRect(&imageRect, imageX, imageY, imageX + imageWidth, imageY + imageHeight);
                if (!IntersectRect(&realDrawRect, &imageRect, &realClipingRect))
                {
                    continue;
                }

                auto maskDC = CreateCompatibleDC(destDC);
                auto oldMask = SelectBitmap(maskDC, image.image->GetMaskImageHandle());
                auto oldColor = SetBkColor(image.imageDC, image.image->GetMaskColor());

                BitBlt(destDC, realDrawRect.left, realDrawRect.top, realDrawRect.right - realDrawRect.left,
                       realDrawRect.bottom - realDrawRect.top, image.imageDC,
                       realDrawRect.left - imageRect.left + image.image->GetClipingLeft(),
                       realDrawRect.top - imageRect.top + image.image->GetClipingTop(), SRCINVERT);
                BitBlt(destDC, realDrawRect.left, realDrawRect.top, realDrawRect.right - realDrawRect.left,
                       realDrawRect.bottom - realDrawRect.top, maskDC,
                       realDrawRect.left - imageRect.left + image.image->GetClipingLeft(),
                       realDrawRect.top - imageRect.top + image.image->GetClipingTop(), SRCAND);
                BitBlt(destDC, realDrawRect.left, realDrawRect.top, realDrawRect.right - realDrawRect.left,
                       realDrawRect.bottom - realDrawRect.top, image.imageDC,
                       realDrawRect.left - imageRect.left + image.image->GetClipingLeft(),
                       realDrawRect.top - imageRect.top + image.image->GetClipingTop(), SRCINVERT);

                SetBkColor(image.imageDC, oldColor);
                SelectBitmap(maskDC, oldMask);
                DeleteDC(maskDC);
            }
            else
            {
                int originalImageWidth = imageWidth;
                int originalImageHeight = imageHeight;

                if (imageX + imageWidth > _size.cx)
                {
                    imageWidth = _size.cx - imageX;
                }
                if (imageY + imageHeight > _size.cy)
                {
                    imageHeight = _size.cy - imageY;
                }

                RECT imageRect;
                RECT realDrawRect;
                SetRect(&imageRect, imageX, imageY, imageX + imageWidth, imageY + imageHeight);
                if (!IntersectRect(&realDrawRect, &imageRect, &realClipingRect))
                {
                    continue;
                }

                auto memDC = CreateCompatibleDC(destDC);
                auto memBitmap = CreateCompatibleBitmap(destDC, originalImageWidth, originalImageHeight);
                auto oldBitmap = SelectBitmap(memDC, memBitmap);

                auto maskBitmap = CreateBitmap(imageWidth, imageHeight, 1, 1, nullptr);
                auto maskDC = CreateCompatibleDC(destDC);
                auto oldMask = SelectBitmap(maskDC, maskBitmap);

                SetStretchBltMode(memDC, COLORONCOLOR);
                StretchBlt(memDC, 0, 0, originalImageWidth, originalImageHeight, image.imageDC,
                           image.image->GetClipingLeft(), image.image->GetClipingTop(), image.image->GetClipingWidth(),
                           image.image->GetClipingHeight(), SRCCOPY);

                auto oldColor = SetBkColor(memDC, image.image->GetMaskColor());
                BitBlt(maskDC, 0, 0, originalImageWidth, originalImageHeight, memDC, 0, 0, SRCCOPY);

                BitBlt(destDC, realDrawRect.left, realDrawRect.top, realDrawRect.right - realDrawRect.left,
                       realDrawRect.bottom - realDrawRect.top, memDC, realDrawRect.left - imageRect.left,
                       realDrawRect.top - imageRect.top, SRCINVERT);
                BitBlt(destDC, realDrawRect.left, realDrawRect.top, realDrawRect.right - realDrawRect.left,
                       realDrawRect.bottom - realDrawRect.top, maskDC, realDrawRect.left - imageRect.left,
                       realDrawRect.top - imageRect.top, SRCAND);
                BitBlt(destDC, realDrawRect.left, realDrawRect.top, realDrawRect.right - realDrawRect.left,
                       realDrawRect.bottom - realDrawRect.top, memDC, realDrawRect.left - imageRect.left,
                       realDrawRect.top - imageRect.top, SRCINVERT);

                SetBkColor(memDC, oldColor);
                SelectBitmap(maskDC, oldMask);
                SelectBitmap(memDC, oldBitmap);
                DeleteBitmap(memBitmap);
                DeleteBitmap(maskBitmap);
                DeleteDC(memDC);
                DeleteDC(maskDC);
            }
        }
    }
}

void CLayoutControl::Draw(HDC destDC, RECT& clipingRect, COLORREF mixedColor)
{
    for (ImageInformation image : _images)
    {
        if (image.isHide)
        {
            RECT realClipingRect;
            RECT layoutRect;
            SetRect(&layoutRect, _position.x, _position.y, _position.x + _size.cx, _position.y + _size.cy);
            if (!IntersectRect(&realClipingRect, &layoutRect, &clipingRect))
            {
                continue;
            }

            int imageX = (image.position.x * _ratioX) + _position.x;
            int imageY = (image.position.y * +_ratioY) + _position.y;
            auto width = image.image->GetWidth();
            int height = image.image->GetHeight();
            int imageWidth = width * _ratioX;
            int imageHeight = height * _ratioY;

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

                RECT imageRect;
                RECT realDrawRect;
                SetRect(&imageRect, imageX, imageY, imageX + imageWidth, imageY + imageHeight);
                if (!IntersectRect(&realDrawRect, &imageRect, &realClipingRect))
                {
                    continue;
                }

                auto bitmapInfo = image.image->GetBitmapInfo();
                GetDIBits(image.imageDC, image.image->GetImageHandle(), 0, 0, nullptr, &bitmapInfo, DIB_RGB_COLORS);

                int bits = bitmapInfo.bmiHeader.biBitCount / 8;
                auto pixels = new BYTE[bitmapInfo.bmiHeader.biSizeImage];
                GetDIBits(image.imageDC, image.image->GetImageHandle(), 0, height, pixels, &bitmapInfo, DIB_RGB_COLORS);

                auto cx = realDrawRect.right - realDrawRect.left;
                auto cy = realDrawRect.bottom - realDrawRect.top;
                auto drawingImageX = realDrawRect.left - imageRect.left;
                auto drawingImageY = realDrawRect.top - imageRect.top;

                for (int y = height - drawingImageY - 1; y > height - drawingImageY - cy - 1; --y)
                {
                    for (int x = drawingImageX; x < drawingImageX + cx; ++x)
                    {
                        double h, s, v, h2, s2, v2;
                        BYTE b = pixels[(x + y * width) * bits];
                        BYTE g = pixels[(x + y * width) * bits + 1];
                        BYTE r = pixels[(x + y * width) * bits + 2];
                        RgbToHsv(r, g, b, h, s, v);
                        RgbToHsv(GetRValue(mixedColor), GetGValue(mixedColor), GetBValue(mixedColor), h2, s2, v2);
                        h = h2;
                        s = s2;
                        HsvToRgb(h, s, v, r, g, b);
                        pixels[(x + y * width) * bits] = b;
                        pixels[(x + y * width) * bits + 1] = g;
                        pixels[(x + y * width) * bits + 2] = r;
                    }
                }
                BITMAPINFOHEADER* bitmapInfoHeader = (BITMAPINFOHEADER *)&bitmapInfo;
                HBITMAP newBitmap = CreateDIBitmap(image.imageDC, bitmapInfoHeader, CBM_INIT, pixels, &bitmapInfo,
                                                   DIB_RGB_COLORS);
                auto oldBitmap = SelectBitmap(image.imageDC, newBitmap);


                BitBlt(destDC, realDrawRect.left, realDrawRect.top, cx,
                       cy, image.imageDC, drawingImageX,
                       drawingImageY, SRCCOPY);

                SelectObject(image.imageDC, oldBitmap);
                DeleteBitmap(newBitmap);
                delete[] pixels;
            }
            else
            {
                auto memDc = CreateCompatibleDC(destDC);
                auto memBitmap = CreateCompatibleBitmap(destDC, imageWidth, imageHeight);
                auto oldBitmap = SelectBitmap(memDc, memBitmap);

                SetStretchBltMode(memDc, COLORONCOLOR);
                StretchBlt(memDc, 0, 0, imageWidth, imageHeight, image.imageDC, 0, 0, width,
                           height, SRCCOPY);

                if (imageX + imageWidth > _size.cx)
                {
                    imageWidth = _size.cx - imageX;
                }
                if (imageY + imageHeight > _size.cy)
                {
                    imageHeight = _size.cy - imageY;
                }

                RECT imageRect;
                RECT realDrawRect;
                SetRect(&imageRect, imageX, imageY, imageX + imageWidth, imageY + imageHeight);
                if (!IntersectRect(&realDrawRect, &imageRect, &realClipingRect))
                {
                    continue;
                }

                BitBlt(destDC, imageX, imageY, imageWidth, imageHeight, memDc, 0, 0, SRCCOPY);

                SelectBitmap(memDc, oldBitmap);
                DeleteBitmap(memBitmap);
                DeleteDC(memDc);
            }
        }
    }
}

void CLayoutControl::Erase()
{
}

void CLayoutControl::Refresh()
{
    if (!_parents.empty())
    {
        for (auto& parent : _parents)
        {
            for (ImageInformation image : _images)
            {
                int imageX = (image.position.x * _ratioX) + _position.x;
                int imageY = (image.position.y * _ratioY) + _position.y;

                RECT rect;
                SetRect(&rect, imageX, imageY, imageX + image.image->GetClipingWidth() * _ratioX,
                        imageY + image.image->GetClipingHeight() * _ratioY);
                InvalidateRect(parent->GetHWnd(), &rect, TRUE);
            }
        }
    }
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
