#include "LuaControl.h"
#include "../LuaJojo.h"
#include "../LuaTinker.h"

#include <WindowsX.h>

namespace jojogame {
LuaControl::LuaControl()
{
}

LuaControl::~LuaControl()
{
}

inline bool LuaControl::bold() const
{
    return _bold;
}

inline bool LuaControl::enabled() const
{
    return _enabled;
}

inline bool LuaControl::italic() const
{
    return _italic;
}

inline bool LuaControl::underline() const
{
    return _underline;
}

inline bool LuaControl::visible() const
{
    return _visible;
}

inline std::wstring LuaControl::fontName() const
{
    return _fontName;
}

inline int LuaControl::fontSize() const
{
    return _fontSize;
}

inline int LuaControl::width() const
{
    return _size.cx;
}

inline int LuaControl::height() const
{
    return _size.cy;
}

inline int LuaControl::x() const
{
    return _position.x;
}

inline int LuaControl::y() const
{
    return _position.y;
}

inline LONG LuaControl::style() const
{
    return _style;
}

inline std::wstring LuaControl::createFunction() const
{
    return _createFunction;
}

inline std::wstring LuaControl::destroyFunction() const
{
    return _destroyFunction;
}

inline std::wstring LuaControl::clickFunction() const
{
    return _clickFunction;
}

inline HWND LuaControl::parentHwnd() const
{
    return _parentHwnd;
}

void LuaControl::resetFont()
{
    if (_font != nullptr)
    {
        DeleteFont(_font);
    }

    _font = CreateFontW(_fontSize,
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
                        _fontName.c_str());

    if (_hwnd != nullptr)
    {
        SendMessage(_hwnd, WM_SETFONT, (WPARAM)_font, (LPARAM)TRUE);
    }

    RECT rect;
    SetRect(&rect, _position.x, _position.y, _position.x + _size.cx, _position.y + _size.cy);
    InvalidateRect(_parentHwnd, &rect, true);
}

void LuaControl::setBold(const bool bold)
{
    _bold = bold;
    resetFont();
}

void LuaControl::setItalic(const bool italic)
{
    _italic = italic;
    resetFont();
}

void LuaControl::setUnderline(const bool underline)
{
    _underline = underline;
    resetFont();
}

void LuaControl::setFontName(const std::wstring fontName)
{
    _fontName = fontName;
    resetFont();
}

void LuaControl::setFontSize(const int fontSize)
{
    _fontSize = fontSize;
    resetFont();
}

void LuaControl::setStyle(const LONG style)
{
    _style = style;
}

void LuaControl::setVisible(const bool visible)
{
    _visible = visible;
}

void LuaControl::setEnabled(const bool enabled)
{
    _enabled = enabled;
}

void LuaControl::setY(const int top)
{
    _position.y = top;
}

void LuaControl::setX(const int left)
{
    _position.x = left;
}

void LuaControl::setWidth(const int width)
{
    _size.cx = width;
}

void LuaControl::setHeight(const int height)
{
    _size.cy = height;
}

void LuaControl::setCreateFunction(const std::wstring createFunction)
{
    _createFunction = createFunction;
}

void LuaControl::setDestroyFunction(const std::wstring destroyFunction)
{
    _destroyFunction = destroyFunction;
}

void LuaControl::setClickFunction(const std::wstring clickFunction)
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
    auto parentHwnd = _parentHwnd;
    if (parentHwnd == nullptr)
    {
        parentHwnd = GetDesktopWindow();
    }

    RECT parentRect;
    GetWindowRect(parentHwnd, &parentRect);

    const int parentWidth = parentRect.right - parentRect.left;
    const int parentHeight = parentRect.bottom - parentRect.top;
    _position.x = parentWidth / 2 - _size.cx / 2;
    _position.y = parentHeight / 2 - _size.cy / 2;

    if (_hwnd != nullptr)
    {
        MoveWindow(_hwnd, _position.x, _position.y, _size.cx, _size.cy, TRUE);
    }
}

void LuaControl::registerToLua()
{
    const auto lua = LuaJojo::getInstance()->getLuaState();

    lua_tinker::class_add<LuaControl>(lua, "__control__");

    lua_tinker::class_def<LuaControl>(lua, "setVisible", &LuaControl::setVisible);
    lua_tinker::class_def<LuaControl>(lua, "setEnabled", &LuaControl::setEnabled);
    lua_tinker::class_def<LuaControl>(lua, "setWidth", &LuaControl::setWidth);
    lua_tinker::class_def<LuaControl>(lua, "setHeight", &LuaControl::setHeight);
    lua_tinker::class_def<LuaControl>(lua, "setY", &LuaControl::setY);
    lua_tinker::class_def<LuaControl>(lua, "setX", &LuaControl::setX);
    lua_tinker::class_def<LuaControl>(lua, "setStyle", &LuaControl::setStyle);
    lua_tinker::class_def<LuaControl>(lua, "visible", &LuaControl::visible);
    lua_tinker::class_def<LuaControl>(lua, "enabled", &LuaControl::enabled);
    lua_tinker::class_def<LuaControl>(lua, "width", &LuaControl::width);
    lua_tinker::class_def<LuaControl>(lua, "height", &LuaControl::height);
    lua_tinker::class_def<LuaControl>(lua, "y", &LuaControl::y);
    lua_tinker::class_def<LuaControl>(lua, "x", &LuaControl::x);
    lua_tinker::class_def<LuaControl>(lua, "style", &LuaControl::style);
    lua_tinker::class_def<LuaControl>(lua, "show", &LuaControl::show);
    lua_tinker::class_def<LuaControl>(lua, "hide", &LuaControl::hide);
    lua_tinker::class_def<LuaControl>(lua, "moveToCenter", &LuaControl::moveToCenter);
    lua_tinker::class_def<LuaControl>(lua, "setItalic", &LuaControl::setItalic);
    lua_tinker::class_def<LuaControl>(lua, "setUnderline", &LuaControl::setUnderline);
    lua_tinker::class_def<LuaControl>(lua, "setBold", &LuaControl::setBold);
    lua_tinker::class_def<LuaControl>(lua, "setFontSize", &LuaControl::setFontSize);
    lua_tinker::class_def<LuaControl>(lua, "setFontName", &LuaControl::setFontName);
    lua_tinker::class_def<LuaControl>(lua, "italic", &LuaControl::italic);
    lua_tinker::class_def<LuaControl>(lua, "underline", &LuaControl::underline);
    lua_tinker::class_def<LuaControl>(lua, "bold", &LuaControl::bold);
    lua_tinker::class_def<LuaControl>(lua, "fondSize", &LuaControl::fontSize);
    lua_tinker::class_def<LuaControl>(lua, "fontName", &LuaControl::fontName);
}
}
