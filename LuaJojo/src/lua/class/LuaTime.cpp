#include "LuaTime.h"
#include "../LuaJojo.h"
#include "../LuaTinker.h"

#include <windows.h>

#define WM_STOP_DELAY (WM_USER + 1)

namespace jojogame {
LuaTime* LuaTime::s_sharedTime = nullptr;

LuaTime* LuaTime::getInstance()
{
    if (s_sharedTime == nullptr)
    {
        s_sharedTime = new(std::nothrow) LuaTime();
    }
    return s_sharedTime;
}

LuaTime::LuaTime()
{
    const auto lua = LuaJojo::getInstance()->getLuaState();
    lua_tinker::class_add<LuaTime>(lua, "__time__");
    lua_tinker::class_def<LuaTime>(lua, "delay", &LuaTime::delay);
    lua_tinker::class_def<LuaTime>(lua, "stopDelay", &LuaTime::stopDelay);
}

LuaTime::~LuaTime()
{
    s_sharedTime = nullptr;
}

void LuaTime::delay(int time)
{
    const auto starttime = GetTickCount();
    MSG message;
    auto quit = false;

    for (;;)
    {
        if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
        {
            if (message.message == WM_QUIT)
            {
                quit = true;
                break;
            }
            if (message.message == WM_STOP_DELAY)
            {
                break;
            }
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        else
        {
            if ((GetTickCount() - starttime) >= static_cast<DWORD>(time))
            {
                break;
            }
        }
    }

    if (quit)
    {
        PostQuitMessage(0);
    }
}

void LuaTime::stopDelay()
{
    PostMessage(nullptr, WM_STOP_DELAY, 0, 0);
}

}
