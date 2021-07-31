#include <ole2.h>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

#include "LayoutControl.h"

#include "WindowControl.h"
#include "ImageControl.h"
#include "GraphicText.h"
#include "GraphicRect.h"
#include "BaseLib/Color.h"
#include "CommonLib/GameManager.h"
#include "ControlManager.h"

namespace three_kingdoms
{
void CLayoutControl::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN(CLayoutControl, "_Layout");

    LUA_METHOD(GetX);
    LUA_METHOD(GetY);
    LUA_METHOD(GetWidth);
    LUA_METHOD(GetHeight);

    LUA_METHOD(SetX);
    LUA_METHOD(SetY);
    LUA_METHOD(SetWidth);
    LUA_METHOD(SetHeight);
    LUA_METHOD(SetRatioX);
    LUA_METHOD(SetRatioY);
    LUA_METHOD(SetHide);

    LUA_METHOD(AddImage);
    LUA_METHOD(DeleteImage);
    LUA_METHOD(MoveImage);
    LUA_METHOD(HideImage);
    LUA_METHOD(ShowImage);

    LUA_METHOD(AddText);
    LUA_METHOD(DeleteText);
    LUA_METHOD(MoveText);
    LUA_METHOD(HideText);
    LUA_METHOD(ShowText);

    LUA_METHOD(Refresh);
    LUA_METHOD(Erase);
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
        HBITMAP deletedBitmap = SelectBitmap(imageInfo.imageDC, imageInfo.oldBitmap);
        DeleteBitmap(deletedBitmap);
        if (imageInfo.oldMirrorBitmap != nullptr)
        {
            HBITMAP deletedMirrorBitmap = SelectBitmap(imageInfo.mirrorDC, imageInfo.oldMirrorBitmap);
            DeleteBitmap(deletedMirrorBitmap);
        }

        DeleteDC(imageInfo.imageDC);
        DeleteDC(imageInfo.mirrorDC);
    }
    DeleteDC(_dc);
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

bool CLayoutControl::IsHide() const
{
    return _isHide;
}

void CLayoutControl::SetX(int x, bool isRedraw)
{
    if (_position.x != x)
    {
        if (isRedraw)
        {
            if (!_parents.empty())
            {
                for (auto &parent : _parents)
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
                    SetRect(&rect, _position.x, _position.y, _position.x + int(width * _ratioX),
                            _position.y + int(height * _ratioY));
                    _refreshRect.push_back(rect);
                }
            }

            _position.x = x;

            if (!_parents.empty())
            {
                for (auto &parent : _parents)
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
                    SetRect(&rect, _position.x, _position.y, _position.x + int(width * _ratioX),
                            _position.y + int(height * _ratioY));
                    _refreshRect.push_back(rect);
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
                for (auto &parent : _parents)
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
                    SetRect(&rect, _position.x, _position.y, _position.x + int(width * _ratioX),
                            _position.y + int(height * _ratioY));
                    _refreshRect.push_back(rect);
                }
            }

            _position.y = y;

            if (!_parents.empty())
            {
                for (auto &parent : _parents)
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
                    SetRect(&rect, _position.x, _position.y, _position.x + int(width * _ratioX),
                            _position.y + int(height * _ratioY));
                    _refreshRect.push_back(rect);
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
                for (auto &parent : _parents)
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
                    SetRect(&rect, _position.x, _position.y, _position.x + int(width * _ratioX),
                            _position.y + int(height * _ratioY));
                    _refreshRect.push_back(rect);
                }
            }

            _size.cx = cx;

            if (!_parents.empty())
            {
                for (auto &parent : _parents)
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
                    SetRect(&rect, _position.x, _position.y, _position.x + int(width * _ratioX),
                            _position.y + int(height * _ratioY));
                    _refreshRect.push_back(rect);
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
            for (auto &parent : _parents)
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
                SetRect(&rect, _position.x, _position.y, _position.x + int(width * _ratioX),
                        _position.y + int(height * _ratioY));
                _refreshRect.push_back(rect);
            }
        }

        _size.cy = cy;

        if (!_parents.empty())
        {
            for (auto &parent : _parents)
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
                SetRect(&rect, _position.x, _position.y, _position.x + int(width * _ratioX),
                        _position.y + int(height * _ratioY));
                _refreshRect.push_back(rect);
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
            for (auto &parent : _parents)
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
                SetRect(&rect, _position.x, _position.y, _position.x + int(width * _ratioX),
                        _position.y + int(height * _ratioY));
                _refreshRect.push_back(rect);
            }
        }

        _ratioX = ratio;

        if (!_parents.empty())
        {
            for (auto &parent : _parents)
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
                SetRect(&rect, _position.x, _position.y, _position.x + int(width * _ratioX),
                        _position.y + int(height * _ratioY));
                _refreshRect.push_back(rect);
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
            for (auto &parent : _parents)
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
                SetRect(&rect, _position.x, _position.y, _position.x + int(width * _ratioX),
                        _position.y + int(height * _ratioY));
                _refreshRect.push_back(rect);
            }
        }

        _ratioY = ratio;

        if (!_parents.empty())
        {
            for (auto &parent : _parents)
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
                SetRect(&rect, _position.x, _position.y, _position.x + int(width * _ratioX),
                        _position.y + int(height * _ratioY));
                _refreshRect.push_back(rect);
            }
        }
    }
    else
    {
        _ratioY = ratio;
    }
}

void CLayoutControl::SetHide(bool value)
{
    _isHide = value;
    this->Refresh();
}

void CLayoutControl::AddParentWindow(CWindowControl *parent)
{
    _parents.push_back(parent);
}

void CLayoutControl::RemoveParentWIndow(CWindowControl *parent)
{
    _parents.erase(std::remove(_parents.begin(), _parents.end(), parent), _parents.end());
}

int CLayoutControl::AddImage(CImageControl *image, int x, int y, bool isShow)
{
    HDC imageDC = CreateCompatibleDC(_dc);
    HBITMAP oldBitmap = SelectBitmap(imageDC, image->GetImageHandle());

    auto bitmapInfo = image->GetBitmapInfo();
    GetDIBits(imageDC, image->GetImageHandle(), 0, 0, nullptr, &bitmapInfo, DIB_RGB_COLORS);
    auto pixels = new BYTE[bitmapInfo.bmiHeader.biSizeImage];
    GetDIBits(imageDC, image->GetImageHandle(), 0, image->GetHeight(), pixels, &bitmapInfo, DIB_RGB_COLORS);

    BITMAPINFOHEADER *bitmapInfoHeader = (BITMAPINFOHEADER *)&bitmapInfo;
    HBITMAP newBitmap = CreateDIBitmap(imageDC, bitmapInfoHeader, CBM_INIT, pixels, &bitmapInfo,
                                       DIB_RGB_COLORS);

    SelectBitmap(imageDC, oldBitmap);
    DeleteDC(imageDC);
    delete[] pixels;

    HBITMAP newMirrorBitmap = nullptr;
    if (image->GetMirrorImageHandle() != nullptr)
    {
        HDC mirrorDC = CreateCompatibleDC(_dc);
        HBITMAP oldMirrorBitmap = SelectBitmap(mirrorDC, image->GetMirrorImageHandle());
        auto bitmapInfo = image->GetBitmapInfo();
        GetDIBits(mirrorDC, image->GetMirrorImageHandle(), 0, 0, nullptr, &bitmapInfo, DIB_RGB_COLORS);
        auto mirrorPixels = new BYTE[bitmapInfo.bmiHeader.biSizeImage];
        GetDIBits(mirrorDC, image->GetMirrorImageHandle(), 0, image->GetHeight(), mirrorPixels, &bitmapInfo,
                  DIB_RGB_COLORS);

        BITMAPINFOHEADER *bitmapInfoHeader = (BITMAPINFOHEADER *)&bitmapInfo;
        newMirrorBitmap = CreateDIBitmap(mirrorDC, bitmapInfoHeader, CBM_INIT, mirrorPixels, &bitmapInfo,
                                         DIB_RGB_COLORS);

        SelectBitmap(mirrorDC, oldBitmap);
        DeleteDC(mirrorDC);
        delete[] mirrorPixels;
    }

    int index = _GetNewImageIndex();
    ImageInformation imageInfo;
    HDC newDC = CreateCompatibleDC(_dc);
    HDC newMirrorDC = CreateCompatibleDC(_dc);
    imageInfo.oldBitmap = SelectBitmap(newDC, newBitmap);
    imageInfo.oldMirrorBitmap = newMirrorBitmap != nullptr ? SelectBitmap(newMirrorDC, newMirrorBitmap) : nullptr;
    imageInfo.imageDC = newDC;
    imageInfo.mirrorDC = newMirrorDC;
    imageInfo.index = index;
    imageInfo.image = image;
    imageInfo.position.x = x;
    imageInfo.position.y = y;
    imageInfo.isHide = !isShow;
    imageInfo.isRefresh = true;

    _images.push_back(imageInfo);

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

            int imageX = int(position.x * _ratioX) + _position.x;
            int imageY = int(position.y * _ratioY) + _position.y;
            RECT rect;
            SetRect(&rect, imageX, imageY, imageX + int(image->GetWidth() * _ratioX),
                    imageY + int(image->GetHeight() * _ratioY));
            _refreshRect.push_back(rect);

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
            RECT rect;
            int imageX = int(iter->position.x * _ratioX) + _position.x;
            int imageY = int(iter->position.y * _ratioY) + _position.y;
            SetRect(&rect, imageX, imageY, imageX + int(iter->image->GetWidth() * _ratioX),
                    imageY + int(iter->image->GetHeight() * _ratioY));
            _refreshRect.push_back(rect);

            iter->position.x = x;
            iter->position.y = y;

            iter->isRefresh = true;
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
            iter->isHide = true;
            iter->isRefresh = true;
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
            iter->isHide = false;
            iter->isRefresh = true;
            break;
        }
        ++iter;
    }
}

int CLayoutControl::AddText(CGraphicText *text, int x, int y, bool isShow)
{
    int index = _GetNewTextIndex();
    TextInformation textInformation;
    textInformation.index = index;
    textInformation.text = text;
    textInformation.position.x = x;
    textInformation.position.y = y;
    textInformation.isHide = !isShow;
    textInformation.isRefresh = true;

    _texts.push_back(textInformation);

    return index;
}

void CLayoutControl::DeleteText(int index, bool isUpdate)
{
    auto iter = std::begin(_texts);

    while (iter != std::end(_texts))
    {
        if (iter->index == index)
        {
            auto position = iter->position;
            auto text = iter->text;

            _reusingTextIndex.push(iter->index);
            iter = _texts.erase(iter);

            if (!_parents.empty())
            {
                for (auto &parent : _parents)
                {
                    int textX = int(position.x * _ratioX) + _position.x;
                    int textY = int(position.y * _ratioY) + _position.y;
                    RECT rect;

                    HDC hdc = GetDC(parent->GetHWnd());
                    SetRect(&rect, textX, textY, textX + int(text->GetWidth(hdc) * _ratioX),
                            textY + int(text->GetHeight(hdc) * _ratioY));
                    _refreshRect.push_back(rect);
                }
            }

            break;
        }
        ++iter;
    }
}

void CLayoutControl::MoveText(int index, int x, int y, bool isUpdate)
{
    auto iter = std::begin(_texts);

    while (iter != std::end(_texts))
    {
        if (iter->index == index)
        {
            if (!_parents.empty())
            {
                for (auto &parent : _parents)
                {
                    int textX = int(iter->position.x * _ratioX) + _position.x;
                    int textY = int(iter->position.y * _ratioY) + _position.y;
                    RECT rect;

                    HDC hdc = GetDC(parent->GetHWnd());
                    SetRect(&rect, textX, textY, textX + int(iter->text->GetWidth(hdc) * _ratioX),
                            textY + int(iter->text->GetHeight(hdc) * _ratioY));
                    _refreshRect.push_back(rect);
                    ReleaseDC(parent->GetHWnd(), hdc);
                }
            }

            iter->position.x = x;
            iter->position.y = y;
            iter->isRefresh = true;
            break;
        }
        ++iter;
    }
}

void CLayoutControl::HideText(int index, bool isUpdate)
{
    auto iter = std::begin(_texts);

    while (iter != std::end(_texts))
    {
        if (iter->index == index)
        {
            iter->isHide = true;
            iter->isRefresh = true;
            break;
        }
        ++iter;
    }
}

void CLayoutControl::ShowText(int index, bool isUpdate)
{
    auto iter = std::begin(_texts);

    while (iter != std::end(_texts))
    {
        if (iter->index == index)
        {
            iter->isHide = false;
            iter->isRefresh = true;
            break;
        }
        ++iter;
    }
}

void CLayoutControl::Draw(HDC destDC)
{
    if (!_isHide)
    {
        for (ImageInformation image : _images)
        {
            if (!image.isHide)
            {
                HDC imageDC = image.image->IsDisplayMirror() ? image.mirrorDC : image.imageDC;

                int imageX = int(image.position.x * _ratioX) + _position.x;
                int imageY = int(image.position.y * +_ratioY) + _position.y;
                int imageWidth = int(image.image->GetClipingWidth() * _ratioX);
                int imageHeight = int(image.image->GetClipingHeight() * _ratioY);

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
                    HBITMAP maskBitmap = image.image->IsDisplayMirror()
                                             ? image.image->GetMaskMirrorImageHandle()
                                             : image.image->GetMaskImageHandle();
                    auto oldMask = SelectBitmap(maskDC, maskBitmap);
                    auto oldColor = SetBkColor(imageDC, image.image->GetMaskColor());

                    BitBlt(destDC, imageX, imageY, imageWidth, imageHeight, imageDC, image.image->GetClipingLeft(),
                           image.image->GetClipingTop(), SRCINVERT);
                    BitBlt(destDC, imageX, imageY, imageWidth, imageHeight, maskDC, image.image->GetClipingLeft(),
                           image.image->GetClipingTop(), SRCAND);
                    BitBlt(destDC, imageX, imageY, imageWidth, imageHeight, imageDC, image.image->GetClipingLeft(),
                           image.image->GetClipingTop(), SRCINVERT);

                    SetBkColor(imageDC, oldColor);
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
                    StretchBlt(memDC, 0, 0, imageWidth, imageHeight, imageDC, image.image->GetClipingLeft(),
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

        for (TextInformation text : _texts)
        {
            if (!text.isHide)
            {
                int textX = int(text.position.x * _ratioX) + _position.x;
                int textY = int(text.position.y * +_ratioY) + _position.y;

                text.text->Draw(destDC, POINT{textX, textY});
            }
        }

        for (RectInformation rect : _rects)
        {
            if (!rect.isHide)
            {
                Gdiplus::Graphics *graphics = Gdiplus::Graphics::FromHDC(destDC);
                int r = GetRValue(rect.rect->GetColor());
                int g = GetGValue(rect.rect->GetColor());
                int b = GetBValue(rect.rect->GetColor());
                Gdiplus::Color color((int)(rect.rect->GetOpacity() * 255), r, g, b);

                Gdiplus::Rect rectangle(rect.position.x, rect.position.y, rect.position.x + rect.rect->GetWidth(), rect.position.y + rect.rect->GetHeight());
                Gdiplus::SolidBrush solidBrush(color);
                graphics->FillRectangle(&solidBrush, rectangle);
            }
        }
    }
}

void CLayoutControl::Draw(HDC destDC, RECT &clipingRect)
{
    if (!_isHide)
    {
        RECT realClipingRect;
        RECT layoutRect;
        SetRect(&layoutRect, _position.x, _position.y, _position.x + _size.cx, _position.y + _size.cy);
        if (!IntersectRect(&realClipingRect, &layoutRect, &clipingRect))
        {
            return;
        }

        for (ImageInformation image : _images)
        {
            if (!image.isHide)
            {
                HDC imageDC = image.image->IsDisplayMirror() ? image.mirrorDC : image.imageDC;

                int imageX = int(image.position.x * _ratioX) + _position.x;
                int imageY = int(image.position.y * _ratioY) + _position.y;
                int imageWidth = int(image.image->GetClipingWidth() * _ratioX);
                int imageHeight = int(image.image->GetClipingHeight() * _ratioY);

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
                    HBITMAP maskBitmap = image.image->IsDisplayMirror()
                                             ? image.image->GetMaskMirrorImageHandle()
                                             : image.image->GetMaskImageHandle();
                    auto oldMask = SelectBitmap(maskDC, maskBitmap);
                    auto oldColor = SetBkColor(imageDC, image.image->GetMaskColor());

                    BitBlt(destDC, realDrawRect.left, realDrawRect.top, realDrawRect.right - realDrawRect.left,
                           realDrawRect.bottom - realDrawRect.top, imageDC,
                           realDrawRect.left - imageRect.left + image.image->GetClipingLeft(),
                           realDrawRect.top - imageRect.top + image.image->GetClipingTop(), SRCINVERT);
                    BitBlt(destDC, realDrawRect.left, realDrawRect.top, realDrawRect.right - realDrawRect.left,
                           realDrawRect.bottom - realDrawRect.top, maskDC,
                           realDrawRect.left - imageRect.left + image.image->GetClipingLeft(),
                           realDrawRect.top - imageRect.top + image.image->GetClipingTop(), SRCAND);
                    BitBlt(destDC, realDrawRect.left, realDrawRect.top, realDrawRect.right - realDrawRect.left,
                           realDrawRect.bottom - realDrawRect.top, imageDC,
                           realDrawRect.left - imageRect.left + image.image->GetClipingLeft(),
                           realDrawRect.top - imageRect.top + image.image->GetClipingTop(), SRCINVERT);

                    SetBkColor(imageDC, oldColor);
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
                    StretchBlt(memDC, 0, 0, originalImageWidth, originalImageHeight, imageDC,
                               image.image->GetClipingLeft(), image.image->GetClipingTop(),
                               image.image->GetClipingWidth(),
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

        for (TextInformation text : _texts)
        {
            if (!text.isHide)
            {
                int textX = int(text.position.x * _ratioX) + _position.x;
                int textY = int(text.position.y * _ratioY) + _position.y;
                int textWidth = int(text.text->GetWidth(destDC) * _ratioX);
                int textHeight = int(text.text->GetHeight(destDC) * _ratioY);

                RECT textRect;
                RECT realDrawRect;
                SetRect(&textRect, textX, textY, textX + textWidth, textY + textHeight);
                if (!IntersectRect(&realDrawRect, &textRect, &realClipingRect))
                {
                    continue;
                }

                text.text->Draw(destDC, POINT{textX, textY});
            }
        }

        for (RectInformation rect : _rects)
        {
            if (!rect.isHide)
            {
                Gdiplus::Graphics *graphics = Gdiplus::Graphics::FromHDC(destDC);
                int r = GetRValue(rect.rect->GetColor());
                int g = GetGValue(rect.rect->GetColor());
                int b = GetBValue(rect.rect->GetColor());
                Gdiplus::Color color((int)(rect.rect->GetOpacity() * 255), r, g, b);

                Gdiplus::Rect rectangle(rect.position.x, rect.position.y, rect.position.x + rect.rect->GetWidth(), rect.position.y + rect.rect->GetHeight());
                Gdiplus::SolidBrush solidBrush(color);
                graphics->FillRectangle(&solidBrush, rectangle);
            }
        }
    }
}

void CLayoutControl::Draw(HDC destDC, RECT &clipingRect, COLORREF mixedColor)
{
    if (!_isHide)
    {
        RECT realClipingRect;
        RECT layoutRect;
        SetRect(&layoutRect, _position.x, _position.y, _position.x + _size.cx, _position.y + _size.cy);
        if (!IntersectRect(&realClipingRect, &layoutRect, &clipingRect))
        {
            return;
        }

        for (ImageInformation image : _images)
        {
            if (!image.isHide)
            {
                HDC imageDC = image.image->IsDisplayMirror() ? image.mirrorDC : image.imageDC;

                int imageX = int(image.position.x * _ratioX) + _position.x;
                int imageY = int(image.position.y * +_ratioY) + _position.y;
                auto width = image.image->GetClipingWidth();
                int height = image.image->GetClipingHeight();
                int imageWidth = int(width * _ratioX);
                int imageHeight = int(height * _ratioY);

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
                    GetDIBits(imageDC, image.image->GetImageHandle(), 0, 0, nullptr, &bitmapInfo, DIB_RGB_COLORS);

                    int bits = bitmapInfo.bmiHeader.biBitCount / 8;
                    auto pixels = new BYTE[bitmapInfo.bmiHeader.biSizeImage];
                    GetDIBits(imageDC, image.image->GetImageHandle(), 0, height, pixels, &bitmapInfo, DIB_RGB_COLORS);

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
                            if (RGB(r, g, b) == image.image->GetMaskColor())
                            {
                                continue;
                            }
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
                    BITMAPINFOHEADER *bitmapInfoHeader = (BITMAPINFOHEADER *)&bitmapInfo;
                    HDC newDC = CreateCompatibleDC(nullptr);
                    HBITMAP newBitmap = CreateDIBitmap(imageDC, bitmapInfoHeader, CBM_INIT, pixels, &bitmapInfo,
                                                       DIB_RGB_COLORS);
                    auto oldBitmap = SelectBitmap(newDC, newBitmap);

                    auto maskBitmap = CreateBitmap(imageWidth, imageHeight, 1, 1, nullptr);
                    auto maskDC = CreateCompatibleDC(nullptr);
                    auto oldMask = SelectBitmap(maskDC, maskBitmap);

                    auto oldColor = SetBkColor(newDC, image.image->GetMaskColor());
                    BitBlt(maskDC, 0, 0, imageWidth, imageHeight, newDC, 0, 0, SRCCOPY);

                    auto oldBackColor = SetBkColor(destDC, RGB(255, 255, 255));
                    auto oldTextColor = SetTextColor(destDC, RGB(0, 0, 0));
                    BitBlt(destDC, realDrawRect.left, realDrawRect.top, cx, cy, newDC,
                           drawingImageX + image.image->GetClipingLeft(),
                           drawingImageY + image.image->GetClipingTop(), SRCINVERT);
                    BitBlt(destDC, realDrawRect.left, realDrawRect.top, cx, cy, maskDC,
                           drawingImageX + image.image->GetClipingLeft(),
                           drawingImageY + image.image->GetClipingTop(), SRCAND);
                    BitBlt(destDC, realDrawRect.left, realDrawRect.top, cx, cy, newDC,
                           drawingImageX + image.image->GetClipingLeft(),
                           drawingImageY + image.image->GetClipingTop(), SRCINVERT);

                    SetBkColor(destDC, oldBackColor);
                    SetTextColor(destDC, oldTextColor);

                    SelectBitmap(maskDC, oldMask);
                    DeleteDC(maskDC);
                    SetBkColor(newDC, oldColor);
                    SelectObject(newDC, oldBitmap);
                    DeleteDC(newDC);
                    DeleteBitmap(newBitmap);
                    DeleteBitmap(maskBitmap);
                    delete[] pixels;
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

                    auto bitmapInfo = image.image->GetBitmapInfo();
                    GetDIBits(imageDC, image.image->GetImageHandle(), 0, 0, nullptr, &bitmapInfo, DIB_RGB_COLORS);

                    int bits = bitmapInfo.bmiHeader.biBitCount / 8;
                    auto pixels = new BYTE[bitmapInfo.bmiHeader.biSizeImage];
                    GetDIBits(imageDC, image.image->GetImageHandle(), 0, height, pixels, &bitmapInfo, DIB_RGB_COLORS);

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
                            if (RGB(r, g, b) == image.image->GetMaskColor())
                            {
                                continue;
                            }
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

                    auto memDC = CreateCompatibleDC(destDC);
                    auto memBitmap = CreateCompatibleBitmap(destDC, originalImageWidth, originalImageHeight);
                    auto oldBitmap = SelectBitmap(memDC, memBitmap);

                    auto maskBitmap = CreateBitmap(imageWidth, imageHeight, 1, 1, nullptr);
                    auto maskDC = CreateCompatibleDC(destDC);
                    auto oldMask = SelectBitmap(maskDC, maskBitmap);

                    SetStretchBltMode(memDC, COLORONCOLOR);
                    StretchBlt(memDC, 0, 0, originalImageWidth, originalImageHeight, imageDC,
                               image.image->GetClipingLeft(), image.image->GetClipingTop(),
                               image.image->GetClipingWidth(),
                               image.image->GetClipingHeight(), SRCCOPY);

                    auto oldColor = SetBkColor(memDC, image.image->GetMaskColor());
                    BitBlt(maskDC, 0, 0, originalImageWidth, originalImageHeight, memDC, 0, 0, SRCCOPY);

                    BitBlt(destDC, realDrawRect.left, realDrawRect.top, cx,
                           cy, imageDC,
                           drawingImageX + image.image->GetClipingLeft(),
                           drawingImageY + image.image->GetClipingTop(), SRCINVERT);
                    BitBlt(destDC, realDrawRect.left, realDrawRect.top, cx, cy, maskDC,
                           drawingImageX + image.image->GetClipingLeft(),
                           drawingImageY + image.image->GetClipingTop(), SRCAND);
                    BitBlt(destDC, realDrawRect.left, realDrawRect.top, cx, cy, imageDC,
                           drawingImageX + image.image->GetClipingLeft(),
                           drawingImageY + image.image->GetClipingTop(), SRCINVERT);

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

        for (TextInformation text : _texts)
        {
            if (!text.isHide)
            {
                int textX = int(text.position.x * _ratioX) + _position.x;
                int textY = int(text.position.y * _ratioY) + _position.y;
                int textWidth = int(text.text->GetWidth(destDC) * _ratioX);
                int textHeight = int(text.text->GetHeight(destDC) * _ratioY);

                RECT textRect;
                RECT realDrawRect;
                SetRect(&textRect, textX, textY, textX + textWidth, textY + textHeight);
                if (!IntersectRect(&realDrawRect, &textRect, &realClipingRect))
                {
                    continue;
                }

                text.text->Draw(destDC, POINT{textX, textY});
            }
        }

        for (RectInformation rect : _rects)
        {
            if (!rect.isHide)
            {
                Gdiplus::Graphics *graphics = Gdiplus::Graphics::FromHDC(destDC);
                int r = GetRValue(rect.rect->GetColor());
                int g = GetGValue(rect.rect->GetColor());
                int b = GetBValue(rect.rect->GetColor());
                Gdiplus::Color color((int)(rect.rect->GetOpacity() * 255), r, g, b);

                Gdiplus::Rect rectangle(rect.position.x, rect.position.y, rect.position.x + rect.rect->GetWidth(), rect.position.y + rect.rect->GetHeight());
                Gdiplus::SolidBrush solidBrush(color);
                graphics->FillRectangle(&solidBrush, rectangle);
            }
        }
    }
}

void CLayoutControl::Erase()
{
    std::vector<ImageInformation> tempImages;
    std::vector<TextInformation> tempTexts;

    for (ImageInformation image : _images)
    {
        tempImages.push_back(image);
    }
    for (TextInformation text : _texts)
    {
        tempTexts.push_back(text);
    }

    _images.clear();
    _texts.clear();
    while (!_reusingImageIndex.empty())
    {
        _reusingImageIndex.pop();
    }
    while (!_reusingTextIndex.empty())
    {
        _reusingTextIndex.pop();
    }

    for (auto &parent : _parents)
    {
        for (ImageInformation image : tempImages)
        {
            if (!image.isHide)
            {
                int imageX = int(image.position.x * _ratioX) + _position.x;
                int imageY = int(image.position.y * _ratioY) + _position.y;

                RECT rect;
                SetRect(&rect, imageX, imageY, imageX + int(image.image->GetClipingWidth() * _ratioX),
                        imageY + int(image.image->GetClipingHeight() * _ratioY));
                InvalidateRect(parent->GetHWnd(), &rect, FALSE);
            }
        }

        HDC hdc = GetDC(parent->GetHWnd());
        for (TextInformation text : tempTexts)
        {
            if (!text.isHide)
            {
                int textX = int(text.position.x * _ratioX) + _position.x;
                int textY = int(text.position.y * _ratioY) + _position.y;

                RECT rect;
                SetRect(&rect, textX, textY, textX + int(text.text->GetWidth(hdc) * _ratioX),
                        textY + int(text.text->GetHeight(hdc) * _ratioY));
                InvalidateRect(parent->GetHWnd(), &rect, FALSE);
            }
        }
        ReleaseDC(parent->GetHWnd(), hdc);
    }
}

int CLayoutControl::AddRect(CGraphicRect *graphicRect, int x, int y, bool isShow)
{
    int index = _GetNewRectIndex();
    RectInformation rectInformation;
    rectInformation.index = index;
    rectInformation.rect = graphicRect;
    rectInformation.position.x = x;
    rectInformation.position.y = y;
    rectInformation.isHide = !isShow;
    rectInformation.isRefresh = true;

    _rects.push_back(rectInformation);

    return index;
}

void CLayoutControl::DeleteRect(int index, bool isUpdate)
{
    auto iter = std::begin(_rects);

    while (iter != std::end(_rects))
    {
        if (iter->index == index)
        {
            auto position = iter->position;
            auto graphicRect = iter->rect;

            _reusingTextIndex.push(iter->index);
            iter = _rects.erase(iter);

            int rectX = int(position.x * _ratioX) + _position.x;
            int rectY = int(position.y * _ratioY) + _position.y;
            RECT rect;
            SetRect(&rect, rectX, rectY, rectX + int(graphicRect->GetWidth() * _ratioX),
                    rectY + int(graphicRect->GetHeight() * _ratioY));
            _refreshRect.push_back(rect);
            break;
        }
        ++iter;
    }
}

void CLayoutControl::MoveRect(int index, int x, int y, bool isUpdate)
{
    auto iter = std::begin(_rects);

    while (iter != std::end(_rects))
    {
        if (iter->index == index)
        {
            int rectX = int(iter->position.x * _ratioX) + _position.x;
            int rectY = int(iter->position.y * _ratioY) + _position.y;
            RECT rect;
            SetRect(&rect, rectX, rectY, rectX + int(iter->rect->GetWidth() * _ratioX),
                    rectY + int(iter->rect->GetHeight() * _ratioY));
            _refreshRect.push_back(rect);

            iter->position.x = x;
            iter->position.y = y;
            iter->isRefresh = true;
            break;
        }
        ++iter;
    }
}

void CLayoutControl::HideRect(int index, bool isUpdate)
{
    auto iter = std::begin(_rects);

    while (iter != std::end(_rects))
    {
        if (iter->index == index)
        {
            iter->isHide = true;
            iter->isRefresh = true;
            break;
        }
        ++iter;
    }
}

void CLayoutControl::ShowRect(int index, bool isUpdate)
{
    auto iter = std::begin(_rects);

    while (iter != std::end(_rects))
    {
        if (iter->index == index)
        {
            iter->isHide = false;
            iter->isRefresh = true;
            break;
        }
        ++iter;
    }
}

void CLayoutControl::Refresh()
{
    bool update = false;

    if (!_parents.empty())
    {
        for (auto &parent : _parents)
        {
            for (RECT rect : _refreshRect)
            {
                InvalidateRect(parent->GetHWnd(), &rect, FALSE);

                update = true;
            }

            if (_refreshRect.size() > 0)
            {
                _refreshRect.clear();
            }

            for (ImageInformation &image : _images)
            {
                if (image.isRefresh)
                {
                    int imageX = int(image.position.x * _ratioX) + _position.x;
                    int imageY = int(image.position.y * _ratioY) + _position.y;

                    RECT rect;
                    SetRect(&rect, imageX, imageY, imageX + int(image.image->GetClipingWidth() * _ratioX),
                            imageY + int(image.image->GetClipingHeight() * _ratioY));
                    InvalidateRect(parent->GetHWnd(), &rect, FALSE);

                    image.isRefresh = false;

                    update = true;
                }
            }

            HDC hdc = GetDC(parent->GetHWnd());
            for (TextInformation &text : _texts)
            {
                if (text.isRefresh)
                {
                    int textX = int(text.position.x * _ratioX) + _position.x;
                    int textY = int(text.position.y * _ratioY) + _position.y;

                    RECT rect;
                    SetRect(&rect, textX, textY, textX + int(text.text->GetWidth(hdc) * _ratioX),
                            textY + int(text.text->GetHeight(hdc) * _ratioY));
                    InvalidateRect(parent->GetHWnd(), &rect, FALSE);

                    text.isRefresh = false;

                    update = true;
                }
            }

            for (RectInformation &rect : _rects)
            {
                if (rect.isRefresh)
                {
                    int rectX = int(rect.position.x * _ratioX) + _position.x;
                    int rectY = int(rect.position.y * _ratioY) + _position.y;

                    RECT rc;
                    SetRect(&rc, rectX, rectY, rectX + int(rect.rect->GetWidth() * _ratioX),
                            rectY + int(rect.rect->GetHeight() * _ratioY));
                    InvalidateRect(parent->GetHWnd(), &rc, FALSE);

                    rect.isRefresh = false;

                    update = true;
                }
            }

            ReleaseDC(parent->GetHWnd(), hdc);

            if (update)
            {
                UpdateWindow(parent->GetHWnd());
            }
        }
    }
}

int CLayoutControl::_GetNewImageIndex()
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

int CLayoutControl::_GetNewTextIndex()
{
    int index;

    if (_reusingTextIndex.empty())
    {
        index = _texts.size();
    }
    else
    {
        index = _reusingTextIndex.front();
        _reusingTextIndex.pop();
    }

    return index;
}

int CLayoutControl::_GetNewRectIndex()
{
    int index;

    if (_reusingRectIndex.empty())
    {
        index = _rects.size();
    }
    else
    {
        index = _reusingRectIndex.front();
        _reusingRectIndex.pop();
    }

    return index;
}
} // namespace jojogame
