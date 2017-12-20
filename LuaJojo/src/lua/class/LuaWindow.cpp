#include "LuaWindow.h"
#include "../LuaJojo.h"
#include "../LuaTinker.h"

#include <windows.h>

namespace jojogame {
	LuaWindow* LuaWindow::s_sharedWindow = nullptr;
	
	LuaWindow* LuaWindow::getInstance()
	{
		if (s_sharedWindow == nullptr) {
			s_sharedWindow = new (std::nothrow) LuaWindow();
		}
		return s_sharedWindow;
	}

	LuaWindow::LuaWindow() 
	{
		lua_State* lua = LuaJojo::getInstance()->getLuaState();
		lua_tinker::class_add<LuaWindow>(lua, "__window__");
		lua_tinker::class_def<LuaWindow>(lua, "getDesktopWidth", &LuaWindow::getDesktopWidth);
		lua_tinker::class_def<LuaWindow>(lua, "getDesktopHeight", &LuaWindow::getDesktopHeight);

	}

	LuaWindow::~LuaWindow()
	{
		s_sharedWindow = nullptr;
	}

	int LuaWindow::getDesktopWidth()
	{
		RECT rect;
		GetWindowRect(GetDesktopWindow(), &rect);
		return rect.right - rect.left;
	}

	int LuaWindow::getDesktopHeight()
	{
		RECT rect;
		GetWindowRect(GetDesktopWindow(), &rect);
		return rect.bottom - rect.top;
	}
}
