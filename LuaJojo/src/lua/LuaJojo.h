#pragma once

#include <windows.h>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

namespace jojogame {
	class LuaWindow;

	class __declspec(dllexport) LuaJojo {
	public:
		LuaJojo();
		~LuaJojo();

		void init();

		lua_State* getLuaState();
		
		void end();

		static LuaJojo* getInstance();
	protected:
		lua_State* _lua;
		LuaWindow* _window;

		static LuaJojo* s_sharedJojoLua;
	};
}