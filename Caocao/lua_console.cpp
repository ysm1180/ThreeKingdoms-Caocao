#include "lua_console.h"

#include "resource.h"

namespace three_kingdoms {
std::once_flag LuaConsole::s_onceFlag;
std::unique_ptr<LuaConsole> LuaConsole::s_sharedLuaConsole;

INT_PTR __stdcall DialogProc(HWND hDialog, UINT message, WPARAM wParam,
                             LPARAM lParam) {
  switch (message) {
    case WM_COMMAND:
      switch (LOWORD(wParam)) {
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

LuaConsole::LuaConsole() {}

LuaConsole::~LuaConsole() {
  if (_hWnd) {
    DestroyWindow(_hWnd);
    _hWnd = nullptr;
  }
}

HWND LuaConsole::GetHWnd() { return _hWnd; }

void LuaConsole::Output(std::wstring msg) { this->AppendOutput(msg); }

void LuaConsole::AppendOutput(std::wstring msg) {
  HWND output = GetDlgItem(_hWnd, IDC_LOG);
  int oldLen = GetWindowTextLength(output);
  int appendLen = msg.length() + 2;  // + 2 : \r\n
  int newLen = oldLen + appendLen;

  // crop
  if (newLen > OUTPUT_MAX_LENGTH) {
    SendMessage(output, EM_SETSEL, 0, appendLen);
    SendMessage(output, EM_REPLACESEL, 0, (LPARAM) "");
  }

  SendMessage(output, EM_SETSEL, 0, -1);
  SendMessage(output, EM_SETSEL, -1, -1);
  SendMessage(output, EM_REPLACESEL, 0,
              reinterpret_cast<LPARAM>((TCHAR *)L"\r\n"));

  SendMessage(output, EM_SETSEL, 0, -1);
  SendMessage(output, EM_SETSEL, -1, -1);
  SendMessage(output, EM_REPLACESEL, 0, (LPARAM)msg.c_str());

  OutputDebugString(msg.c_str());
  OutputDebugString(L"\r\n");
}

void LuaConsole::Create(HINSTANCE hInstance) {
  Console::RegisterConsole(this);
  _hWnd = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_LUACONSOLE), 0,
                            DialogProc, 0);
}

LuaConsole &LuaConsole::GetInstance() {
  std::call_once(s_onceFlag, [] {
    s_sharedLuaConsole = std::make_unique<three_kingdoms::LuaConsole>();
  });

  return *s_sharedLuaConsole;
}

void LuaConsole::SetDebugFlag(bool flag) {
  HWND hWnd = LuaConsole::GetInstance().GetHWnd();

  if (hWnd != nullptr) {
    if (flag) {
      ShowWindow(hWnd, TRUE);
    } else {
      ShowWindow(hWnd, FALSE);
    }
  }
}
}  // namespace three_kingdoms