#pragma once

#include "Application.h"

namespace jojogame {
	class AppDelegate : public Application {
	public:
		AppDelegate(HINSTANCE);
		virtual ~AppDelegate();

		virtual int run();

	private:
	};
}