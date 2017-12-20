#include "AppDelegate.h"
#include "lua/LuaJojo.h"

namespace jojogame {
	AppDelegate::AppDelegate(HINSTANCE hInstance)
		: Application(hInstance)
	{
	}

	AppDelegate::~AppDelegate()
	{
	}

	int AppDelegate::run()
	{
		MSG message;
		LuaJojo* luaJojo = LuaJojo::getInstance();

		while (GetMessage(&message, 0, 0, 0)) {
			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		return (int)message.wParam;
	}
}