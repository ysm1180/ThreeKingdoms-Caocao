#pragma once

#include <Windows.h>

extern "C" {
#include <libavformat/avformat.h>
#include <SDL/include/SDL.h>
}

namespace jojogame {
class CControlManager;
class CGameManager;
class CGameScriptManager;

class Application
{
public:
    explicit Application(HINSTANCE hInstance);
    virtual ~Application();

    HINSTANCE GetHInstance();

    virtual int Run();

    static Application& GetInstance();
protected:
    HINSTANCE _hInstance;
    CControlManager *_controlManager;
    CGameManager *_gameManager;

    static Application *s_sharedApplication;
};
}  // namespace jojogame
