#pragma once

#include "base/Ref.h"

#include <windows.h>

namespace jojogame {
	class __declspec(dllexport) LuaColor : public Ref {
	public:
		LuaColor();
		~LuaColor();

		COLORREF rgb(int, int, int);
		COLORREF color(wchar_t *);

		static LuaColor* getInstance();
	private:
		static LuaColor* s_sharedColor;
	};
}