#include "ImageControl.h"

#include "BaseLib/Color.h"
#include "CommonLib/ME5File.h"
#include "CommonLib/FileManager.h"
#include "CommonLib/lodepng.h"
extern "C" {
    #include <jpeglib.h>
}

#include <iterator>
#include <vector>

namespace jojogame {
static void init_source(j_decompress_ptr cinfo)
{
}
boolean fill_input_buffer(j_decompress_ptr cinfo)
{
    return TRUE;
}
void skip_input_data(j_decompress_ptr cinfo, long num_bytes)
{
    struct jpeg_source_mgr* src = (struct jpeg_source_mgr*) cinfo->src;

    if (num_bytes > 0)
    {
        src->next_input_byte += (size_t)num_bytes;
        src->bytes_in_buffer -= (size_t)num_bytes;
    }
}
void term_source(j_decompress_ptr cinfo)
{
}
void jpeg_mem_src(j_decompress_ptr cinfo, void* buffer, long nbytes)
{
    struct jpeg_source_mgr* src;

    if (cinfo->src == NULL)
    {   /* first time for this JPEG object? */
        cinfo->src = (struct jpeg_source_mgr *)
            (*cinfo->mem->alloc_small) ((j_common_ptr)cinfo, JPOOL_PERMANENT,
                                        sizeof(struct jpeg_source_mgr));
    }

    src = (struct jpeg_source_mgr*) cinfo->src;
    src->init_source = init_source;
    src->fill_input_buffer = fill_input_buffer;
    src->skip_input_data = skip_input_data;
    src->resync_to_restart = jpeg_resync_to_restart; /* use default method */
    src->term_source = term_source;
    src->bytes_in_buffer = nbytes;
    src->next_input_byte = (JOCTET*)buffer;
}

void PngToBmp(std::vector<BYTE>& bmp, const BYTE* pngImage, int size)
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
    bmp.push_back(0); // 14: biSize
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
    bmp.push_back(0); // 30: biCompression
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0); // 34: biSizeImage
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0); // 38: biXPelsPerMeter
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0); // 42: biYPelsPerMeter
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0); // 46: biClrUsed
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0);
    bmp.push_back(0); // 50: biClrImportant

    /*
    Convert the input RGBRGBRGB pixel buffer to the BMP pixel buffer format. There are 3 differences with the input buffer:
    -BMP stores the rows inversed, from bottom to top
    -BMP stores the color channels in BGR instead of RGB order
    -BMP requires each row to have a multiple of 4 bytes, so sometimes padding bytes are added between rows
    */

    int imagerowbytes = outputChannels * w;
    imagerowbytes = imagerowbytes % 4 == 0 ? imagerowbytes : imagerowbytes + (4 - imagerowbytes % 4);
    // must be multiple of 4

    for (int y = h - 1; y >= 0; y--) // the rows are stored inversed in bmp
    {
        int c = 0;
        for (int x = 0; x < imagerowbytes; x++)
        {
            if (x < (int)w * outputChannels)
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

void CImageControl::RegisterFunctions(lua_State* L)
{
    LUA_BEGIN(CImageControl, "_Image");

    LUA_METHOD(GetWidth);
    LUA_METHOD(GetHeight);

    LUA_METHOD(SetClipingRect);
    LUA_METHOD(ResetClipingRect);

    LUA_METHOD(LoadImageFromMe5FileByIndex);
}

CImageControl::CImageControl()
{
    _size.cx = _size.cy = 0;
    _clipingRect.top = _clipingRect.left = _clipingRect.right = _clipingRect.bottom = 0;
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

void CImageControl::ReadJpeg(BYTE *src, int size, COLORREF maskColor, double brightness)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    //initialize error handling
    cinfo.err = jpeg_std_error(&jerr);

    //initialize the decompression
    jpeg_create_decompress(&cinfo);

    jpeg_mem_src(&cinfo, (void *)src, size);
    jpeg_read_header(&cinfo, TRUE);

    jpeg_start_decompress(&cinfo);

    int height = cinfo.output_height;
    int width = cinfo.output_width;
    int calc_width = (cinfo.output_width * cinfo.jpeg_color_space + 3) / 4 * 4;
    BYTE *data, *data_ori;
    data = data_ori = (BYTE *)malloc(calc_width * cinfo.output_height);

    if (data == nullptr)
    {
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
    }

    while (cinfo.output_scanline < cinfo.output_height)
    {
        jpeg_read_scanlines(&cinfo, &data, 1);
        data += calc_width;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    BITMAPINFO bmpInfo = { 0 };
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biWidth = width;
    bmpInfo.bmiHeader.biHeight = 0xFFFFFFFF - height + 1;
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biBitCount = 24;
    bmpInfo.bmiHeader.biCompression = BI_RGB;
    bmpInfo.bmiHeader.biSizeImage = 0;
    bmpInfo.bmiHeader.biXPelsPerMeter = 0;
    bmpInfo.bmiHeader.biYPelsPerMeter = 0;
    bmpInfo.bmiHeader.biClrUsed = 0;
    bmpInfo.bmiHeader.biClrImportant = 0;

    BYTE* bits = data_ori;
    _size.cx = width;
    _size.cy = height;

    this->ResetClipingRect();

    _info = bmpInfo;

    HDC dc = GetDC(nullptr);
    HDC imageDC = CreateCompatibleDC(dc);
    HDC maskDC = CreateCompatibleDC(dc);

    int lineWidth = _size.cx;
    for (int y = 0; y < _size.cy; ++y)
    {
        for (int x = 0; x < lineWidth; ++x)
        {
            double h, s, v;
            BYTE b = bits[(x + y * lineWidth) * 3];
            BYTE g = bits[(x + y * lineWidth) * 3 + 1];
            BYTE r = bits[(x + y * lineWidth) * 3 + 2];

            if (r == GetRValue(maskColor) && g == GetGValue(maskColor) && b == GetBValue(maskColor))
            {
                continue;
            }
            RgbToHsv(r, g, b, h, s, v);
            v *= brightness;
            if (v > 1)
            {
                v = 1;
            }
            HsvToRgb(h, s, v, r, g, b);
            bits[(x + y * lineWidth) * 3] = r;
            bits[(x + y * lineWidth) * 3 + 1] = g;
            bits[(x + y * lineWidth) * 3 + 2] = b;
        }
    }


    _image = CreateDIBitmap(dc, &bmpInfo.bmiHeader, CBM_INIT, (void *)bits, &bmpInfo, DIB_RGB_COLORS);
    _maskImage = CreateBitmap(_size.cx, _size.cy, 1, 1, nullptr);

    HBITMAP oldImage = (HBITMAP)SelectObject(imageDC, _image);
    HBITMAP oldMask = (HBITMAP)SelectObject(maskDC, _maskImage);
    COLORREF oldColor = SetBkColor(imageDC, maskColor);

    BitBlt(maskDC, 0, 0, _size.cx, _size.cy, imageDC, 0, 0, SRCCOPY);
    BitBlt(imageDC, 0, 0, _size.cx, _size.cy, maskDC, 0, 0, SRCINVERT);

    SetBkColor(imageDC, oldColor);
    SelectObject(imageDC, oldImage);
    SelectObject(maskDC, oldMask);

    DeleteDC(imageDC);
    DeleteDC(maskDC);

    ReleaseDC(nullptr, dc);
    free(data_ori);
}

void CImageControl::ReadPng(BYTE *src, int size, COLORREF maskColor, double brightness)
{
    std::vector<BYTE> bmp;
    PngToBmp(bmp, src, size);

    BYTE* bmpBytes = new BYTE[bmp.size()];
    std::copy(bmp.begin(), bmp.end(), stdext::checked_array_iterator<BYTE *>(bmpBytes, bmp.size()));

    BITMAPFILEHEADER* bmpFileHeader = (BITMAPFILEHEADER *)bmpBytes;
    BITMAPINFOHEADER* bmpInfoHeader = (BITMAPINFOHEADER *)(bmpBytes + sizeof(BITMAPFILEHEADER));
    BITMAPINFO* bmpInfo = (BITMAPINFO *)bmpInfoHeader;
    BYTE* bits = (bmpBytes + bmpFileHeader->bfOffBits);
    _size.cx = bmpInfoHeader->biWidth;
    _size.cy = bmpInfoHeader->biHeight;
    
    this->ResetClipingRect();

    _info = *bmpInfo;

    HDC dc = GetDC(nullptr);
    HDC imageDC = CreateCompatibleDC(dc);
    HDC maskDC = CreateCompatibleDC(dc);

    int lineWidth = _size.cx;
    for (int y = 0; y < _size.cy; ++y)
    {
        for (int x = 0; x < lineWidth; ++x)
        {
            double h, s, v;
            BYTE r = bits[(x + y * lineWidth) * 3];
            BYTE g = bits[(x + y * lineWidth) * 3 + 1];
            BYTE b = bits[(x + y * lineWidth) * 3 + 2];

            if (r == GetRValue(maskColor) && g == GetGValue(maskColor) && b == GetBValue(maskColor))
            {
                continue;
            }
            RgbToHsv(r, g, b, h, s, v);
            v *= brightness;
            if (v > 1)
            {
                v = 1;
            }
            HsvToRgb(h, s, v, r, g, b);
            bits[(x + y * lineWidth) * 3] = r;
            bits[(x + y * lineWidth) * 3 + 1] = g;
            bits[(x + y * lineWidth) * 3 + 2] = b;
        }
    }


    _image = CreateDIBitmap(dc, bmpInfoHeader, CBM_INIT, (void *)bits, bmpInfo, DIB_RGB_COLORS);
    _maskImage = CreateBitmap(_size.cx, _size.cy, 1, 1, nullptr);

    HBITMAP oldImage = (HBITMAP)SelectObject(imageDC, _image);
    HBITMAP oldMask = (HBITMAP)SelectObject(maskDC, _maskImage);
    COLORREF oldColor = SetBkColor(imageDC, maskColor);

    BitBlt(maskDC, 0, 0, _size.cx, _size.cy, imageDC, 0, 0, SRCCOPY);
    BitBlt(imageDC, 0, 0, _size.cx, _size.cy, maskDC, 0, 0, SRCINVERT);

    SetBkColor(imageDC, oldColor);
    SelectObject(imageDC, oldImage);
    SelectObject(maskDC, oldMask);

    DeleteDC(imageDC);
    DeleteDC(maskDC);

    ReleaseDC(nullptr, dc);

    delete[]bmpBytes;
}

int CImageControl::GetClipingTop()
{
    return _clipingRect.top;
}

int CImageControl::GetClipingLeft()
{
    return _clipingRect.left;
}

int CImageControl::GetClipingWidth()
{
    return _clipingRect.right - _clipingRect.left;
}

int CImageControl::GetClipingHeight()
{
    return _clipingRect.bottom - _clipingRect.top;
}


void CImageControl::LoadImageFromMe5FileByIndex(std::wstring filePath, int groupIndex, int subIndex, COLORREF maskColor,
                                                double brightness)
{
    CME5File imageFile;

    imageFile.Open(filePath);

    int size = imageFile.GetItemByteSize(groupIndex, subIndex);
    auto* by = new BYTE[size];

    imageFile.GetItemByteArr(by, groupIndex, subIndex);

    _maskColor = maskColor;
    if (by[0] == 0xFF && by[1] == 0xD8)
    {
        this->ReadJpeg(by, size, maskColor, brightness);
    }
    else if (by[0] == 0x89 && by[1] == 0x50)
    {
        this->ReadPng(by, size, maskColor, brightness);
    }

    delete[] by;
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

BITMAPINFO CImageControl::GetBitmapInfo()
{
    return _info;
}

COLORREF CImageControl::GetMaskColor()
{
    return _maskColor;
}

void CImageControl::SetClipingRect(int left, int top, int right, int bottom)
{
    _clipingRect.left = left;
    _clipingRect.top = top;
    _clipingRect.right = right;
    _clipingRect.bottom = bottom;
}

void CImageControl::ResetClipingRect()
{
    _clipingRect.top = _clipingRect.left = 0;
    _clipingRect.right = _size.cx;
    _clipingRect.bottom = _size.cy;
}


}
