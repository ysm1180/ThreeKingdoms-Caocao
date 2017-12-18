#pragma once

#include <windows.h>
extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include "LuaTinker.h"

namespace jojogame {
	class JojoLua {
	public:
		JojoLua();
		~JojoLua();

		static lua_State* getInstance();
	protected:
		static lua_State* sm_lua;
	};
}