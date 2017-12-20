#include "LuaJojo.h"
#include "LuaTinker.h"

#include "class/LuaWindow.h"
#include "class/LuaGame.h"
#include "class/LuaTime.h"
#include "class/LuaColor.h"
#include "control/LuaControl.h"

namespace jojogame {
	// Shared lua class pointer
	LuaJojo* LuaJojo::s_sharedJojoLua = nullptr;

	LuaJojo* LuaJojo::getInstance()
	{
		if (s_sharedJojoLua == nullptr) {
			s_sharedJojoLua = new (std::nothrow) LuaJojo();
		}
		return s_sharedJojoLua;
	}

	LuaJojo::LuaJojo() {
		_lua = luaL_newstate();
		luaL_openlibs(_lua);

		_window = LuaWindow::getInstance();
		_game = LuaGame::getInstance();
		_time = LuaTime::getInstance();
		_color = LuaColor::getInstance();
	}

	LuaJojo::~LuaJojo() {
		lua_close(_lua);
		s_sharedJojoLua = nullptr;
	}

	void LuaJojo::init()
	{
		lua_State* lua = LuaJojo::getInstance()->getLuaState();
		
		lua_tinker::set(lua, "Window", _window);
		lua_tinker::set(lua, "Game", _game);
		lua_tinker::set(lua, "Time", _time);
		lua_tinker::set(lua, "Color", _color);

		LuaControl::registerToLua();
	}

	lua_State* LuaJojo::getLuaState()
	{
		return _lua;
	}

	void LuaJojo::end() {
		_window->release();
		_game->release();
		_time->release();
		_color->release();

		release();
	}
}
