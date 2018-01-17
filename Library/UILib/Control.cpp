#include "LuaControl.h"
#include "../LuaJojo.h"
#include "../LuaTinker.h"

#include <WindowsX.h>

namespace jojogame {
CControl::CControl()
{
}

CControl::~CControl()
{
}

inline bool CControl::bold() const
{
    return _bold;
}

inline bool CControl::enabled() const
{
    return _enabled;
}

inline bool CControl::italic() const
{
    return _italic;
}

inline bool CControl::underline() const
{
    return _underline;
}

inline bool CControl::visible() const
{
    return _visible;
}

inline std::wstring CControl::fontName() const
{
    return _fontName;
}

inline int CControl::fontSize() const
{
    return _fontSize;
}

inline int CControl::width() const
{
    return _size.cx;
}

inline int CControl::height() const
{
    return _size.cy;
}

inline int CControl::x() const
{
    return _position.x;
}

inline int CControl::y() const
{
    return _position.y;
}

inline LONG CControl::style() const
{
    return _style;
}

inline std::wstring CControl::createFunction() const
{
    return _createFunction;
}

inline std::wstring CControl::destroyFunction() const
{
    return _destroyFunction;
}

inline std::wstring CControl::clickFunction() const
{
    return _clickFunction;
}

inline HWND CControl::parentHwnd() const
{
    return _parentHwnd;
}

void CControl::resetFont()
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

void CControl::setBold(const bool bold)
{
    _bold = bold;
    resetFont();
}

void CControl::setItalic(const bool italic)
{
    _italic = italic;
    resetFont();
}

void CControl::setUnderline(const bool underline)
{
    _underline = underline;
    resetFont();
}

void CControl::setFontName(const std::wstring fontName)
{
    _fontName = fontName;
    resetFont();
}

void CControl::setFontSize(const int fontSize)
{
    _fontSize = fontSize;
    resetFont();
}

void CControl::setStyle(const LONG style)
{
    _style = style;
}

void CControl::setVisible(const bool visible)
{
    _visible = visible;
}

void CControl::setEnabled(const bool enabled)
{
    _enabled = enabled;
}

void CControl::setY(const int top)
{
    _position.y = top;
}

void CControl::setX(const int left)
{
    _position.x = left;
}

void CControl::setWidth(const int width)
{
    _size.cx = width;
}

void CControl::setHeight(const int height)
{
    _size.cy = height;
}

void CControl::setCreateFunction(const std::wstring createFunction)
{
    _createFunction = createFunction;
}

void CControl::setDestroyFunction(const std::wstring destroyFunction)
{
    _destroyFunction = destroyFunction;
}

void CControl::setClickFunction(const std::wstring clickFunction)
{
    _clickFunction = clickFunction;
}

void CControl::show()
{
    ShowWindow(_hwnd, TRUE);
}

void CControl::hide()
{
    ShowWindow(_hwnd, FALSE);
}

void CControl::moveToCenter()
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

void CControl::registerToLua()
{
    const auto lua = LuaJojo::getInstance()->getLuaState();

    lua_tinker::class_add<CControl>(lua, "__control__");

    lua_tinker::class_def<CControl>(lua, "setVisible", &CControl::setVisible);
    lua_tinker::class_def<CControl>(lua, "setEnabled", &CControl::setEnabled);
    lua_tinker::class_def<CControl>(lua, "setWidth", &CControl::setWidth);
    lua_tinker::class_def<CControl>(lua, "setHeight", &CControl::setHeight);
    lua_tinker::class_def<CControl>(lua, "setY", &CControl::setY);
    lua_tinker::class_def<CControl>(lua, "setX", &CControl::setX);
    lua_tinker::class_def<CControl>(lua, "setStyle", &CControl::setStyle);
    lua_tinker::class_def<CControl>(lua, "visible", &CControl::visible);
    lua_tinker::class_def<CControl>(lua, "enabled", &CControl::enabled);
    lua_tinker::class_def<CControl>(lua, "width", &CControl::width);
    lua_tinker::class_def<CControl>(lua, "height", &CControl::height);
    lua_tinker::class_def<CControl>(lua, "y", &CControl::y);
    lua_tinker::class_def<CControl>(lua, "x", &CControl::x);
    lua_tinker::class_def<CControl>(lua, "style", &CControl::style);
    lua_tinker::class_def<CControl>(lua, "show", &CControl::show);
    lua_tinker::class_def<CControl>(lua, "hide", &CControl::hide);
    lua_tinker::class_def<CControl>(lua, "moveToCenter", &CControl::moveToCenter);
    lua_tinker::class_def<CControl>(lua, "setItalic", &CControl::setItalic);
    lua_tinker::class_def<CControl>(lua, "setUnderline", &CControl::setUnderline);
    lua_tinker::class_def<CControl>(lua, "setBold", &CControl::setBold);
    lua_tinker::class_def<CControl>(lua, "setFontSize", &CControl::setFontSize);
    lua_tinker::class_def<CControl>(lua, "setFontName", &CControl::setFontName);
    lua_tinker::class_def<CControl>(lua, "italic", &CControl::italic);
    lua_tinker::class_def<CControl>(lua, "underline", &CControl::underline);
    lua_tinker::class_def<CControl>(lua, "bold", &CControl::bold);
    lua_tinker::class_def<CControl>(lua, "fondSize", &CControl::fontSize);
    lua_tinker::class_def<CControl>(lua, "fontName", &CControl::fontName);
}
}
