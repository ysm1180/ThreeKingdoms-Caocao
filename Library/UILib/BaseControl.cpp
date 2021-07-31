#include "BaseControl.h"
#include "ControlManager.h"
#include "LuaLib\LuaTinker.h"
#include <CommCtrl.h>

namespace three_kingdoms
{
	void CBaseControl::RegisterFunctions(lua_State* L)
	{
		LUA_BEGIN(CBaseControl, "_BaseControl");

		LUA_METHOD(IsEnabled);
		LUA_METHOD(IsVisible);
		LUA_METHOD(GetWidth);
		LUA_METHOD(GetHeight);
		LUA_METHOD(GetX);
		LUA_METHOD(GetY);
		LUA_METHOD(GetUserData);

		LUA_METHOD(SetEnabled);
		LUA_METHOD(SetWidth);
		LUA_METHOD(SetHeight);
		LUA_METHOD(SetX);
		LUA_METHOD(SetY);
		LUA_METHOD(SetCreateEvent);
		LUA_METHOD(SetDestroyEvent);
		LUA_METHOD(SetMouseLButtonUpEvent);
		LUA_METHOD(SetMouseLButtonDownEvent);
		LUA_METHOD(SetMouseMoveEvent);
		LUA_METHOD(SetMouseEnterEvent);
		LUA_METHOD(SetMouseLeaveEvent);
		LUA_METHOD(SetUserData);

		LUA_METHOD(Refresh);
		LUA_METHOD(RefreshRegion);

		LUA_METHOD(Show);
		LUA_METHOD(Hide);
	}

	CBaseControl::CBaseControl()
	{
	}

	CBaseControl::~CBaseControl()
	{
		if (_createEvent != LUA_NOREF)
		{
			luaL_unref(CLuaTinker::GetLuaTinker().GetLuaState(), LUA_REGISTRYINDEX, _createEvent);
		}
		if (_destroyEvent != LUA_NOREF)
		{
			luaL_unref(CLuaTinker::GetLuaTinker().GetLuaState(), LUA_REGISTRYINDEX, _destroyEvent);
		}
		if (_mouseLButtonUpEvent != LUA_NOREF)
		{
			luaL_unref(CLuaTinker::GetLuaTinker().GetLuaState(), LUA_REGISTRYINDEX, _mouseLButtonUpEvent);
		}
		if (_mouseLButtonDownEvent != LUA_NOREF)
		{
			luaL_unref(CLuaTinker::GetLuaTinker().GetLuaState(), LUA_REGISTRYINDEX, _mouseLButtonDownEvent);
		}
		if (_mouseMoveEvent != LUA_NOREF)
		{
			luaL_unref(CLuaTinker::GetLuaTinker().GetLuaState(), LUA_REGISTRYINDEX, _mouseMoveEvent);
		}
		if (_mouseEnterEvent != LUA_NOREF)
		{
			luaL_unref(CLuaTinker::GetLuaTinker().GetLuaState(), LUA_REGISTRYINDEX, _mouseEnterEvent);
		}
		if (_mouseLeaveEvent != LUA_NOREF)
		{
			luaL_unref(CLuaTinker::GetLuaTinker().GetLuaState(), LUA_REGISTRYINDEX, _mouseLeaveEvent);
		}

		Destroy();
	}

	bool CBaseControl::IsHover() const
	{
		return _isHover;
	}

	bool CBaseControl::IsPressed() const
	{
		return _isPressed;
	}

	inline bool CBaseControl::IsEnabled() const
	{
		return _isEnabled;
	}

	inline bool CBaseControl::IsVisible() const
	{
		return _isVisible;
	}

	inline int CBaseControl::GetWidth() const
	{
		return _size.cx;
	}

	inline int CBaseControl::GetHeight() const
	{
		return _size.cy;
	}

	int CBaseControl::GetX() const
	{
		if (_hWnd != nullptr)
		{
			RECT rect;
			GetWindowRect(_hWnd, &rect);
			MapWindowPoints(HWND_DESKTOP, GetParent(_hWnd), (LPPOINT)& rect, 2);
			return rect.left;
		}
		return _position.x;
	}

	int CBaseControl::GetY() const
	{
		if (_hWnd != nullptr)
		{
			RECT rect;
			GetWindowRect(_hWnd, &rect);
			MapWindowPoints(HWND_DESKTOP, GetParent(_hWnd), (LPPOINT)& rect, 2);
			return rect.top;
		}
		return _position.y;
	}

	inline LONG CBaseControl::GetStyle() const
	{
		return _style;
	}

	int CBaseControl::GetCreateEvent() const
	{
		return _createEvent;
	}

	int CBaseControl::GetDestroyEvent() const
	{
		return _destroyEvent;
	}

	int CBaseControl::GetMouseLButtonUpEvent() const
	{
		return _mouseLButtonUpEvent;
	}

	int CBaseControl::GetMouseLButtonDownEvent() const
	{
		return _mouseLButtonDownEvent;
	}

	int CBaseControl::GetMouseMoveEvent() const
	{
		return _mouseMoveEvent;
	}

	int CBaseControl::GetMouseEnterEvent() const
	{
		return _mouseEnterEvent;
	}

	int CBaseControl::GetMouseLeaveEvent() const
	{
		return _mouseLeaveEvent;
	}

	inline HWND CBaseControl::GetHWnd() const
	{
		return _hWnd;
	}

	std::wstring CBaseControl::GetUserData()
	{
		return _userData;
	}

	std::wstring CBaseControl::GetType()
	{
		return _type;
	}

	void CBaseControl::SetStyle(const LONG style)
	{
		if (_style != style)
		{
			_style = style;
		}
	}

	void CBaseControl::SetEnabled(const bool isEnabled)
	{
		if (_isEnabled != isEnabled)
		{
			_isEnabled = isEnabled;

			if (_hWnd != nullptr)
			{
				EnableWindow(_hWnd, _isEnabled);
			}
		}
	}

	void CBaseControl::SetY(const int y)
	{
		if (_position.y != y)
		{
			_position.y = y;

			if (_hWnd != nullptr)
			{
				RECT rect;

				SetRect(&rect, GetX(), _position.y, GetX() + GetWidth(), _position.y + GetHeight());
				AdjustWindowRect(&rect, _style, FALSE);
				const int diffX = GetX() - rect.left;
				const int diffY = _position.y - rect.top;
				SetRect(&rect, rect.left + diffX, rect.top + diffY, rect.right + diffX, rect.bottom + diffY);

				SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
					SWP_NOSIZE || SWP_NOZORDER);
			}
		}
	}

	void CBaseControl::SetX(const int x)
	{
		if (_position.x != x)
		{
			_position.x = x;

			if (_hWnd != nullptr)
			{
				RECT rect;

				SetRect(&rect, _position.x, GetY(), _position.x + GetWidth(), GetY() + GetHeight());
				AdjustWindowRect(&rect, _style, FALSE);
				const int diffX = _position.x - rect.left;
				const int diffY = GetY() - rect.top;
				SetRect(&rect, rect.left + diffX, rect.top + diffY, rect.right + diffX, rect.bottom + diffY);

				SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
					SWP_NOSIZE || SWP_NOZORDER);
			}
		}
	}

	void CBaseControl::SetWidth(const int width)
	{
		if (_size.cx != width)
		{
			_size.cx = width;

			if (_hWnd != nullptr)
			{
				RECT rect;

				SetRect(&rect, GetX(), GetY(), GetX() + _size.cx, GetY() + GetHeight());
				AdjustWindowRect(&rect, _style, FALSE);
				const int diffX = GetX() - rect.left;
				const int diffY = GetY() - rect.top;
				SetRect(&rect, rect.left + diffX, rect.top + diffY, rect.right + diffX, rect.bottom + diffY);

				SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
					SWP_NOMOVE | SWP_NOZORDER);
			}
		}
	}

	void CBaseControl::SetHeight(const int height)
	{
		if (_size.cy != height)
		{
			_size.cy = height;

			if (_hWnd != nullptr)
			{
				RECT rect;

				SetRect(&rect, GetX(), GetY(), GetX() + GetWidth(), GetY() + _size.cy);
				AdjustWindowRect(&rect, _style, FALSE);
				const int diffX = GetX() - rect.left;
				const int diffY = GetY() - rect.top;
				SetRect(&rect, rect.left + diffX, rect.top + diffY, rect.right + diffX, rect.bottom + diffY);

				SetWindowPos(_hWnd, nullptr, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
					SWP_NOMOVE | SWP_NOZORDER);
			}
		}
	}

	void CBaseControl::SetCreateEvent()
	{
		auto l = CLuaTinker::GetLuaTinker().GetLuaState();
		if (lua_isfunction(l, -1))
		{
			lua_pushvalue(l, -1);
			_createEvent = luaL_ref(l, LUA_REGISTRYINDEX);
		}

		lua_pop(l, 1);
	}

	void CBaseControl::SetDestroyEvent()
	{
		auto l = CLuaTinker::GetLuaTinker().GetLuaState();
		if (lua_isfunction(l, -1))
		{
			lua_pushvalue(l, -1);
			_destroyEvent = luaL_ref(l, LUA_REGISTRYINDEX);
		}

		lua_pop(l, 1);
	}

	void CBaseControl::SetMouseLButtonUpEvent()
	{
		auto l = CLuaTinker::GetLuaTinker().GetLuaState();
		if (lua_isfunction(l, -1))
		{
			lua_pushvalue(l, -1);
			_mouseLButtonUpEvent = luaL_ref(l, LUA_REGISTRYINDEX);
		}

		lua_pop(l, 1);
	}

	void CBaseControl::SetMouseLButtonDownEvent()
	{
		auto l = CLuaTinker::GetLuaTinker().GetLuaState();
		if (lua_isfunction(l, -1))
		{
			lua_pushvalue(l, -1);
			_mouseLButtonDownEvent = luaL_ref(l, LUA_REGISTRYINDEX);
		}

		lua_pop(l, 1);
	}

	void CBaseControl::SetMouseMoveEvent()
	{
		auto l = CLuaTinker::GetLuaTinker().GetLuaState();
		if (lua_isfunction(l, -1))
		{
			lua_pushvalue(l, -1);
			_mouseMoveEvent = luaL_ref(l, LUA_REGISTRYINDEX);
		}

		lua_pop(l, 1);
	}

	void CBaseControl::SetMouseEnterEvent()
	{
		auto l = CLuaTinker::GetLuaTinker().GetLuaState();
		if (lua_isfunction(l, -1))
		{
			lua_pushvalue(l, -1);
			_mouseEnterEvent = luaL_ref(l, LUA_REGISTRYINDEX);
		}

		lua_pop(l, 1);
	}

	void CBaseControl::SetMouseLeaveEvent()
	{
		auto l = CLuaTinker::GetLuaTinker().GetLuaState();
		if (lua_isfunction(l, -1))
		{
			lua_pushvalue(l, -1);
			_mouseLeaveEvent = luaL_ref(l, LUA_REGISTRYINDEX);
		}

		lua_pop(l, 1);
	}

	void CBaseControl::SetUserData(std::wstring data)
	{
		if (_userData != data)
		{
			_userData = data;
		}
	}

	void CBaseControl::SetParentControl(CBaseControl* parent)
	{
		_parentControl = parent;
	}

	void CBaseControl::Refresh()
	{
		if (_hWnd != nullptr)
		{
			RECT rect;

			GetClientRect(_hWnd, &rect);
			InvalidateRect(_hWnd, &rect, TRUE);
			UpdateWindow(_hWnd);
		}
	}

	void CBaseControl::RefreshRegion(int left, int top, int right, int bottom)
	{
		if (_hWnd != nullptr)
		{
			RECT rect;

			SetRect(&rect, left, top, right, bottom);
			InvalidateRect(_hWnd, &rect, TRUE);
			UpdateWindow(_hWnd);
		}
	}

	void CBaseControl::Show()
	{
		_isVisible = true;
		_style |= WS_VISIBLE;

		if (_hWnd != nullptr)
		{
			ShowWindow(_hWnd, TRUE);
			if (_parentControl != nullptr)
			{
				UpdateWindow(_parentControl->GetHWnd());
			}
		}
	}

	void CBaseControl::Hide()
	{
		_isVisible = false;
		_style &= ~WS_VISIBLE;

		if (_hWnd != nullptr)
		{
			ShowWindow(_hWnd, FALSE);
		}
	}
} // namespace jojogame