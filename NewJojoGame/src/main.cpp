#pragma comment(lib, "comctl32.lib")
//#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version = '6.0.0.0' processorArchitecture = '*' publicKeyToken = '6595b64144ccf1df' language = '*'\"")

#include <windows.h>
#include <CommCtrl.h>
#include <string>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "AppDelegate.h"

using namespace jojogame;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    AppDelegate app(hInstance);

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // Control ÃÊ±âÈ­
    INITCOMMONCONTROLSEX stylesStruct;
    stylesStruct.dwSize = sizeof(stylesStruct);
    stylesStruct.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&stylesStruct);

    return Application::getInstance()->run();
}
