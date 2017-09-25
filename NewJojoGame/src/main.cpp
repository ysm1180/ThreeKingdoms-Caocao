#pragma comment(lib, "comctl32.lib")
//#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version = '6.0.0.0' processorArchitecture = '*' publicKeyToken = '6595b64144ccf1df' language = '*'\"")

#include <windows.h>
#include <commctrl.h>
#include <string>


HINSTANCE g_hInstance;

namespace jojogame {
	namespace usercontrol {
		LRESULT CALLBACK procForm(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
			switch (iMessage) {
			case WM_CREATE:
			{
				return 0;
			}

			case WM_SETCURSOR:
			{
				break;
			}

			case WM_NOTIFY:
				break;


			case WM_LBUTTONUP:
			{
				break;
			}

			case WM_ACTIVATEAPP:
			{
				if (wParam == TRUE) {
				}
			}

			case WM_COMMAND:
			{
				break;
			}

			case WM_SIZE:
			{
				break;
			}

			case WM_MOVE:
			{
				break;
			}

			case WM_CLOSE:
			{
				break;
			}

			case WM_DESTROY:
			{
				return 0;
			}

			case WM_PAINT:
			{
				return 0;
			}

			case WM_ERASEBKGND:
			{
				return TRUE;
			}
			}

			return DefWindowProc(hwnd, iMessage, wParam, lParam);
		}

		void registerFormClass() {
			// WNDCLASS 초기화
			WNDCLASS wndClass;
			wndClass.cbClsExtra = 0;
			wndClass.cbWndExtra = 0;
			wndClass.hbrBackground = (HBRUSH)GetSysColorBrush(COLOR_3DFACE);
			wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wndClass.hInstance = g_hInstance;
			wndClass.lpfnWndProc = (WNDPROC)jojogame::usercontrol::procForm;
			wndClass.lpszClassName = "jojo_form";
			wndClass.lpszMenuName = nullptr;
			wndClass.style = CS_HREDRAW | CS_VREDRAW;
			RegisterClass(&wndClass);
		}

		void registerJojoControl() {
			registerFormClass();
		}

	} // namespace usercontrol
} // namespace jojogame

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
	MSG Message;
	g_hInstance = hInstance;

	// Control 초기화
	INITCOMMONCONTROLSEX stylesStruct;
	stylesStruct.dwSize = sizeof(stylesStruct);
	stylesStruct.dwICC = ICC_STANDARD_CLASSES;
	InitCommonControlsEx(&stylesStruct);

	// Jojo Control 클래스 등록
	jojogame::usercontrol::registerJojoControl();


	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}