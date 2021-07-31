#include "ButtonControl.h"

#include "ControlManager.h"
#include "WindowControl.h"

namespace three_kingdoms
{
	WNDPROC CButtonControl::s_originalProc = nullptr;

	LRESULT CALLBACK CButtonControl::OnControlProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
		TRACKMOUSEEVENT trackMouseEvent;

		switch (iMessage)
		{
		case WM_CREATE:
		{
			auto createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			void* lpParamCreate = createStruct->lpCreateParams;
			auto button = reinterpret_cast<CButtonControl*>(lpParamCreate);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(button));

			auto createEvent = button->GetCreateEvent();
			if (createEvent != LUA_NOREF)
			{
				CLuaTinker::GetLuaTinker().Call(createEvent, button);
			}
			break;
		}

		case WM_LBUTTONUP:
		{
			auto button = reinterpret_cast<CButtonControl*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			auto mouseLButtonUpEvent = button->GetMouseLButtonUpEvent();
			if (mouseLButtonUpEvent != LUA_NOREF)
			{
				CLuaTinker::GetLuaTinker().Call(mouseLButtonUpEvent,
					button,
					(int)wParam,
					GET_X_LPARAM(lParam),
					GET_Y_LPARAM(lParam));
			}

			button->_isPressed = false;
			break;
		}

		case WM_LBUTTONDOWN:
		{
			auto button = reinterpret_cast<CButtonControl*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			auto mouseLButtonDownEvent = button->GetMouseLButtonDownEvent();

			if (mouseLButtonDownEvent != LUA_NOREF)
			{
				CLuaTinker::GetLuaTinker().Call(mouseLButtonDownEvent,
					button,
					(int)wParam,
					GET_X_LPARAM(lParam),
					GET_Y_LPARAM(lParam));
			}

			button->_isPressed = true;

			break;
		}

		case WM_MOUSEMOVE:
		{
			auto button = reinterpret_cast<CButtonControl*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			auto mouseMoveEvent = button->GetMouseMoveEvent();
			if (mouseMoveEvent != LUA_NOREF)
			{
				CLuaTinker::GetLuaTinker().Call(mouseMoveEvent,
					button,
					(int)wParam,
					GET_X_LPARAM(lParam),
					GET_Y_LPARAM(lParam));
			}

			if (!button->_isHover)
			{
				trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
				trackMouseEvent.dwFlags = TME_HOVER;
				trackMouseEvent.hwndTrack = hWnd;
				trackMouseEvent.dwHoverTime = 10;
				TrackMouseEvent(&trackMouseEvent);
			}
			break;
		}

		case WM_MOUSEHOVER:
		{
			auto button = reinterpret_cast<CButtonControl*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			auto mouseHoverEvent = button->GetMouseEnterEvent();
			if (mouseHoverEvent != LUA_NOREF)
			{
				CLuaTinker::GetLuaTinker().Call(mouseHoverEvent, button);
			}

			button->_isHover = true;

			if (!button->IsTransparentBackground() || button->GetBorderWidth() > 0)
			{
				InvalidateRect(button->_hWnd, NULL, TRUE);
				UpdateWindow(button->_hWnd);
			}

			trackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
			trackMouseEvent.dwFlags = TME_LEAVE;
			trackMouseEvent.hwndTrack = hWnd;
			trackMouseEvent.dwHoverTime = 100;
			TrackMouseEvent(&trackMouseEvent);
			break;
		}

		case WM_MOUSELEAVE:
		{
			auto button = reinterpret_cast<CButtonControl*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			auto mouseLeaveEvent = button->GetMouseLeaveEvent();
			if (mouseLeaveEvent != LUA_NOREF)
			{
				CLuaTinker::GetLuaTinker().Call(mouseLeaveEvent, button);
			}

			button->_isPressed = false;
			button->_isHover = false;

			if (!button->IsTransparentBackground() || button->GetBorderWidth() > 0)
			{
				InvalidateRect(button->_hWnd, NULL, TRUE);
				UpdateWindow(button->_hWnd);
			}

			break;
		}

		case WM_DESTROY:
		{
			auto button = reinterpret_cast<CButtonControl*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
			auto destroyEvent = button->GetDestroyEvent();

			if (destroyEvent != LUA_NOREF)
			{
				CLuaTinker::GetLuaTinker().Call(destroyEvent, button);
			}
			break;
		}
		}
		return CallWindowProc(CButtonControl::GetOriginalProc(), hWnd, iMessage, wParam, lParam);
	}

	void CButtonControl::RegisterFunctions(lua_State* L)
	{
		LUA_BEGIN_CHILD(CButtonControl, "_ButtonControl", CWindowChildControl);

		LUA_METHOD(IsTransparentBackground);
		LUA_METHOD(GetText);
		LUA_METHOD(GetFont);
		LUA_METHOD(GetBackgroundColor);
		LUA_METHOD(GetBorderColor);
		LUA_METHOD(GetBorderWidth);
		LUA_METHOD(GetNormalBackgroundColor);
		LUA_METHOD(GetFocusedBackgroundColor);
		LUA_METHOD(GetPushedBackgroundColor);
		LUA_METHOD(GetNormalBorderColor);
		LUA_METHOD(GetFocusedBorderColor);
		LUA_METHOD(GetPushedBorderColor);
		LUA_METHOD(GetNormalTextColor);
		LUA_METHOD(GetFocusedTextColor);
		LUA_METHOD(GetPushedTextColor);
		LUA_METHOD(GetParentWindow);

		LUA_METHOD(SetTransparentBackground);
		LUA_METHOD(SetText);
		LUA_METHOD(SetParentWindow);
		LUA_METHOD(SetBackgroundColor);
		LUA_METHOD(SetFocusedBackgroundColor);
		LUA_METHOD(SetPushedBackgroundColor);
		LUA_METHOD(SetBorderColor);
		LUA_METHOD(SetFocusedBorderColor);
		LUA_METHOD(SetPushedBorderColor);
		LUA_METHOD(SetTextColor);
		LUA_METHOD(SetFocusedTextColor);
		LUA_METHOD(SetPushedTextColor);
		LUA_METHOD(SetBorderWidth);

		LUA_METHOD(Create);
		LUA_METHOD(Destroy);

		WNDCLASS wndClass;
		GetClassInfo(NULL, TEXT("button"), &wndClass);
		s_originalProc = wndClass.lpfnWndProc;
		wndClass.hInstance = CControlManager::GetInstance().GetHInstance();
		wndClass.lpfnWndProc = (WNDPROC)CButtonControl::OnControlProc;
		wndClass.lpszClassName = L"jojo_button";
		RegisterClass(&wndClass);
	}

	CButtonControl::CButtonControl()
		: _font(this)
	{
		//_style = WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_NOTIFY;
		_style = WS_TABSTOP | BS_OWNERDRAW | WS_CHILD;
		_type = L"button";
	}

	CButtonControl::~CButtonControl()
	{
	}

	bool CButtonControl::IsTransparentBackground() const
	{
		return _isTransparentBackground;
	}

	CTextFont* CButtonControl::GetFont()
	{
		return &_font;
	}

	std::wstring CButtonControl::GetText() const
	{
		return _text;
	}

	ButtonStateColor& CButtonControl::GetBackgroundColor()
	{
		return _backgroundColor;
	}

	ButtonStateColor& CButtonControl::GetBorderColor()
	{
		return _borderColor;
	}

	ButtonStateColor& CButtonControl::GetTextColor()
	{
		return _textColor;
	}

	COLORREF CButtonControl::GetNormalBackgroundColor() const
	{
		return _backgroundColor.normal;
	}

	COLORREF CButtonControl::GetFocusedBackgroundColor() const
	{
		return _backgroundColor.focused;
	}

	COLORREF CButtonControl::GetPushedBackgroundColor() const
	{
		return _backgroundColor.pushed;
	}

	COLORREF CButtonControl::GetNormalBorderColor() const
	{
		return _borderColor.normal;
	}

	COLORREF CButtonControl::GetFocusedBorderColor() const
	{
		return _borderColor.focused;
	}

	COLORREF CButtonControl::GetPushedBorderColor() const
	{
		return _borderColor.pushed;
	}

	COLORREF CButtonControl::GetNormalTextColor() const
	{
		return _textColor.normal;
	}

	COLORREF CButtonControl::GetFocusedTextColor() const
	{
		return _textColor.focused;
	}

	COLORREF CButtonControl::GetPushedTextColor() const
	{
		return _textColor.pushed;
	}

	int CButtonControl::GetBorderWidth() const
	{
		return _borderWidth;
	}

	void CButtonControl::SetTransparentBackground(bool isTransparentBackground)
	{
		if (_isTransparentBackground != isTransparentBackground)
		{
			_isTransparentBackground = isTransparentBackground;

			UpdateWindow(_hWnd);
		}
	}

	void CButtonControl::SetText(std::wstring text)
	{
		if (_text != text)
		{
			_text = text;

			if (_hWnd != nullptr)
			{
				SetWindowText(_hWnd, _text.c_str());
			}
		}
	}

	void CButtonControl::SetParentWindow(CWindowControl* parent)
	{
		if (_parentControl != parent)
		{
			_parentControl = parent;
		}
	}

	void CButtonControl::SetBackgroundColor(COLORREF color)
	{
		if (_backgroundColor.normal != color)
		{
			_backgroundColor.normal = color;
		}
	}

	void CButtonControl::SetFocusedBackgroundColor(COLORREF color)
	{
		if (_backgroundColor.focused != color)
		{
			_backgroundColor.focused = color;
		}
	}

	void CButtonControl::SetPushedBackgroundColor(COLORREF color)
	{
		if (_backgroundColor.pushed != color)
		{
			_backgroundColor.pushed = color;
		}
	}

	void CButtonControl::SetBorderColor(COLORREF color)
	{
		if (_borderColor.normal != color)
		{
			_borderColor.normal = color;
		}
	}

	void CButtonControl::SetFocusedBorderColor(COLORREF color)
	{
		if (_borderColor.focused != color)
		{
			_borderColor.focused = color;
		}
	}

	void CButtonControl::SetPushedBorderColor(COLORREF color)
	{
		if (_borderColor.pushed != color)

			_borderColor.pushed = color;
	}

	void CButtonControl::SetTextColor(COLORREF color)
	{
		if (_textColor.normal != color)
		{
			_textColor.normal = color;
		}
	}

	void CButtonControl::SetFocusedTextColor(COLORREF color)
	{
		if (_textColor.focused != color)
		{
			_textColor.focused = color;
		}
	}

	void CButtonControl::SetPushedTextColor(COLORREF color)
	{
		if (_textColor.pushed != color)
		{
			_textColor.pushed = color;
		}
	}

	void CButtonControl::SetBorderWidth(int width)
	{
		if (_borderWidth != width)
		{
			_borderWidth = width;
		}
	}

	bool CButtonControl::Create()
	{
		if (_parentControl != nullptr)
		{
			_hWnd = CreateWindow(L"jojo_button",
				_text.c_str(),
				_style,
				_position.x,
				_position.y,
				_size.cx,
				_size.cy,
				_parentControl->GetHWnd(),
				(HMENU)this,
				CControlManager::GetInstance().GetHInstance(),
				(LPVOID)this);
			_font.ResetFont();
		}

		return _hWnd != nullptr;
	}

	void CButtonControl::Show()
	{
		_isVisible = true;
		_style |= WS_VISIBLE;
		if (_hWnd != nullptr)
		{
			ShowWindow(_hWnd, TRUE);
			UpdateWindow(_parentControl->GetHWnd());
		}
	}

	WNDPROC CButtonControl::GetOriginalProc()
	{
		return s_originalProc;
	}
} // namespace jojogame