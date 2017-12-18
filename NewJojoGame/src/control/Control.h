#pragma once

#include <windows.h>

namespace jojogame {
	class Control {
	public:
		Control(HINSTANCE);
		~Control();

	protected:
		static HINSTANCE sm_hInstance;
	};
}