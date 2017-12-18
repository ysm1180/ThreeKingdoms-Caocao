#pragma once

#include <windows.h>

namespace jojogame {
	HWND GetDesktopHwnd();
	int GetDesktopWidth();
	int GetDesktopHeight();
	COLORREF ToRGB(int r, int g, int b);
	void Delay(int time);
	void StopDelay();
	void Quit();
}