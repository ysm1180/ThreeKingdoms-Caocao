#include "MoviePlayerControl.h"
#include "ControlManager.h"
#include "WindowControl.h"

#include "BaseLib\ConsoleOutput.h"
#include "CommonLib\GameManager.h"

#include <thread>

namespace jojogame {
void CMoviePlayerControl::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN(CMoviePlayerControl, "_MoviePlayer");

    LUA_METHOD(GetX);
    LUA_METHOD(GetY);
    LUA_METHOD(GetWidth);
    LUA_METHOD(GetHeight);
    LUA_METHOD(IsPlaying);
    LUA_METHOD(SetX);
    LUA_METHOD(SetY);
    LUA_METHOD(SetWidth);
    LUA_METHOD(SetHeight);
    LUA_METHOD(SetEndEvent);

    LUA_METHOD(Play);
    LUA_METHOD(WaitForPlay);
    LUA_METHOD(Stop);
    LUA_METHOD(Create);
    LUA_METHOD(Destroy);
}

CMoviePlayerControl::CMoviePlayerControl(CWindowControl *parent, std::wstring fileName)
{
    if (parent)
    {
        _parent = parent;
    }
    _fileName = fileName;
}

CMoviePlayerControl::~CMoviePlayerControl()
{
    CMoviePlayerControl::Destroy();
}

bool CMoviePlayerControl::IsPlaying()
{
    return _playing;
}

std::wstring CMoviePlayerControl::GetEndEvent()
{
    return _endEvent;
}

void CMoviePlayerControl::SetEndEvent(std::wstring endEvent)
{
    _endEvent = endEvent;
}

void CMoviePlayerControl::SetFileName(std::wstring fileName)
{
    _fileName = fileName;
}

bool CMoviePlayerControl::Create()
{
    AVIFileInit();

    int error = AVIFileOpen(&_aviFile, _fileName.c_str(), OF_READ, NULL);
    if (error)
    {
        AVIFileExit();
        return false;
    }

    AVIFILEINFO aviInfo;
    AVIFileInfo(_aviFile, &aviInfo, sizeof(AVIFILEINFO));
    _fps = (double) aviInfo.dwRate / aviInfo.dwScale;
    _size.cx = aviInfo.dwWidth;
    _size.cy = aviInfo.dwHeight;

    error = AVIFileGetStream(_aviFile, &_aviStream, streamtypeVIDEO, 0);
    if (error)
    {
        AVIFileRelease(_aviFile);
        AVIFileExit();
        return false;
    }

    _position.x = _position.y = 0;

    return true;
}

BOOL CreateFromPackedDIBPointer(LPBYTE pDIB, int iFrame)
{
    //Creates a full-color (no palette) DIB from a pointer to a
    //full-color memory DIB

    //get the BitmapInfoHeader
    BITMAPINFOHEADER bih;
    RtlMoveMemory(&bih.biSize, pDIB, sizeof(BITMAPINFOHEADER));

    //now get the bitmap bits
    if (bih.biSizeImage < 1)
    {
        return FALSE;
    }

    BYTE* Bits = new BYTE[bih.biSizeImage];

    RtlMoveMemory(Bits, pDIB + sizeof(BITMAPINFOHEADER), bih.biSizeImage);

    //and BitmapInfo variable-length UDT
    BYTE memBitmapInfo[40];
    RtlMoveMemory(memBitmapInfo, &bih, sizeof(bih));

    BITMAPFILEHEADER bfh;
    bfh.bfType = 19778;    //BM header
    bfh.bfSize = 55 + bih.biSizeImage;
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfOffBits = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER); //54

    char temp[256];
    sprintf_s(temp, "Frame-%05d.bmp", iFrame);

    FILE* fp;
    fopen_s(&fp, temp, "wb");
    if (fp != NULL)
    {
        fwrite(&bfh, sizeof(bfh), 1, fp);
        fwrite(&memBitmapInfo, sizeof(memBitmapInfo), 1, fp);
        fwrite(Bits, bih.biSizeImage, 1, fp);
        fclose(fp);
    }
    else
    {
        return FALSE;
    }

    delete[] Bits;
    return TRUE;
}

void DrawMovieFrame(CMoviePlayerControl *movieControl, unsigned char *data, CWindowControl *parent)
{
    BITMAPINFO bitmapInfo;
    BITMAPINFOHEADER bitmapInfoHeader;
    ZeroMemory(&bitmapInfo, sizeof(BITMAPINFO));
    RtlMoveMemory(&bitmapInfoHeader.biSize, data, sizeof(BITMAPINFOHEADER));
    bitmapInfo.bmiHeader = bitmapInfoHeader;

    if (bitmapInfoHeader.biSizeImage < 1)
    {
        return;
    }

    auto pixels = new char[bitmapInfoHeader.biSizeImage];
    RtlMoveMemory(pixels, data + sizeof(BITMAPINFOHEADER), bitmapInfoHeader.biSizeImage);

    BYTE memBitmapInfo[40];
    RtlMoveMemory(memBitmapInfo, &bitmapInfoHeader, sizeof(bitmapInfoHeader));

    char *bits;
    HDC dc = GetDC(parent->GetHWnd());
    HDC newDC = CreateCompatibleDC(dc);
    HBITMAP newBitmap = CreateDIBSection(dc, &bitmapInfo, DIB_RGB_COLORS, (void **)&bits, 0, 0);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(newDC, newBitmap);
    SetDIBits(newDC, newBitmap, 0, bitmapInfoHeader.biHeight, pixels, &bitmapInfo, DIB_RGB_COLORS);
    /*parent->AddDrawingImage(newDC,
                            movieControl->GetX(), movieControl->GetY(),
                            movieControl->GetWidth(), movieControl->GetHeight());
    parent->RefreshByRegion(movieControl->GetX(), movieControl->GetY(),
                            movieControl->GetWidth(), movieControl->GetHeight());*/
    delete[] pixels;
    BitBlt(dc, 120, 240, movieControl->GetWidth(), movieControl->GetHeight(), newDC, 0, 0, SRCCOPY);

    SelectObject(newDC, oldBitmap);
    DeleteBitmap(newBitmap);
    DeleteDC(newDC);
    ReleaseDC(parent->GetHWnd(), dc);

}

void CMoviePlayerControl::Play()
{
    std::thread t([&](PAVISTREAM aviStream) {
        int firstFrame = AVIStreamStart(aviStream);
        int numFrames = AVIStreamLength(aviStream);
        PGETFRAME frame = AVIStreamGetFrameOpen(aviStream, NULL);

        for (int i = firstFrame; i < numFrames && _playing; i++)
        {
            BYTE *data = (BYTE *) AVIStreamGetFrame(frame, i - firstFrame);
            DrawMovieFrame(this, data, _parent);
            //CreateFromPackedDIBPointer(data, i - firstFrame);
            CGameManager::GetInstance().Delay(120);
        }
        AVIStreamGetFrameClose(frame);

        _playing = false;
    }, _aviStream);
    t.detach();
    _playing = true;

}

void CMoviePlayerControl::WaitForPlay()
{
    auto quit = false;
    MSG message;

    for (;;)
    {
        if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
        {
            if (message.message == WM_QUIT)
            {
                quit = true;
                break;
            }

            TranslateMessage(&message);
            DispatchMessage(&message);
        } else if (!_playing)
        {
            break;
        }
    }

    Stop();
    if (quit)
    {
        PostQuitMessage(0);
    }
}

void CMoviePlayerControl::Stop()
{
    if (_playing)
    {
        _playing = false;
    }
}

void CMoviePlayerControl::Destroy()
{
    if (_aviStream)
    {
        AVIStreamRelease(_aviStream);
        _aviStream = nullptr;
    }
    if (_aviFile != nullptr)
    {
        AVIFileRelease(_aviFile);
        _aviFile = nullptr;
    }
    AVIFileExit();
    _playing = false;
}

int CMoviePlayerControl::GetX()
{
    return _position.x;
}

int CMoviePlayerControl::GetY()
{
    return _position.y;
}

int CMoviePlayerControl::GetWidth()
{
    return _size.cx;
}

int CMoviePlayerControl::GetHeight()
{
    return _size.cy;
}

void CMoviePlayerControl::SetX(int x)
{
    _position.x = x;
}

void CMoviePlayerControl::SetY(int y)
{
    _position.y = y;
}

void CMoviePlayerControl::SetWidth(int width)
{
    _size.cx = width;
}

void CMoviePlayerControl::SetHeight(int height)
{
    _size.cy = height;
}
}