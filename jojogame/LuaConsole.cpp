#include "LuaConsole.h"


namespace jojogame {
std::once_flag CLuaConsole::s_onceFlag;
std::unique_ptr<CLuaConsole> CLuaConsole::s_sharedLuaConsole;

INT_PTR __stdcall DialogProc(HWND hDialog, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
                case IDOK:
                    SendMessage(hDialog, WM_CLOSE, 0, 0);
                    return TRUE;
            }
            break;

        case WM_CLOSE:
            DestroyWindow(hDialog);
            return TRUE;

        case WM_DESTROY:
            return TRUE;
    }

    return FALSE;
}

CLuaConsole::CLuaConsole()
{
}

CLuaConsole::~CLuaConsole()
{
}

HWND CLuaConsole::GetHWnd()
{
    return _hWnd;
}

void CLuaConsole::SetHInstance(HINSTANCE hInstance)
{
    _hInstance = hInstance;
}

void CLuaConsole::Output(std::wstring msg)
{
    this->AppendOutput(msg);
}

void CLuaConsole::AppendOutput(std::wstring msg)
{
    HWND output = GetDlgItem(_hWnd, IDC_LOG);
    int oldLen = GetWindowTextLength(output);
    int appendLen = msg.length() + 2; // + 2 : \r\n
    int newLen = oldLen + appendLen;

    //crop
    if (newLen > OUTPUT_MAX_LENGTH)
    {
        SendMessage(output, EM_SETSEL, 0, appendLen);
        SendMessage(output, EM_REPLACESEL, 0, (LPARAM) "");
    }

    SendMessage(output, EM_SETSEL, 0, -1);
    SendMessage(output, EM_SETSEL, -1, -1);
    SendMessage(output, EM_REPLACESEL, 0, (LPARAM) "\n");

    SendMessage(output, EM_SETSEL, 0, -1);
    SendMessage(output, EM_SETSEL, -1, -1);
    SendMessage(output, EM_REPLACESEL, 0, (LPARAM) msg.c_str());
}

void CLuaConsole::Create()
{
    CConsoleOutput::RegisterConsole(this);
    _hWnd = CreateDialogParam(_hInstance, MAKEINTRESOURCE(IDD_LUACONSOLE), 0, DialogProc, 0);
}

CLuaConsole& CLuaConsole::GetInstance()
{
    std::call_once(s_onceFlag, []
    {
        s_sharedLuaConsole.reset(new CLuaConsole);
    });

    return *s_sharedLuaConsole.get();
}

void CLuaConsole::SetDebugFlag(bool flag)
{
    HWND hWnd = CLuaConsole::GetInstance().GetHWnd();

    if (hWnd != nullptr)
    {
        if (flag)
        {
            ShowWindow(hWnd, TRUE);
        } else
        {
            ShowWindow(hWnd, FALSE);
        }
    }
}


}
