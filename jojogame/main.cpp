#pragma comment(lib, "comctl32.lib")
#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version = '6.0.0.0' processorArchitecture = '*' publicKeyToken = '6595b64144ccf1df' language = '*'\"")

#include "Application.h"


#include <CommCtrl.h>
#include <string>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
#endif


using namespace jojogame;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    Application app(hInstance);

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    INITCOMMONCONTROLSEX stylesStruct;
    stylesStruct.dwSize = sizeof(stylesStruct);
    stylesStruct.dwICC = ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&stylesStruct);

    return app.Run();
}