#pragma once

#include <windows.h>

namespace jojogame {
class __declspec(dllexport) Application
{
public:
    Application(HINSTANCE);
    virtual ~Application();

    HINSTANCE getHInstance();

    virtual int run() = 0;

    static Application* getInstance();
protected:
    HINSTANCE _hInstance;
    static Application* s_sharedApplication;
};
}
