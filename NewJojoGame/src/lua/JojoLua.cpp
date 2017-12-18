#include "JojoLua.h"

namespace jojogame {
	lua_State *JojoLua::sm_lua = luaL_newstate();

	JojoLua::JojoLua() {
		luaL_openlibs(sm_lua);
	}

	JojoLua::~JojoLua() {
		lua_close(sm_lua);
	}

	lua_State * JojoLua::getInstance()
	{
		return sm_lua;
	}
}
