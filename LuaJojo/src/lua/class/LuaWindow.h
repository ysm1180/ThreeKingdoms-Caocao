#pragma once

#include <utility>
#include <vector>

namespace jojogame {
	class __declspec(dllexport) LuaWindow {
	public:
		LuaWindow();

		int getDesktopWidth();
		int getDesktopHeight();

		static LuaWindow* getInstance();
	private:
		
		static LuaWindow* s_sharedWindow;
	};
}