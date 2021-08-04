#pragma once

#include <Windows.h>

extern "C" {
#include <SDL/include/SDL.h>
#include <libavformat/avformat.h>
}

namespace three_kingdoms {
class CControlManager;
class CGameManager;
class CGameScriptManager;
class CFileManager;

class Application {
public:
    explicit Application(HINSTANCE hInstance);
    virtual ~Application();

    HINSTANCE GetHInstance();

    void Render();

    virtual int Run();

    static Application& GetInstance();

protected:
    HINSTANCE _hInstance;
    CControlManager* _controlManager;
    CGameManager* _gameManager;
    CFileManager* _fileManager;

    static Application* s_sharedApplication;
};
} // namespace three_kingdoms
