#include "StaticControl.h"
#include "ControlManager.h"
#include <vector>

namespace three_kingdoms
{
WNDPROC CStaticControl::s_originalProc = nullptr;

LRESULT CStaticControl::OnControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        auto createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        void *lpParamCreate = createStruct->lpCreateParams;
        auto staticControl = reinterpret_cast<CStaticControl *>(lpParamCreate);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(staticControl));

        auto createEvent = staticControl->GetCreateEvent();
        if (createEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(createEvent, staticControl);
        }
        break;
    }

    case WM_DESTROY:
    {
        auto staticControl = reinterpret_cast<CStaticControl *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        auto destroyEvent = staticControl->GetDestroyEvent();

        if (destroyEvent != LUA_NOREF)
        {
            CLuaTinker::GetLuaTinker().Call(destroyEvent, staticControl);
        }
        break;
    }
    }

    return CallWindowProc(CStaticControl::GetOriginalProc(), hWnd, msg, wParam, lParam);
}

void SplitString(std::wstring src, std::wstring delimiter, std::vector<std::wstring> *out)
{
    size_t pos = 0;

    while ((pos = src.find(delimiter)) != std::wstring::npos)
    {
        auto token = src.substr(0, pos);
        out->push_back(token);
        src.erase(0, pos + delimiter.length());
    }

    out->push_back(src);
}

void CStaticControl::_AutoSizing()
{
    if (_isAutoSize && _hWnd != nullptr)
    {
        auto hdc = GetDC(_hWnd);
        auto originalFont = SelectFont(hdc, _font.GetHFont());

        RECT rect{0, 0, 0, 0};
        DrawText(hdc, _text.c_str(), _text.length(), &rect, DT_CALCRECT);
        SetWidth(rect.right);
        SetHeight(rect.bottom);
        SelectFont(hdc, originalFont);

        ReleaseDC(_hWnd, hdc);
    }
}

void CStaticControl::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN_CHILD(CStaticControl, "_StaticControl", CWindowChildControl);

    LUA_METHOD(GetText);
    LUA_METHOD(GetFont);

    LUA_METHOD(SetAutoSize);
    LUA_METHOD(SetText);
    LUA_METHOD(SetTextColor);
    LUA_METHOD(SetBackgroundColor);
    LUA_METHOD(SetTransparentBackground);
    LUA_METHOD(SetAlign);

    LUA_METHOD(Create);
    LUA_METHOD(Destroy);

    WNDCLASS wndClass;
    GetClassInfo(NULL, TEXT("static"), &wndClass);
    s_originalProc = wndClass.lpfnWndProc;
    wndClass.hInstance = CControlManager::GetInstance().GetHInstance();
    wndClass.lpfnWndProc = (WNDPROC)CStaticControl::OnControlProc;
    wndClass.lpszClassName = L"jojo_static";
    RegisterClass(&wndClass);
}

CStaticControl::CStaticControl()
    : _font(this)
{
    _type = L"static";
    _style = WS_CHILD | SS_OWNERDRAW;
}

CStaticControl::~CStaticControl()
{
    if (_backgroundBrush != nullptr)
    {
        DeleteBrush(_backgroundBrush);
    }
}

bool CStaticControl::IsAutoSize()
{
    return _isAutoSize;
}

bool CStaticControl::IsTransparentBackground()
{
    return _isTransparentBackground;
}

int CStaticControl::GetAlign()
{
    return _align;
}

std::wstring CStaticControl::GetText()
{
    return _text;
}

CTextFont *CStaticControl::GetFont()
{
    return &_font;
}

COLORREF CStaticControl::GetTextColor()
{
    return _textColor;
}

HBRUSH CStaticControl::GetBackgroundBrush()
{
    return _backgroundBrush;
}

void CStaticControl::SetAutoSize(bool value)
{
    if (_isAutoSize != value)
    {
        _isAutoSize = value;

        _AutoSizing();
    }
}

void CStaticControl::SetTransparentBackground(bool value)
{
    if (_isTransparentBackground != value)
    {
        _isTransparentBackground = value;

        Refresh();
    }
}

void CStaticControl::SetAlign(int align)
{
    if (_align != align)
    {
        _align = align;

        Refresh();
    }
}

void CStaticControl::SetText(std::wstring text)
{
    if (_text != text)
    {
        _text = text;

        if (_hWnd != nullptr)
        {
            SetWindowText(_hWnd, _text.c_str());
        }

        _AutoSizing();
    }
}

void CStaticControl::SetTextColor(COLORREF color)
{
    if (_textColor != color)
    {
        _textColor = color;

        Refresh();
    }
}

void CStaticControl::SetBackgroundColor(COLORREF color)
{
    if (_backgroundColor != color)
    {
        _backgroundColor = color;

        if (_backgroundBrush)
        {
            DeleteBrush(_backgroundBrush);
            _backgroundBrush = nullptr;
        }

        _backgroundBrush = CreateSolidBrush(_backgroundColor);

        Refresh();
    }
}

bool CStaticControl::Create()
{
    if (_parentControl != nullptr)
    {
        _hWnd = CreateWindow(L"jojo_static",
                             _text.c_str(),
                             _style,
                             _position.x,
                             _position.y,
                             _size.cx,
                             _size.cy,
                             _parentControl->GetHWnd(),
                             (HMENU)this,
                             CControlManager::GetInstance().GetHInstance(),
                             this);

        _AutoSizing();
    }

    return _hWnd != nullptr;
}

void CStaticControl::Show()
{
    _isVisible = true;
    _style |= WS_VISIBLE;

    if (_hWnd != nullptr)
    {
        ShowWindow(_hWnd, TRUE);
        UpdateWindow(_parentControl->GetHWnd());
    }
}

WNDPROC CStaticControl::GetOriginalProc()
{
    return s_originalProc;
}
} // namespace jojogame
