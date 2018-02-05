#pragma once
#pragma comment(lib, "vfw32.lib")

#include "BaseControl.h"

#include <string>
#include <Windows.h>
#include <Vfw.h>

namespace jojogame {
class CWindowControl;

class CMoviePlayerControl
{
public:
    static void RegisterFunctions(lua_State *L);

    CMoviePlayerControl(CWindowControl *parent, std::wstring fileName);
    virtual ~CMoviePlayerControl();

    int GetX();
    int GetY();
    int GetWidth();
    int GetHeight();
    bool IsPlaying();
    std::wstring GetEndEvent();
    int GetDrawingIndex();

    void SetX(int x);
    void SetY(int y);
    void SetWidth(int width);
    void SetHeight(int height);
    void SetEndEvent(std::wstring endEvent);
    void SetFileName(std::wstring fileName);
    void SetDrawingIndex(int index);

    bool Create();
    void Destroy();

    void Play();
    void WaitForPlay();
    void Stop();

private:
    std::wstring _endEvent = L"";
    std::wstring _fileName = L"";
    bool _playing = false;
    POINT _position;
    SIZE _size;

    CWindowControl *_parent;
    PAVIFILE _aviFile;
    PAVISTREAM _aviStream;
    double _fps = 0;

    int _drawingIndex = 0;
};
}
