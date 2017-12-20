#include "LuaControl.h"

#include <WindowsX.h>


namespace jojogame {
	LuaControl::LuaControl()
	{
	}

	LuaControl::~LuaControl()
	{
	}

	inline bool LuaControl::bold() const { return _bold; }
	
	inline bool LuaControl::enabled() const { return _enabled; }
	
	inline bool LuaControl::italic() const { return _italic; }
	
	inline bool LuaControl::underline() const { return _underline; }
	
	inline bool LuaControl::visible() const { return _visible; }
	
	inline wchar_t * LuaControl::fontName() const { return _fontName; }
	
	inline int LuaControl::fontSize() const { return _fontSize; }
	
	inline int LuaControl::width() const { return _size.cx; }
	
	inline int LuaControl::height() const { return _size.cy; }
	
	inline int LuaControl::left() const { return _position.x; }
	
	inline int LuaControl::top() const { return _position.y; }
	
	inline LONG LuaControl::style() const { return _style; }
	
	inline wchar_t * LuaControl::createFunction() const { return _createFunction; }
	
	inline wchar_t * LuaControl::destroyFunction() const { return _destroyFunction; }
	
	inline wchar_t * LuaControl::clickFunction() const { return _clickFunction; }

	void LuaControl::resetFont()
	{
		if (_font != nullptr) {
			DeleteFont(_font);
		}

		_font = CreateFontW(
			_fontSize,
			0,
			0,
			0,
			FW_NORMAL,
			_italic,
			_underline,
			_bold,
			HANGEUL_CHARSET,
			0,
			0,
			0,
			VARIABLE_PITCH | FF_ROMAN,
			_fontName);

		if (_hwnd != nullptr) {
			SendMessage(_hwnd, WM_SETFONT, (WPARAM)_font, (LPARAM)TRUE);
		}

		RECT rect;
		SetRect(&rect, _position.x, _position.y, _position.x + _size.cx, _position.y + _size.cy);
		InvalidateRect(_parentHwnd, &rect, true);
	}

	void LuaControl::setBold(bool bold)
	{
		_bold = bold;
		resetFont();
	}

	void LuaControl::setItalic(bool italic)
	{
		_italic = italic;
		resetFont();
	}

	void LuaControl::setUnderline(bool underline)
	{
		_underline = underline;
		resetFont();
	}

	void LuaControl::setFontName(wchar_t * fontName)
	{
		_fontName = fontName;
		resetFont();
	}

	void LuaControl::setFontSize(int fontSize)
	{
		_fontSize = fontSize;
		resetFont();
	}

	void LuaControl::setStyle(LONG style)
	{
		_style = style;
	}

	void LuaControl::setVisible(bool visible)
	{
		_visible = visible;
	}

	void LuaControl::setEnabled(bool enabled)
	{
		_enabled = enabled;
	}

	void LuaControl::setTop(int top)
	{
		_position.y = top;
	}

	void LuaControl::setLeft(int left)
	{
		_position.x = left;
	}

	void LuaControl::setWidth(int width)
	{
		_size.cx = width;
	}

	void LuaControl::setHeight(int height)
	{
		_size.cy = height;
	}

	void LuaControl::setCreateFunction(wchar_t * createFunction)
	{
		_createFunction = createFunction;
	}

	void LuaControl::setDestroyFunction(wchar_t * destroyFunction)
	{
		_destroyFunction = destroyFunction;
	}

	void LuaControl::setClickFunction(wchar_t * clickFunction)
	{
		_clickFunction = clickFunction;
	}

	void LuaControl::show()
	{
		ShowWindow(_hwnd, TRUE);
	}

	void LuaControl::hide()
	{
		ShowWindow(_hwnd, FALSE);
	}

	void LuaControl::moveToCenter()
	{
		HWND parentHwnd = _parentHwnd;
		if (parentHwnd == NULL) {
			parentHwnd = GetDesktopWindow();
		}

		RECT parentRect, rect;
		int parentWidth, parentHeight;
		int width, height;

		GetWindowRect(parentHwnd, &parentRect);
		GetWindowRect(_hwnd, &rect);

		parentWidth = parentRect.right - parentRect.left;
		parentHeight = parentRect.bottom - parentRect.top;
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
		rect.left = parentWidth / 2 - width / 2;
		rect.top = parentHeight / 2 - height / 2;
		_position.x = rect.left;
		_position.y = rect.top;
		MoveWindow(_hwnd, rect.left, rect.top, width, height, TRUE);
	}
}
