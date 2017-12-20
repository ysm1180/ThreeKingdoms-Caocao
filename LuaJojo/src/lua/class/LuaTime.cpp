#include "LuaTime.h"
#include "../LuaJojo.h"
#include "../LuaTinker.h"

#define WM_STOP_DELAY (WM_USER + 1)

namespace jojogame {
	LuaTime* LuaTime::s_sharedTime = nullptr;

	LuaTime* LuaTime::getInstance()
	{
		if (s_sharedTime == nullptr) {
			s_sharedTime = new (std::nothrow) LuaTime();
		}
		return s_sharedTime;
	}

	LuaTime::LuaTime()
	{
		lua_State* lua = LuaJojo::getInstance()->getLuaState();
		lua_tinker::class_add<LuaTime>(lua, "__time__");
		lua_tinker::class_def<LuaTime>(lua, "delay", &LuaTime::delay);
		lua_tinker::class_def<LuaTime>(lua, "stopDelay", &LuaTime::stopDelay);
	}

	LuaTime::~LuaTime()
	{
		s_sharedTime = nullptr;
	}

	void LuaTime::delay(int time)
	{
		DWORD starttime = GetTickCount();
		MSG Message;
		bool isQuit = false;

		for (;;) {
			if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
				if (Message.message == WM_QUIT) {
					isQuit = true;
					break;
				}
				else if (Message.message == WM_STOP_DELAY) {
					break;
				}
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}
			else {
				if ((GetTickCount() - starttime) >= (DWORD)time) {
					break;
				}
			}
		}

		if (isQuit) {
			PostQuitMessage(0);
		}
	}

	void LuaTime::stopDelay()
	{
		PostMessage(NULL, WM_STOP_DELAY, 0, 0);
	}
	
}

