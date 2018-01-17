#include "AppDelegate.h"
#include "LuaLib/LuaJojo.h"

namespace jojogame {
AppDelegate::AppDelegate(HINSTANCE hInstance)
    : Application(hInstance)
{
}

AppDelegate::~AppDelegate()
= default;

int AppDelegate::run()
{
    MSG message;
    auto luaJojo = LuaJojo::getInstance();

    luaJojo->run("main.lua");
    while (GetMessage(&message, 0, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    luaJojo->end();

    return (int)message.wParam;
}
}
