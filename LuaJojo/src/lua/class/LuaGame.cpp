#include "LuaGame.h"
#include "../LuaJojo.h"
#include "../LuaTinker.h"

#include <windows.h>

namespace jojogame {
	LuaGame* LuaGame::s_sharedGame = nullptr;

	LuaGame* LuaGame::getInstance()
	{
		if (s_sharedGame == nullptr) {
			s_sharedGame = new (std::nothrow) LuaGame();
		}
		return s_sharedGame;
	}

	LuaGame::LuaGame()
	{
		lua_State* lua = LuaJojo::getInstance()->getLuaState();
		lua_tinker::class_add<LuaGame>(lua, "__game__");
		lua_tinker::class_def<LuaGame>(lua, "quit", &LuaGame::quit);
	}

	LuaGame::~LuaGame()
	{
		s_sharedGame = nullptr;
	}

	void LuaGame::quit()
	{
		PostQuitMessage(0);
	}
	
	
}

