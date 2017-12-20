#pragma once

#include "base/Ref.h"

#include <utility>
#include <vector>

namespace jojogame {
	class __declspec(dllexport) LuaWindow : public Ref {
	public:
		LuaWindow();

		int getDesktopWidth();
		int getDesktopHeight();

		static LuaWindow* getInstance();
	private:
		
		static LuaWindow* s_sharedWindow;
	};
}