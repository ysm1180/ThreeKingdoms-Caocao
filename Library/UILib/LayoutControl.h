#pragma once

#include <atlcoll.h>
#include <windows.h>
#include <CommCtrl.h>
#include <vector>
#include <utility>

namespace jojogame {
struct ImageInformation
{
    HBITMAP bitmap;
    POINT position;
    SIZE size;
};

class CLayoutControl
{
public:
    CLayoutControl();
    virtual ~CLayoutControl();

private:
    HDC _dc;
    std::vector<ImageInformation> _images;
};
}
