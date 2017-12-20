#include "LuaColor.h"
#include "../LuaJojo.h"
#include "../LuaTinker.h"

namespace jojogame {
	LuaColor* LuaColor::s_sharedColor = nullptr;

	LuaColor * LuaColor::getInstance()
	{
		if (s_sharedColor == nullptr) {
			s_sharedColor = new (std::nothrow) LuaColor();
		}
		return s_sharedColor;
	}

	LuaColor::LuaColor()
	{
		lua_State* lua = LuaJojo::getInstance()->getLuaState();
		lua_tinker::class_add<LuaColor>(lua, "__color__");
		lua_tinker::class_def<LuaColor>(lua, "rgb", &LuaColor::rgb);
		lua_tinker::class_def<LuaColor>(lua, "color", &LuaColor::color);
	}

	LuaColor::~LuaColor()
	{
		s_sharedColor = nullptr;
	}

	COLORREF LuaColor::rgb(int r, int g, int b)
	{
		return RGB(r, g, b);
	}
	COLORREF LuaColor::color(wchar_t *colorName)
	{
		COLORREF c = RGB(0, 0, 0);
		if (wcscmp(colorName, L"black") == 0) {
			c = RGB(0, 0, 0);
		}
		else if (wcscmp(colorName, L"red") == 0) {
			c = RGB(0xff, 0, 0);
		}
		else if (wcscmp(colorName, L"white") == 0) {
			c = RGB(0xff, 0xff, 0xff);
		}

		return c;
	}
}