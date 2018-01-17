#include "LuaJojo.h"
#include "LuaTinker.h"

#include "class/LuaWindow.h"
#include "class/LuaGame.h"
#include "class/LuaTime.h"
#include "class/LuaColor.h"

#include "control/LuaControl.h"
#include "control/LuaForm.h"
#include "control/LuaMovie.h"
#include "control/LuaMenu.h"

namespace jojogame {
// Shared lua class pointer
LuaJojo* LuaJojo::s_sharedJojoLua = nullptr;

LuaJojo* LuaJojo::getInstance()
{
    if (s_sharedJojoLua == nullptr)
    {
        s_sharedJojoLua = new(std::nothrow) LuaJojo();
        s_sharedJojoLua->init();
    }
    return s_sharedJojoLua;
}

LuaJojo::LuaJojo()
{
    _lua = luaL_newstate();
    luaL_openlibs(_lua);
}

LuaJojo::~LuaJojo()
{
    lua_close(_lua);
    s_sharedJojoLua = nullptr;
}

void LuaJojo::init()
{
    _window = LuaWindow::getInstance();
    _game = LuaGame::getInstance();
    _time = LuaTime::getInstance();
    _color = LuaColor::getInstance();
    lua_tinker::set(_lua, "Window", _window);
    lua_tinker::set(_lua, "Game", _game);
    lua_tinker::set(_lua, "Time", _time);
    lua_tinker::set(_lua, "Color", _color);

    LuaControl::registerToLua();
    LuaForm::registerToLua();
    LuaMovie::registerToLua();
    LuaMenubar::registerToLua();
    LuaMenuItem::registerToLua();
}

lua_State* LuaJojo::getLuaState()
{
    return _lua;
}

void LuaJojo::run(const char* fileName)
{
    lua_tinker::dofile(_lua, fileName);
}

void LuaJojo::end()
{
    _window->release();
    _game->release();
    _time->release();
    _color->release();
    _menuManager->release();

    release();
}
}
