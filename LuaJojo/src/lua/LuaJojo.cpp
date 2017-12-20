#include "LuaJojo.h"
#include "LuaTinker.h"
#include "class/LuaWindow.h"

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
	}

	LuaJojo::~LuaJojo() {
		lua_close(_lua);
		s_sharedJojoLua = nullptr;
	}

	void LuaJojo::init()
	{
		lua_State* lua = LuaJojo::getInstance()->getLuaState();
		lua_tinker::set(lua, "Window", _window);
	}

	lua_State* LuaJojo::getLuaState()
	{
		return _lua;
	}

	void LuaJojo::end() {
		delete this;
	}
}
