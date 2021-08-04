#include "CheckboxControl.h"

#include "ControlManager.h"

namespace three_kingdoms {
WNDPROC CCheckboxControl::s_originalProc = nullptr;

LRESULT CCheckboxControl::OnControlProc(HWND hWnd, UINT msg, WPARAM wParam,
                                        LPARAM lParam) {
  TRACKMOUSEEVENT trackMouseEvent;

  switch (msg) {
    case WM_CREATE: {
      auto createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
      void *lpParamCreate = createStruct->lpCreateParams;
      auto checkBoxControl =
          reinterpret_cast<CCheckboxControl *>(lpParamCreate);
      SetWindowLongPtr(hWnd, GWLP_USERDATA,
                       reinterpret_cast<LONG_PTR>(checkBoxControl));

      auto createEvent = checkBoxControl->GetCreateEvent();
      if (createEvent != LUA_NOREF) {
        CLuaTinker::GetLuaTinker().Call(createEvent, checkBoxControl);
      }

      checkBoxControl->_theme = OpenThemeData(hWnd, L"Button");
      SetWindowTheme(hWnd, L"", L"");
      break;
    }

    case WM_LBUTTONUP: {
      auto checkBoxControl = reinterpret_cast<CCheckboxControl *>(
          GetWindowLongPtr(hWnd, GWLP_USERDATA));
      auto mouseLButtonUpEvent = checkBoxControl->GetMouseLButtonUpEvent();
      if (mouseLButtonUpEvent != LUA_NOREF) {
        CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent, checkBoxControl,
                                        (int)wParam, GET_X_LPARAM(lParam),
                                        GET_Y_LPARAM(lParam));
      }

      checkBoxControl->_isPressed = false;

      InvalidateRect(checkBoxControl->_hWnd, NULL, TRUE);
      UpdateWindow(checkBoxControl->_hWnd);
      break;
    }

    case WM_LBUTTONDOWN: {
      auto checkBoxControl = reinterpret_cast<CCheckboxControl *>(
          GetWindowLongPtr(hWnd, GWLP_USERDATA));
      auto mouseLButtonUpEvent = checkBoxControl->GetMouseLButtonDownEvent();

      checkBoxControl->_isPressed = true;

      if (mouseLButtonUpEvent != LUA_NOREF) {
        CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent, checkBoxControl,
                                        (int)wParam, GET_X_LPARAM(lParam),
                                        GET_Y_LPARAM(lParam));
      }

      InvalidateRect(checkBoxControl->_hWnd, NULL, TRUE);
      UpdateWindow(checkBoxControl->_hWnd);

      break;
    }

    case WM_MOUSEMOVE: {
      auto checkBoxControl = reinterpret_cast<CCheckboxControl *>(
          GetWindowLongPtr(hWnd, GWLP_USERDATA));
      auto mouseMoveEvent = checkBoxControl->GetMouseMoveEvent();
      if (mouseMoveEvent != LUA_NOREF) {
        CLuaTinker::GetLuaTinker().Call(mouseMoveEvent, checkBoxControl,
                                        (int)wParam, GET_X_LPARAM(lParam),
                                        GET_Y_LPARAM(lParam));
      }

      if (!checkBoxControl->_isHover) {
        trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
        trackMouseEvent.dwFlags = TME_HOVER;
        trackMouseEvent.hwndTrack = hWnd;
        trackMouseEvent.dwHoverTime = 10;
        TrackMouseEvent(&trackMouseEvent);
      }
      break;
    }

    case WM_MOUSEHOVER: {
      auto checkBoxControl = reinterpret_cast<CCheckboxControl *>(
          GetWindowLongPtr(hWnd, GWLP_USERDATA));
      auto mouseHoverEvent = checkBoxControl->GetMouseEnterEvent();
      if (mouseHoverEvent != LUA_NOREF) {
        CLuaTinker::GetLuaTinker().Call(mouseHoverEvent, checkBoxControl);
      }

      checkBoxControl->_isHover = true;

      trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
      trackMouseEvent.dwFlags = TME_LEAVE;
      trackMouseEvent.hwndTrack = hWnd;
      trackMouseEvent.dwHoverTime = 100;
      TrackMouseEvent(&trackMouseEvent);

      InvalidateRect(checkBoxControl->_hWnd, NULL, TRUE);
      UpdateWindow(checkBoxControl->_hWnd);
      break;
    }

    case WM_MOUSELEAVE: {
      auto checkBoxControl = reinterpret_cast<CCheckboxControl *>(
          GetWindowLongPtr(hWnd, GWLP_USERDATA));
      auto mouseLeaveEvent = checkBoxControl->GetMouseLeaveEvent();
      if (mouseLeaveEvent != LUA_NOREF) {
        CLuaTinker::GetLuaTinker().Call(mouseLeaveEvent, checkBoxControl);
      }

      checkBoxControl->_isPressed = false;
      checkBoxControl->_isHover = false;

      InvalidateRect(checkBoxControl->_hWnd, NULL, TRUE);
      UpdateWindow(checkBoxControl->_hWnd);

      break;
    }

    case WM_DESTROY: {
      auto checkBoxControl = reinterpret_cast<CCheckboxControl *>(
          GetWindowLongPtr(hWnd, GWLP_USERDATA));
      auto destroyEvent = checkBoxControl->GetDestroyEvent();

      if (destroyEvent != LUA_NOREF) {
        CLuaTinker::GetLuaTinker().Call(destroyEvent, checkBoxControl);
      }
      break;
    }
  }
  return CallWindowProc(CCheckboxControl::GetOriginalProc(), hWnd, msg, wParam,
                        lParam);
}

void CCheckboxControl::RegisterFunctions(lua_State *L) {
  LUA_BEGIN_CHILD(CCheckboxControl, "_CheckboxControl", CWindowChildControl);

  LUA_METHOD(IsChecked);
  LUA_METHOD(GetText);

  LUA_METHOD(SetText);
  LUA_METHOD(SetChecked);

  LUA_METHOD(Create);
  LUA_METHOD(Destroy);

  WNDCLASS wndClass;
  GetClassInfo(NULL, TEXT("Button"), &wndClass);
  s_originalProc = wndClass.lpfnWndProc;
  wndClass.hInstance = CControlManager::GetInstance().GetHInstance();
  wndClass.lpfnWndProc = (WNDPROC)CCheckboxControl::OnControlProc;
  wndClass.lpszClassName = L"jojo_check";
  RegisterClass(&wndClass);
}

CCheckboxControl::CCheckboxControl() : _font(this) {
  _style = WS_TABSTOP | WS_CHILD | BS_CHECKBOX | BS_AUTOCHECKBOX;
  _type = L"checkbox";
}

CCheckboxControl::~CCheckboxControl() {}

bool CCheckboxControl::IsChecked() {
  if (_hWnd != nullptr) {
    return SendMessage(_hWnd, BM_GETCHECK, 0, 0) == BST_CHECKED;
  } else {
    return _isChecked;
  }
}

std::wstring CCheckboxControl::GetText() { return _text; }

CTextFont *CCheckboxControl::GetFont() { return &_font; }

HTHEME CCheckboxControl::GetTheme() { return _theme; }

void CCheckboxControl::SetText(std::wstring text) {
  if (_text != text) {
    _text = text;

    if (_hWnd != nullptr) {
      SetWindowText(_hWnd, _text.c_str());
    }
  }
}

void CCheckboxControl::SetChecked(bool checked) {
  _isChecked = checked;
  if (_hWnd != nullptr) {
    if (checked) {
      SendMessage(_hWnd, BM_SETCHECK, BST_CHECKED, 0);
    } else {
      SendMessage(_hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
    }
  }
}

bool CCheckboxControl::Create() {
  if (_parentControl != nullptr) {
    _hWnd = CreateWindow(L"jojo_check", _text.c_str(), _style, _position.x,
                         _position.y, _size.cx, _size.cy,
                         _parentControl->GetHWnd(), (HMENU)this,
                         CControlManager::GetInstance().GetHInstance(), this);

    if (_isChecked) {
      SendMessage(_hWnd, BM_SETCHECK, BST_CHECKED, 0);
    } else {
      SendMessage(_hWnd, BM_SETCHECK, BST_UNCHECKED, 0);
    }

    _font.ResetFont();
  }

  return _hWnd != nullptr;
}

WNDPROC CCheckboxControl::GetOriginalProc() { return s_originalProc; }
}  // namespace three_kingdoms