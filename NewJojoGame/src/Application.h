#pragma once

#include <windows.h>

namespace jojogame {
	class Application {
	public:
		Application(HINSTANCE);
		~Application();

		int run();

		static Application* getInstance();
	protected:
		HINSTANCE _hInstance;
		static Application* s_sharedApplication;
	};
}