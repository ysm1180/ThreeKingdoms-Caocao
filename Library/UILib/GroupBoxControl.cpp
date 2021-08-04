#include "GroupBoxControl.h"

#include "ControlManager.h"

namespace three_kingdoms {
WNDPROC CGroupBoxControl::s_originalProc = nullptr;

LRESULT CGroupBoxControl::OnControlProc(HWND hWnd, UINT msg, WPARAM wParam,
                                        LPARAM lParam) {
  TRACKMOUSEEVENT trackMouseEvent;

  switch (msg) {
    case WM_CREATE: {
      auto createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
      void *lpParamCreate = createStruct->lpCreateParams;
      auto groupBoxControl =
          reinterpret_cast<CGroupBoxControl *>(lpParamCreate);
      SetWindowLongPtr(hWnd, GWLP_USERDATA,
                       reinterpret_cast<LONG_PTR>(groupBoxControl));

      auto createEvent = groupBoxControl->GetCreateEvent();
      if (createEvent != LUA_NOREF) {
        CLuaTinker::GetLuaTinker().Call(createEvent, groupBoxControl);
      }
      break;
    }

    case WM_LBUTTONUP: {
      auto groupBoxControl = reinterpret_cast<CGroupBoxControl *>(
          GetWindowLongPtr(hWnd, GWLP_USERDATA));
      auto mouseLButtonUpEvent = groupBoxControl->GetMouseLButtonUpEvent();
      if (mouseLButtonUpEvent != LUA_NOREF) {
        CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent, groupBoxControl,
                                        (int)wParam, GET_X_LPARAM(lParam),
                                        GET_Y_LPARAM(lParam));
      }

      break;
    }

    case WM_LBUTTONDOWN: {
      auto groupBoxControl = reinterpret_cast<CGroupBoxControl *>(
          GetWindowLongPtr(hWnd, GWLP_USERDATA));
      auto mouseLButtonUpEvent = groupBoxControl->GetMouseLButtonDownEvent();

      if (mouseLButtonUpEvent != LUA_NOREF) {
        CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent, groupBoxControl,
                                        (int)wParam, GET_X_LPARAM(lParam),
                                        GET_Y_LPARAM(lParam));
      }

      break;
    }

    case WM_MOUSEMOVE: {
      auto groupBoxControl = reinterpret_cast<CGroupBoxControl *>(
          GetWindowLongPtr(hWnd, GWLP_USERDATA));
      auto mouseMoveEvent = groupBoxControl->GetMouseMoveEvent();
      if (mouseMoveEvent != LUA_NOREF) {
        CLuaTinker::GetLuaTinker().Call(mouseMoveEvent, groupBoxControl,
                                        (int)wParam, GET_X_LPARAM(lParam),
                                        GET_Y_LPARAM(lParam));
      }

      if (!groupBoxControl->_isHover) {
        trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
        trackMouseEvent.dwFlags = TME_HOVER;
        trackMouseEvent.hwndTrack = hWnd;
        trackMouseEvent.dwHoverTime = 10;
        TrackMouseEvent(&trackMouseEvent);
      }
      break;
    }

    case WM_MOUSEHOVER: {
      auto groupBoxControl = reinterpret_cast<CGroupBoxControl *>(
          GetWindowLongPtr(hWnd, GWLP_USERDATA));
      auto mouseHoverEvent = groupBoxControl->GetMouseEnterEvent();
      if (mouseHoverEvent != LUA_NOREF) {
        CLuaTinker::GetLuaTinker().Call(mouseHoverEvent, groupBoxControl);
      }

      groupBoxControl->_isHover = true;

      trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
      trackMouseEvent.dwFlags = TME_LEAVE;
      trackMouseEvent.hwndTrack = hWnd;
      trackMouseEvent.dwHoverTime = 100;
      TrackMouseEvent(&trackMouseEvent);
      break;
    }

    case WM_MOUSELEAVE: {
      auto groupBoxControl = reinterpret_cast<CGroupBoxControl *>(
          GetWindowLongPtr(hWnd, GWLP_USERDATA));
      auto mouseLeaveEvent = groupBoxControl->GetMouseLeaveEvent();
      if (mouseLeaveEvent != LUA_NOREF) {
        CLuaTinker::GetLuaTinker().Call(mouseLeaveEvent, groupBoxControl);
      }

      groupBoxControl->_isHover = false;

      break;
    }

    case WM_DESTROY: {
      auto groupBoxControl = reinterpret_cast<CGroupBoxControl *>(
          GetWindowLongPtr(hWnd, GWLP_USERDATA));
      auto destroyEvent = groupBoxControl->GetDestroyEvent();

      if (destroyEvent != LUA_NOREF) {
        CLuaTinker::GetLuaTinker().Call(destroyEvent, groupBoxControl);
      }

      break;
    }
  }
  return CallWindowProc(CGroupBoxControl::GetOriginalProc(), hWnd, msg, wParam,
                        lParam);
}

void CGroupBoxControl::RegisterFunctions(lua_State *L) {
  LUA_BEGIN_CHILD(CGroupBoxControl, "_GroupBoxControl", CWindowChildControl);

  LUA_METHOD(GetText);
  LUA_METHOD(GetFont);

  LUA_METHOD(SetText);

  LUA_METHOD(Create);
  LUA_METHOD(Destroy);

  WNDCLASS wndClass;
  GetClassInfo(NULL, TEXT("Button"), &wndClass);
  s_originalProc = wndClass.lpfnWndProc;
  wndClass.hInstance = CControlManager::GetInstance().GetHInstance();
  wndClass.lpfnWndProc = (WNDPROC)CGroupBoxControl::OnControlProc;
  wndClass.lpszClassName = L"jojo_group";
  RegisterClass(&wndClass);
}

CGroupBoxControl::CGroupBoxControl() : _font(this) {
  _style = WS_CHILD | BS_GROUPBOX;
  _type = L"groupbox";
}

CGroupBoxControl::~CGroupBoxControl() {}

std::wstring CGroupBoxControl::GetText() { return _text; }

CTextFont *CGroupBoxControl::GetFont() { return &_font; }

void CGroupBoxControl::SetText(std::wstring text) {
  if (_text != text) {
    _text = text;

    if (_hWnd != nullptr) {
      InvalidateRect(_hWnd, nullptr, TRUE);
      UpdateWindow(_hWnd);
    }
  }
}

bool CGroupBoxControl::Create() {
  if (_parentControl != nullptr) {
    _hWnd =
        CreateWindow(L"jojo_group", L"", _style, _position.x, _position.y,
                     _size.cx, _size.cy, _parentControl->GetHWnd(), (HMENU)this,
                     CControlManager::GetInstance().GetHInstance(), this);

    _font.ResetFont();
  }

  return _hWnd != nullptr;
}

WNDPROC CGroupBoxControl::GetOriginalProc() { return s_originalProc; }
}  // namespace three_kingdoms