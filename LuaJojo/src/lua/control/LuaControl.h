#pragma once

#include <Windows.h>

namespace jojogame {
	class __declspec(dllexport) LuaControl {
	public:
		LuaControl();
		virtual ~LuaControl();

		LuaControl(const LuaControl&) = delete;
		LuaControl& operator=(const LuaControl&) = delete;

		bool bold() const;
		bool enabled() const;
		bool italic() const;
		bool underline() const;
		bool visible() const;
		wchar_t *fontName() const;
		int fontSize() const;
		int width() const;
		int height() const;
		int left() const;
		int top() const;
		LONG style() const;
		wchar_t* createFunction() const;
		wchar_t* destroyFunction() const;
		wchar_t* clickFunction() const;

		void setParentHwnd(HWND parentHwnd);
		void setBold(bool bold);
		void setItalic(bool italic);
		void setUnderline(bool underline);
		void setFontName(wchar_t *fontName);
		void setFontSize(int fontSize);
		void setStyle(LONG style);
		void setVisible(bool visible);
		void setEnabled(bool enabled);
		void setTop(int top);
		void setLeft(int left);
		void setWidth(int width);
		void setHeight(int height);
		void setCreateFunction(wchar_t *createFunction);
		void setDestroyFunction(wchar_t *destroyFunction);
		void setClickFunction(wchar_t *clickFunction);

		void show();

		void hide();

		void moveToCenter();

		virtual void create() = 0;
		virtual void destroy() = 0;

		static void registerToLua();

	protected:
		void resetFont();

		bool _visible = false;
		bool _enabled = true;
		HWND _hwnd = nullptr;
		HWND _parentHwnd = nullptr;
		SIZE _size;
		POINT _position;
		LONG _style = 0;
		HFONT _font = nullptr;
		bool _bold = false;
		bool _italic = false;
		bool _underline = false;
		int _fontSize = 12;

		wchar_t *_fontName;
		wchar_t *_createFunction;
		wchar_t *_destroyFunction;
		wchar_t *_clickFunction;
	protected:
	};
}