#include "ImageControl.h"

#include "CommonLib/ME5File.h"

#include <iterator>

namespace jojogame {
void PngToBmp(std::vector<BYTE>& bmp, const BYTE *pngImage, int size)
{
    std::vector<BYTE> png, image; // the raw pixels
    unsigned w, h;

    png.insert(png.end(), &pngImage[0], &pngImage[size]);
    lodepng::decode(image, w, h, png, LCT_RGB, 8);

    // 3 bytes per pixel used for both input and output.
    int inputChannels = 3;
    int outputChannels = 3;

    // bytes 0-13
    bmp.push_back('B');
    bmp.push_back('M'); // 0: bfType
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0); // 2: bfSize; size not yet known for now, filled in later.
    bmp.push_back(0);
    bmp.push_back(0); // 6: bfReserved1
    bmp.push_back(0);
    bmp.push_back(0); // 8: bfReserved2
    bmp.push_back(54 % 256);
    bmp.push_back(54 / 256);
    bmp.push_back(0);
    bmp.push_back(0); // 10: bfOffBits (54 header bytes)

    // bytes 14-53
    bmp.push_back(40);
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0);  // 14: biSize
    bmp.push_back(static_cast<unsigned char>(w % 256));
    bmp.push_back(static_cast<unsigned char>(w / 256));
    bmp.push_back(0);
    bmp.push_back(0); // 18: biWidth
    bmp.push_back(static_cast<unsigned char>(h % 256));
    bmp.push_back(static_cast<unsigned char>(h / 256));
    bmp.push_back(0);
    bmp.push_back(0); // 22: biHeight
    bmp.push_back(1);
    bmp.push_back(0); // 26: biPlanes
    bmp.push_back(static_cast<unsigned char>(outputChannels * 8));
    bmp.push_back(0); // 28: biBitCount
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0);  // 30: biCompression
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0);  // 34: biSizeImage
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0);  // 38: biXPelsPerMeter
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0);  // 42: biYPelsPerMeter
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0);  // 46: biClrUsed
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0);  // 50: biClrImportant

    /*
    Convert the input RGBRGBRGB pixel buffer to the BMP pixel buffer format. There are 3 differences with the input buffer:
    -BMP stores the rows inversed, from bottom to top
    -BMP stores the color channels in BGR instead of RGB order
    -BMP requires each row to have a multiple of 4 bytes, so sometimes padding bytes are added between rows
    */

    int imagerowbytes = outputChannels * w;
    imagerowbytes = imagerowbytes % 4 == 0 ?
                    imagerowbytes : imagerowbytes + (4 - imagerowbytes % 4); // must be multiple of 4

    for (int y = h - 1; y >= 0; y--) // the rows are stored inversed in bmp
    {
        int c = 0;
        for (int x = 0; x < imagerowbytes; x++)
        {
            if (x < (int) w * outputChannels)
            {
                int inc = c;
                // Convert RGB(A) into BGR(A)
                if (c == 0)
                {
                    inc = 2;
                }
                else if (c == 2)
                {
                    inc = 0;
                }
                bmp.push_back(image[inputChannels * (w * y + x / outputChannels) + inc]);
            }
            else
            {
                bmp.push_back(0);
            }
            c++;
            if (c >= outputChannels)
            {
                c = 0;
            }
        }
    }

    // Fill in the size
    bmp[2] = bmp.size() % 256;
    bmp[3] = (bmp.size() / 256) % 256;
    bmp[4] = (bmp.size() / 65536) % 256;
    bmp[5] = static_cast<unsigned char>(bmp.size() / 16777216);
}


void CImageControl::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN(CImageControl, "_Image");

    LUA_METHOD(GetWidth);
    LUA_METHOD(GetHeight);

    LUA_METHOD(LoadImageFromMe5File);
}

CImageControl::CImageControl()
{

}

CImageControl::~CImageControl()
{
    if (_image)
    {
        DeleteObject(_image);
        _image = nullptr;
    }
    if (_maskImage)
    {
        DeleteObject(_maskImage);
        _maskImage = nullptr;
    }
}

void CImageControl::LoadImageFromMe5File(std::wstring filePath, int groupIndex, int subIndex, COLORREF maskColor)
{
    CME5File imageFile;

    imageFile.Open(filePath);

    int size = imageFile.GetItemByteSize(groupIndex, subIndex);
    auto *by = new BYTE[size];
    std::vector<BYTE> bmp;

    imageFile.GetItemByteArr(by, groupIndex, subIndex);
    PngToBmp(bmp, by, size);

    BYTE *bmpBytes = new BYTE[bmp.size()];
    std::copy(bmp.begin(), bmp.end(), stdext::checked_array_iterator<BYTE *>(bmpBytes, bmp.size()));

    BITMAPFILEHEADER *bmpFileHeader = (BITMAPFILEHEADER *) bmpBytes;
    BITMAPINFOHEADER *bmpInfoHeader = (BITMAPINFOHEADER *) (bmpBytes + sizeof(BITMAPFILEHEADER));
    BITMAPINFO *bmpInfo = (BITMAPINFO *) bmpInfoHeader;
    BYTE *bits = (bmpBytes + bmpFileHeader->bfOffBits);
    _size.cx = bmpInfoHeader->biWidth;
    _size.cy = bmpInfoHeader->biHeight;

    HDC dc = GetDC(nullptr);
    HDC imageDC = CreateCompatibleDC(dc);
    HDC maskDC = CreateCompatibleDC(dc);

    _image = CreateDIBitmap(dc, bmpInfoHeader, CBM_INIT, (void *) bits, bmpInfo, DIB_RGB_COLORS);
    _maskImage = CreateBitmap(_size.cx, _size.cy, 1, 1, nullptr);

    HBITMAP oldImage = (HBITMAP) SelectObject(imageDC, _image);
    HBITMAP oldMask = (HBITMAP) SelectObject(maskDC, _maskImage);
    COLORREF oldColor = SetBkColor(imageDC, maskColor);

    BitBlt(maskDC, 0, 0, _size.cx, _size.cy, imageDC, 0, 0, SRCCOPY);
    BitBlt(imageDC, 0, 0, _size.cx, _size.cy, maskDC, 0, 0, SRCINVERT);

    SetBkColor(imageDC, oldColor);
    SelectObject(imageDC, oldImage);
    SelectObject(maskDC, oldMask);

    DeleteDC(imageDC);
    DeleteDC(maskDC);

    ReleaseDC(nullptr, dc);

    delete[]by;
    delete[]bmpBytes;
}

int CImageControl::GetWidth()
{
    return _size.cx;
}

int CImageControl::GetHeight()
{
    return _size.cy;
}

HBITMAP CImageControl::GetImageHandle()
{
    return _image;
}

HBITMAP CImageControl::GetMaskImageHandle()
{
    return _maskImage;
}
}