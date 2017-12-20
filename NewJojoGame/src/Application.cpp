#include "Application.h"
#include "lua/LuaJojo.h"

namespace jojogame {
	Application* Application::s_sharedApplication = nullptr;

	Application* Application::getInstance()
	{
		return s_sharedApplication;
	}

	Application::Application(HINSTANCE instance)
	{
		_hInstance = instance;
		s_sharedApplication = this;
	}

	Application::~Application()
	{
		s_sharedApplication = nullptr;
	}

	int Application::run()
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